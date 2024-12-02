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

#include <memory>

#include "absl/memory/memory.h"
#include "mediapipe/framework/port/threadpool.h"

#import "mediapipe/gpu/gpu_shared_data_internal.h"
#import "mediapipe/gpu/metal_shared_resources.h"

@interface MPPMetalSharedResourcesTests : XCTestCase {
}
@end

@implementation MPPMetalSharedResourcesTests

// This test verifies that the internal Objective-C object is correctly
// released when the C++ wrapper is released.
- (void)testCorrectlyReleased {
  __weak id metalRes = nil;
  std::weak_ptr<mediapipe::GpuResources> weakGpuRes;
  @autoreleasepool {
    auto maybeGpuRes = mediapipe::GpuResources::Create();
    XCTAssertTrue(maybeGpuRes.ok());
    weakGpuRes = *maybeGpuRes;
    metalRes = (**maybeGpuRes).metal_shared().resources();
    XCTAssertNotEqual(weakGpuRes.lock(), nullptr);
    XCTAssertNotNil(metalRes);
  }
  XCTAssertEqual(weakGpuRes.lock(), nullptr);
  XCTAssertNil(metalRes);
}

@end
