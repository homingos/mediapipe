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
// Defines DeletingFile.
#include "mediapipe/framework/formats/deleting_file.h"

#include <stdio.h>

#include <utility>

#include "absl/log/absl_log.h"

namespace mediapipe {

DeletingFile::DeletingFile(DeletingFile&& other)
    : path_(std::move(other.path_)),
      delete_on_destruction_(other.delete_on_destruction_) {
  other.delete_on_destruction_ = false;
}

DeletingFile& DeletingFile::operator=(DeletingFile&& other) {
  path_ = std::move(other.path_);
  delete_on_destruction_ = other.delete_on_destruction_;
  other.delete_on_destruction_ = false;
  return *this;
}

DeletingFile::DeletingFile(const std::string& path, bool delete_on_destruction)
    : path_(path), delete_on_destruction_(delete_on_destruction) {}

DeletingFile::~DeletingFile() {
  if (delete_on_destruction_) {
    if (remove(path_.c_str()) != 0) {
      ABSL_LOG(ERROR) << "Unable to delete file: " << path_;
    }
  }
}

const std::string& DeletingFile::Path() const { return path_; }

}  // namespace mediapipe
