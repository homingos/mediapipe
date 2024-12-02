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

#include <string>

#include "mediapipe/framework/mediapipe_options.pb.h"
#include "mediapipe/framework/thread_pool_executor.pb.h"

namespace mediapipe {
namespace tool {

void EnsureMinimumDefaultExecutorStackSize(const int32_t min_stack_size,
                                           CalculatorGraphConfig* config) {
  mediapipe::ExecutorConfig* default_executor_config = nullptr;
  for (mediapipe::ExecutorConfig& executor_config :
       *config->mutable_executor()) {
    if (executor_config.name().empty()) {
      default_executor_config = &executor_config;
      break;
    }
  }
  if (!default_executor_config) {
    default_executor_config = config->add_executor();
    if (config->num_threads()) {
      default_executor_config->mutable_options()
          ->MutableExtension(mediapipe::ThreadPoolExecutorOptions::ext)
          ->set_num_threads(config->num_threads());
      config->clear_num_threads();
    }
  }
  if (default_executor_config->type().empty() ||
      default_executor_config->type() == "ThreadPoolExecutor") {
    mediapipe::ThreadPoolExecutorOptions* extension =
        default_executor_config->mutable_options()->MutableExtension(
            mediapipe::ThreadPoolExecutorOptions::ext);
    if (extension->stack_size() < min_stack_size) {
      extension->set_stack_size(min_stack_size);
    }
  }
}

}  // namespace tool
}  // namespace mediapipe
