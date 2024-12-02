
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

#include "absl/memory/memory.h"
#include "mediapipe/framework/profiler/graph_profiler.h"
#include "mediapipe/framework/profiler/profiler_resource_util.h"
#include "mediapipe/objc/MPPGraph.h"
#include "mediapipe/objc/MPPGraphTestBase.h"

static NSString* const kTraceFilename = @"mediapipe_trace_0.binarypb";

static const char* kOutputStream = "counter";

@interface GraphProfilerTest : MPPGraphTestBase
@end

@implementation GraphProfilerTest

- (void)mediapipeGraph:(MPPGraph*)graph
     didOutputPacket:(const mediapipe::Packet&)packet
          fromStream:(const std::string&)streamName {
  XCTAssertTrue(streamName == kOutputStream);
  NSLog(@"Received counter packet.");
}

- (void)testDefaultTraceLogPathValueIsSet {
  mediapipe::CalculatorGraphConfig graphConfig;
  mediapipe::CalculatorGraphConfig::Node* node = graphConfig.add_node();
  node->set_calculator("SimpleCalculator");
  node->add_output_stream(kOutputStream);

  mediapipe::ProfilerConfig* profilerConfig = graphConfig.mutable_profiler_config();
  profilerConfig->set_trace_enabled(true);
  profilerConfig->set_enable_profiler(true);
  profilerConfig->set_trace_log_disabled(false);

  MPPGraph* graph = [[MPPGraph alloc] initWithGraphConfig:graphConfig];
  [graph addFrameOutputStream:kOutputStream outputPacketType:MPPPacketTypeRaw];
  graph.delegate = self;

  NSError* error;
  BOOL success = [graph startWithError:&error];
  XCTAssertTrue(success, @"%@", error.localizedDescription);

  // Shut down the graph.
  success = [graph waitUntilDoneWithError:&error];
  XCTAssertTrue(success, @"%@", error.localizedDescription);

  absl::StatusOr<string> getTraceLogDir = mediapipe::GetDefaultTraceLogDirectory();
  XCTAssertTrue(getTraceLogDir.ok(), "GetDefaultTraceLogDirectory failed.");

  NSString* directoryPath = [NSString stringWithCString:(*getTraceLogDir).c_str()
                                               encoding:[NSString defaultCStringEncoding]];
  NSString* traceLogPath = [directoryPath stringByAppendingPathComponent:kTraceFilename];
  BOOL traceLogFileExists = [[NSFileManager defaultManager] fileExistsAtPath:traceLogPath];
  XCTAssertTrue(traceLogFileExists, @"Trace log file not found at path: %@", traceLogPath);
}

@end
