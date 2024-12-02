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

#include "mediapipe/framework/tool/executor_util.h"

#include "mediapipe/framework/port/gmock.h"
#include "mediapipe/framework/port/gtest.h"
#include "mediapipe/framework/port/parse_text_proto.h"

namespace mediapipe {

TEST(GraphTest, MinimumDefaultExecutorStackSizeExistingConfigSizeUnspecified) {
  CalculatorGraphConfig config =
      ParseTextProtoOrDie<CalculatorGraphConfig>(R"pb(
        executor {
          options {
            [mediapipe.ThreadPoolExecutorOptions.ext] { num_threads: 2 }
          }
        }
      )pb");
  CalculatorGraphConfig expected_config =
      ParseTextProtoOrDie<CalculatorGraphConfig>(R"pb(
        executor {
          options {
            [mediapipe.ThreadPoolExecutorOptions.ext] {
              num_threads: 2
              stack_size: 131072
            }
          }
        }
      )pb");
  tool::EnsureMinimumDefaultExecutorStackSize(131072, &config);
  EXPECT_THAT(config, EqualsProto(expected_config));
}

TEST(GraphTest, MinimumDefaultExecutorStackSizeExistingConfigSizeTooSmall) {
  CalculatorGraphConfig config =
      ParseTextProtoOrDie<CalculatorGraphConfig>(R"pb(
        executor {
          options {
            [mediapipe.ThreadPoolExecutorOptions.ext] {
              num_threads: 2
              stack_size: 65536
            }
          }
        }
      )pb");
  CalculatorGraphConfig expected_config =
      ParseTextProtoOrDie<CalculatorGraphConfig>(R"pb(
        executor {
          options {
            [mediapipe.ThreadPoolExecutorOptions.ext] {
              num_threads: 2
              stack_size: 131072
            }
          }
        }
      )pb");
  tool::EnsureMinimumDefaultExecutorStackSize(131072, &config);
  EXPECT_THAT(config, EqualsProto(expected_config));
}

TEST(GraphTest, MinimumDefaultExecutorStackSizeExistingConfigSizeLargeEnough) {
  CalculatorGraphConfig config =
      ParseTextProtoOrDie<CalculatorGraphConfig>(R"pb(
        executor {
          options {
            [mediapipe.ThreadPoolExecutorOptions.ext] {
              num_threads: 2
              stack_size: 262144
            }
          }
        }
      )pb");
  CalculatorGraphConfig expected_config =
      ParseTextProtoOrDie<CalculatorGraphConfig>(R"pb(
        executor {
          options {
            [mediapipe.ThreadPoolExecutorOptions.ext] {
              num_threads: 2
              stack_size: 262144
            }
          }
        }
      )pb");
  tool::EnsureMinimumDefaultExecutorStackSize(131072, &config);
  EXPECT_THAT(config, EqualsProto(expected_config));
}

TEST(GraphTest, MinimumDefaultExecutorStackSizeNumThreads) {
  CalculatorGraphConfig config =
      ParseTextProtoOrDie<CalculatorGraphConfig>(R"pb(
        num_threads: 1
      )pb");
  CalculatorGraphConfig expected_config =
      ParseTextProtoOrDie<CalculatorGraphConfig>(R"pb(
        executor {
          options {
            [mediapipe.ThreadPoolExecutorOptions.ext] {
              num_threads: 1
              stack_size: 131072
            }
          }
        }
      )pb");
  tool::EnsureMinimumDefaultExecutorStackSize(131072, &config);
  EXPECT_THAT(config, EqualsProto(expected_config));
}

}  // namespace mediapipe
