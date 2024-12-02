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

#include <memory>
#include <utility>

#include "absl/memory/memory.h"
#include "mediapipe/framework/executor.h"
#include "mediapipe/framework/port/gmock.h"
#include "mediapipe/framework/port/gtest.h"
#include "mediapipe/framework/port/threadpool.h"

// IMPORTANT: DO NOT add "namespace mediapipe" to this file.
// Leave this file outside the mediapipe namespace to emulate how MediaPipe
// clients implement and use a mediapipe::Executor subclass.
namespace {

// NOTE: If we need to update this class, that means there is a
// backward-incompatible change in the MediaPipe API and MediaPipe clients also
// need to update their mediapipe::Executor subclasses.
class MyExecutor : public mediapipe::Executor {
 public:
  MyExecutor();
  ~MyExecutor() override;

  // To verify a mediapipe::Executor subclass outside the mediapipe namespace
  // can override any method, override every method in the mediapipe::Executor
  // interface.
  void AddTask(mediapipe::TaskQueue* task_queue) override;
  void Schedule(std::function<void()> task) override;

 private:
  std::unique_ptr<mediapipe::ThreadPool> thread_pool_;
};

MyExecutor::MyExecutor() {
  thread_pool_ = absl::make_unique<mediapipe::ThreadPool>("my_executor", 1);
  thread_pool_->StartWorkers();
}

MyExecutor::~MyExecutor() { thread_pool_.reset(nullptr); }

void MyExecutor::AddTask(mediapipe::TaskQueue* task_queue) {
  thread_pool_->Schedule([task_queue] { task_queue->RunNextTask(); });
}

void MyExecutor::Schedule(std::function<void()> task) {
  thread_pool_->Schedule(std::move(task));
}

class NoOpTaskQueue : public mediapipe::TaskQueue {
 public:
  // Returns the number of times RunNextTask() was called.
  int call_count() const { return call_count_; }

 private:
  void RunNextTask() override { ++call_count_; }

  int call_count_ = 0;
};

TEST(ExecutorTest, MyExecutor) {
  NoOpTaskQueue task_queue;
  std::shared_ptr<MyExecutor> executor(new MyExecutor);
  int counter = 0;

  executor->AddTask(&task_queue);
  executor->Schedule([&counter] { ++counter; });
  executor->AddTask(&task_queue);
  executor->Schedule([&counter] { ++counter; });
  executor->AddTask(&task_queue);
  executor = nullptr;
  EXPECT_EQ(3, task_queue.call_count());
  EXPECT_EQ(2, counter);
}

}  // namespace
