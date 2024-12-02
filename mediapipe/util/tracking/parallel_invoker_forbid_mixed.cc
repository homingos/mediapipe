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
//
// Guard to ensure clients do not link against both,
// single and parallel version.
#ifdef PARALLEL_INVOKER_ACTIVE
int LinkageAgainstBothSingleAndParallelTrackingVersionsDetected() { return 0; }

#endif  // PARALLEL_INVOKER_ACTIVE

#ifdef PARALLEL_INVOKER_INACTIVE
int LinkageAgainstBothSingleAndParallelTrackingVersionsDetected() { return 1; }
#endif  // PARALLEL_INVOKER_INACTIVE
