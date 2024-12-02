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

#import "mediapipe/objc/CFHolder.h"

#include <utility>

@interface CFHolderTests : XCTestCase {
  UInt8 _bytes[4];
  CFDataRef _data;
}
@end

@implementation CFHolderTests

- (void)setUp {
  _data = CFDataCreate(NULL, _bytes, sizeof(_bytes));
}

- (void)tearDown {
  CFRelease(_data);
}

- (void)testCreateAndDestroy {
  XCTAssertEqual(CFGetRetainCount(_data), 1);
  {
    CFHolder<CFDataRef> holder(_data);
    XCTAssertEqual(CFGetRetainCount(_data), 2);
  }
  XCTAssertEqual(CFGetRetainCount(_data), 1);
}

- (void)testDereference {
  CFHolder<CFDataRef> holder(_data);
  XCTAssertEqual(*holder, _data);
}

- (void)testCopy {
  XCTAssertEqual(CFGetRetainCount(_data), 1);
  {
    CFHolder<CFDataRef> holder(_data);
    XCTAssertEqual(CFGetRetainCount(_data), 2);
    {
      CFHolder<CFDataRef> holder2(holder);
      XCTAssertEqual(CFGetRetainCount(_data), 3);
      {
        CFHolder<CFDataRef> holder3 = holder;
        XCTAssertEqual(CFGetRetainCount(_data), 4);
      }
      XCTAssertEqual(CFGetRetainCount(_data), 3);
    }
    XCTAssertEqual(CFGetRetainCount(_data), 2);
  }
  XCTAssertEqual(CFGetRetainCount(_data), 1);
}

- (void)testOverwriteWithNull {
  XCTAssertEqual(CFGetRetainCount(_data), 1);
  {
    // Copy assignment.
    CFHolder<CFDataRef> holder(_data);
    XCTAssertEqual(CFGetRetainCount(_data), 2);
    CFHolder<CFDataRef> holder2;
    holder = holder2;
    XCTAssertEqual(CFGetRetainCount(_data), 1);
  }
  {
    // Move assignment.
    CFHolder<CFDataRef> holder(_data);
    XCTAssertEqual(CFGetRetainCount(_data), 2);
    holder = CFHolder<CFDataRef>();
    XCTAssertEqual(CFGetRetainCount(_data), 1);
  }
}

- (void)testOverwriteWithOther {
  CFDataRef data2 = CFDataCreate(NULL, _bytes, sizeof(_bytes));
  XCTAssertEqual(CFGetRetainCount(_data), 1);
  XCTAssertEqual(CFGetRetainCount(data2), 1);
  {
    // Copy assignment.
    CFHolder<CFDataRef> holder(_data);
    XCTAssertEqual(CFGetRetainCount(_data), 2);
    CFHolder<CFDataRef> holder2(data2);
    XCTAssertEqual(CFGetRetainCount(data2), 2);
    holder = holder2;
    XCTAssertEqual(CFGetRetainCount(_data), 1);
    XCTAssertEqual(CFGetRetainCount(data2), 3);
  }
  {
    // Move assignment.
    CFHolder<CFDataRef> holder(_data);
    XCTAssertEqual(CFGetRetainCount(_data), 2);
    holder = CFHolder<CFDataRef>(data2);
    XCTAssertEqual(CFGetRetainCount(_data), 1);
    XCTAssertEqual(CFGetRetainCount(data2), 2);
  }
  CFRelease(data2);
}

- (void)testCompare {
  CFDataRef data2 = CFDataCreate(NULL, _bytes, sizeof(_bytes));
  CFHolder<CFDataRef> holder(_data);
  CFHolder<CFDataRef> holdersame(_data);
  CFHolder<CFDataRef> holderother(data2);
  CFHolder<CFDataRef> empty;
  // Compare with other holder.
  XCTAssertEqual(holder, holder);
  XCTAssertEqual(holder, holdersame);
  XCTAssertNotEqual(holder, holderother);
  XCTAssertNotEqual(holder, empty);
  // Compare with held type.
  XCTAssertEqual(holder, _data);
  XCTAssertNotEqual(holder, data2);
  XCTAssertNotEqual(holder, nil);
  XCTAssertEqual(empty, nil);
  XCTAssertNotEqual(empty, _data);

  CFRelease(data2);
}

- (void)testReset {
  XCTAssertEqual(CFGetRetainCount(_data), 1);
  {
    CFHolder<CFDataRef> holder;
    holder.reset(_data);
    XCTAssertEqual(CFGetRetainCount(_data), 2);
    CFDataRef data2 = CFDataCreate(NULL, _bytes, sizeof(_bytes));
    holder.reset(data2);
    XCTAssertEqual(CFGetRetainCount(_data), 1);
    XCTAssertEqual(CFGetRetainCount(data2), 2);
    CFRelease(data2);
  }
  XCTAssertEqual(CFGetRetainCount(_data), 1);
}

- (void)testAdopt {
  CFRetain(_data);
  XCTAssertEqual(CFGetRetainCount(_data), 2);
  {
    CFHolder<CFDataRef> holder;
    holder.adopt(_data);
    XCTAssertEqual(CFGetRetainCount(_data), 2);
  }
  XCTAssertEqual(CFGetRetainCount(_data), 1);
}

- (void)testMove {
  XCTAssertEqual(CFGetRetainCount(_data), 1);
  {
    CFHolder<CFDataRef> holder(_data);
    XCTAssertEqual(CFGetRetainCount(_data), 2);
    CFHolder<CFDataRef> holder2(std::move(holder));
    XCTAssertEqual(CFGetRetainCount(_data), 2);
    CFHolder<CFDataRef> holder3(std::move(holder2));
    XCTAssertEqual(CFGetRetainCount(_data), 2);
  }
  XCTAssertEqual(CFGetRetainCount(_data), 1);
}

@end
