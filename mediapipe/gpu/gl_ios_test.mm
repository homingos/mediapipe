// Copyright 2024 Flamingos Technologies Inc. All Rights Reserved.
//
// PROPRIETARY AND CONFIDENTIAL
//
// This software and its documentation are the confidential and proprietary
// information of Flamingos Technologies Inc. ("Confidential Information").
// You shall not disclose such Confidential Information and shall use it
// only in accordance with the terms of the license agreement you entered
// into with Flamingos Technologies Inc. Unauthorized copying, redistribution,
// or use of this software in source or binary forms is strictly prohibited.

#import <UIKit/UIKit.h>
#import <XCTest/XCTest.h>

#import "mediapipe/framework/tool/source.pb.h"
#import "mediapipe/gpu/gpu_shared_data_internal.h"
#import "mediapipe/objc/MPPGraph.h"
#import "mediapipe/objc/MPPGraphTestBase.h"
#import "mediapipe/objc/util.h"

#include "absl/memory/memory.h"
#import "mediapipe/framework/calculator_framework.h"
#import "mediapipe/gpu/gl_calculator_helper.h"

@interface GLIOSTests : MPPGraphTestBase{
  UIImage* _sourceImage;
  MPPGraph* _graph;
}
@end

@implementation GLIOSTests

- (void)setUp {
  [super setUp];

  _sourceImage = [self testImageNamed:@"googlelogo_color_272x92dp" extension:@"png"];
}

- (void)tearDown {
  [super tearDown];
}

- (CVPixelBufferRef)redPixelBuffer:(CVPixelBufferRef)input {
  return [self transformPixelBuffer:input
                  outputPixelFormat:kCVPixelFormatType_32BGRA
                     transformation:^(CVPixelBufferRef input,
                                      CVPixelBufferRef output) {
    vImage_Buffer vInput = vImageForCVPixelBuffer(input);
    vImage_Buffer vRed = vImageForCVPixelBuffer(output);

    static const int16_t matrix[16] = {
      0, 0, 0, 0,
      0, 0, 0, 0,
      0, 0, 256, 0,
      0, 0, 0, 256,
    };
    vImage_Error vError = vImageMatrixMultiply_ARGB8888(
         &vInput, &vRed, matrix, 256, NULL, NULL, 0);
    XCTAssertEqual(vError, kvImageNoError);
  }];
}

- (CVPixelBufferRef)luminancePixelBuffer:(CVPixelBufferRef)input {
  return [self transformPixelBuffer:input
                  outputPixelFormat:kCVPixelFormatType_32BGRA
                     transformation:^(CVPixelBufferRef input,
                                      CVPixelBufferRef output) {
    vImage_Buffer vInput = vImageForCVPixelBuffer(input);
    vImage_Buffer vLuminance = vImageForCVPixelBuffer(output);

    // sRGB weights: R 0.2125, G 0.7154, B 0.0721
    static const int16_t matrix[16] = {
      721, 721, 721, 0,
      7154, 7154, 7154, 0,
      2125, 2125, 2125, 0,
      0, 0, 0, 10000,
    };
    vImage_Error vError = vImageMatrixMultiply_ARGB8888(
         &vInput, &vLuminance, matrix, 10000, NULL, NULL, 0);
    XCTAssertEqual(vError, kvImageNoError);
  }];
}

- (CVPixelBufferRef)grayPixelBuffer:(CVPixelBufferRef)input {
  return [self transformPixelBuffer:input
                  outputPixelFormat:kCVPixelFormatType_OneComponent8
                     transformation:^(CVPixelBufferRef input,
                                      CVPixelBufferRef output) {
    vImage_Buffer vInput = vImageForCVPixelBuffer(input);
    vImage_Buffer vGray = vImageForCVPixelBuffer(output);
    vImage_Error vError = vImageBGRAToGray(&vInput, &vGray);
    XCTAssertEqual(vError, kvImageNoError);
  }];
}

- (void)testGlConverters {
  CFHolder<CVPixelBufferRef> originalPixelBuffer;
  absl::Status status =
      CreateCVPixelBufferFromCGImage([_sourceImage CGImage], &originalPixelBuffer);
  XCTAssert(status.ok());

  mediapipe::CalculatorGraphConfig config;
  config.add_input_stream("input_frames");
  auto node = config.add_node();
  node->set_calculator("GpuBufferToImageFrameCalculator");
  node->add_input_stream("input_frames");
  node->add_output_stream("image_frames");
  auto node2 = config.add_node();
  node2->set_calculator("ImageFrameToGpuBufferCalculator");
  node2->add_input_stream("image_frames");
  node2->add_output_stream("output_frames");

  _graph = [[MPPGraph alloc] initWithGraphConfig:config];
  [_graph addFrameOutputStream:"output_frames"
              outputPacketType:MPPPacketTypePixelBuffer];
  [self testGraph:_graph input:*originalPixelBuffer expectedOutput:*originalPixelBuffer];
}

- (void)testGlConvertersNoOpInserted {
  CFHolder<CVPixelBufferRef> originalPixelBuffer;
  absl::Status status =
      CreateCVPixelBufferFromCGImage([_sourceImage CGImage], &originalPixelBuffer);
  XCTAssert(status.ok());

  mediapipe::CalculatorGraphConfig config;
  config.add_input_stream("input_frames");
  auto node = config.add_node();
  node->set_calculator("GpuBufferToImageFrameCalculator");
  node->add_input_stream("input_frames");
  node->add_output_stream("image_frames");
  // This node should be a no-op, since its inputs are already ImageFrames.
  auto no_op_node = config.add_node();
  no_op_node->set_calculator("GpuBufferToImageFrameCalculator");
  no_op_node->add_input_stream("image_frames");
  no_op_node->add_output_stream("still_image_frames");
  auto node2 = config.add_node();
  node2->set_calculator("ImageFrameToGpuBufferCalculator");
  node2->add_input_stream("still_image_frames");
  node2->add_output_stream("output_frames");

  _graph = [[MPPGraph alloc] initWithGraphConfig:config];
  [_graph addFrameOutputStream:"output_frames"
              outputPacketType:MPPPacketTypePixelBuffer];
  [self testGraph:_graph input:*originalPixelBuffer expectedOutput:*originalPixelBuffer];
}

- (void)testGlConvertersWithOptionalSidePackets {
  CFHolder<CVPixelBufferRef> originalPixelBuffer;
  absl::Status status =
      CreateCVPixelBufferFromCGImage([_sourceImage CGImage], &originalPixelBuffer);
  XCTAssert(status.ok());

  mediapipe::CalculatorGraphConfig config;
  config.add_input_stream("input_frames");
  auto node = config.add_node();
  node->set_calculator("GpuBufferToImageFrameCalculator");
  node->add_input_stream("input_frames");
  node->add_output_stream("image_frames");
  auto node2 = config.add_node();
  node2->set_calculator("ImageFrameToGpuBufferCalculator");
  node2->add_input_stream("image_frames");
  node2->add_output_stream("output_frames");

  _graph = [[MPPGraph alloc] initWithGraphConfig:config];
  [_graph addFrameOutputStream:"output_frames"
              outputPacketType:MPPPacketTypePixelBuffer];
  [self testGraph:_graph input:*originalPixelBuffer expectedOutput:*originalPixelBuffer];
}

- (void)testDestinationSizes {
  mediapipe::GpuSharedData gpuData;
  mediapipe::GlCalculatorHelper helper;
  helper.InitializeForTest(&gpuData);

  helper.RunInGlContext([&helper] {
    std::vector<std::pair<int, int>> sizes{
        {200, 300}, {200, 299}, {196, 300}, {194, 300}, {193, 300},
    };
    for (const auto& width_height : sizes) {
      mediapipe::GlTexture texture =
          helper.CreateDestinationTexture(width_height.first, width_height.second);
      XCTAssertNotEqual(texture.name(), 0);
    }
  });
}

- (void)testSimpleConversionFromFormat:(OSType)cvPixelFormat {
  CFHolder<CVPixelBufferRef> originalPixelBuffer;
  absl::Status status =
      CreateCVPixelBufferFromCGImage([_sourceImage CGImage], &originalPixelBuffer);
  XCTAssert(status.ok());
  CVPixelBufferRef convertedPixelBuffer =
      [self convertPixelBuffer:*originalPixelBuffer
                 toPixelFormat:cvPixelFormat];
  CVPixelBufferRef bgraPixelBuffer =
      [self convertPixelBuffer:convertedPixelBuffer
                 toPixelFormat:kCVPixelFormatType_32BGRA];

  mediapipe::CalculatorGraphConfig config;
  config.add_input_stream("input_frames");
  auto node = config.add_node();
  node->set_calculator("GlScalerCalculator");
  node->add_input_stream("input_frames");
  node->add_output_stream("output_frames");

  _graph = [[MPPGraph alloc] initWithGraphConfig:config];
  [_graph addFrameOutputStream:"output_frames"
              outputPacketType:MPPPacketTypePixelBuffer];
  [self testGraph:_graph input:convertedPixelBuffer expectedOutput:bgraPixelBuffer];
  CFRelease(convertedPixelBuffer);
  CFRelease(bgraPixelBuffer);
}

- (void)testOneComponent8 {
  [self testSimpleConversionFromFormat:kCVPixelFormatType_OneComponent8];
}

- (void)testMetalRgbWeight {
#if TARGET_IPHONE_SIMULATOR
  NSLog(@"Metal tests skipped on Simulator.");
#else
  CFHolder<CVPixelBufferRef> originalPixelBuffer;
  absl::Status status =
      CreateCVPixelBufferFromCGImage([_sourceImage CGImage], &originalPixelBuffer);
  XCTAssert(status.ok());
  CVPixelBufferRef redPixelBuffer = [self redPixelBuffer:*originalPixelBuffer];

  mediapipe::CalculatorGraphConfig config;
  config.add_input_stream("input_frames");
  auto node = config.add_node();
  node->set_calculator("MetalRgbWeightCalculator");
  node->add_input_stream("input_frames");
  node->add_output_stream("output_frames");
  node->add_input_side_packet("WEIGHTS:rgb_weights");

  _graph = [[MPPGraph alloc] initWithGraphConfig:config];
  [_graph addFrameOutputStream:"output_frames"
              outputPacketType:MPPPacketTypePixelBuffer];
  [_graph setSidePacket:(mediapipe::MakePacket<float[3]>(1.0, 0.0, 0.0))
                  named:"rgb_weights"];

  [self testGraph:_graph input:*originalPixelBuffer expectedOutput:redPixelBuffer];
  CFRelease(redPixelBuffer);
#endif  // TARGET_IPHONE_SIMULATOR
}

@end
