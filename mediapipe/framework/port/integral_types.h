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

#ifndef MEDIAPIPE_PORT_INTEGRAL_TYPES_H_
#define MEDIAPIPE_PORT_INTEGRAL_TYPES_H_

// TODO: <sys/types.h> is not portable C. Take a close look at this when we add
// mobile support.
#include <sys/types.h>

#include <cstdint>

typedef signed char schar;
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef unsigned long uword_t;

#define GG_LONGLONG(x) x##LL
#define GG_ULONGLONG(x) x##ULL
#define GG_LL_FORMAT "ll"  // As in "%lld". Note that "q" is poor form also.
#define GG_LL_FORMAT_W L"ll"

// Add namespace here to avoid conflict with other libraries.
namespace mediapipe {

typedef signed int char32;

const uint8 kuint8max{0xFF};
const uint16 kuint16max{0xFFFF};
const uint32 kuint32max{0xFFFFFFFF};
const uint64 kuint64max{GG_ULONGLONG(0xFFFFFFFFFFFFFFFF)};
const int8 kint8min{~0x7F};
const int8 kint8max{0x7F};
const int16 kint16min{~0x7FFF};
const int16 kint16max{0x7FFF};
const int32 kint32min{~0x7FFFFFFF};
const int32 kint32max{0x7FFFFFFF};
const int64 kint64min{GG_LONGLONG(~0x7FFFFFFFFFFFFFFF)};
const int64 kint64max{GG_LONGLONG(0x7FFFFFFFFFFFFFFF)};

typedef uint64 Fprint;
static const Fprint kIllegalFprint = 0;
static const Fprint kMaxFprint = GG_ULONGLONG(0xFFFFFFFFFFFFFFFF);

}  // namespace mediapipe

#endif  // MEDIAPIPE_PORT_INTEGRAL_TYPES_H_
