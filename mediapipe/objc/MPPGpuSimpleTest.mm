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

#import <XCTest/XCTest.h>

#import "mediapipe/objc/MPPGraph.h"
#import "mediapipe/objc/MPPGraphTestBase.h"

@interface MPPGpuSimpleTest : MPPGraphTestBase
@end

@implementation MPPGpuSimpleTest{
  CFHolder<CVPixelBufferRef> _inputPixelBuffer;
  CFHolder<CVPixelBufferRef> _referencePixelBuffer;
  CFHolder<CVPixelBufferRef> _outputPixelBuffer;
}
- (void)setUp {
  [super setUp];
  UIImage* image = [self testImageNamed:@"sergey" extension:@"png"];
  XCTAssertTrue(CreateCVPixelBufferFromCGImage(image.CGImage, &_inputPixelBuffer).ok());
  image = [self testImageNamed:@"sobel_reference" extension:@"png"];
  XCTAssertTrue(CreateCVPixelBufferFromCGImage(image.CGImage, &_referencePixelBuffer).ok());
}

// This delegate method receives output.
- (void)mediapipeGraph:(MPPGraph*)graph
    didOutputPixelBuffer:(CVPixelBufferRef)pixelBuffer
              fromStream:(const std::string&)streamName
               timestamp:(const mediapipe::Timestamp&)timestamp {
  NSLog(@"CALLBACK INVOKED");
  _outputPixelBuffer.reset(pixelBuffer);
}

- (void)testSimpleGpuGraph {
  // Graph setup.
  NSData* configData = [self testDataNamed:@"test_sobel.binarypb" extension:nil];
  mediapipe::CalculatorGraphConfig config;
  XCTAssertTrue(config.ParseFromArray([configData bytes], [configData length]));
  MPPGraph* mediapipeGraph = [[MPPGraph alloc] initWithGraphConfig:config];
  // We receive output by setting ourselves as the delegate.
  mediapipeGraph.delegate = self;
  [mediapipeGraph addFrameOutputStream:"output_video" outputPacketType:MPPPacketTypePixelBuffer];

  // Start running the graph.
  NSError *error;
  BOOL success = [mediapipeGraph startWithError:&error];
  XCTAssertTrue(success, @"%@", error.localizedDescription);

  // Send a frame.
  XCTAssertTrue([mediapipeGraph sendPixelBuffer:*_inputPixelBuffer
                                   intoStream:"input_video"
                                   packetType:MPPPacketTypePixelBuffer
                                    timestamp:mediapipe::Timestamp(0)]);

  // Shut down the graph.
  success = [mediapipeGraph closeAllInputStreamsWithError:&error];
  XCTAssertTrue(success, @"%@", error.localizedDescription);
  success = [mediapipeGraph waitUntilDoneWithError:&error];
  XCTAssertTrue(success, @"%@", error.localizedDescription);

  // Check output.
  XCTAssertTrue(_outputPixelBuffer != nullptr);
  [self savePixelBufferToSponge:*_outputPixelBuffer
                    withSubpath:@"sobel.png"];
  XCTAssertTrue([self pixelBuffer:*_outputPixelBuffer
                        isCloseTo:*_referencePixelBuffer
               maxLocalDifference:5
             maxAverageDifference:FLT_MAX]);
}
@end
