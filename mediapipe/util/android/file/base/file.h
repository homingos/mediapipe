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

#ifndef MEDIAPIPE_ANDROID_FILE_BASE_FILE_H_
#define MEDIAPIPE_ANDROID_FILE_BASE_FILE_H_

#include <sys/stat.h>
#include <sys/types.h>

#include <string>

namespace mediapipe {
namespace file {

class Options;

bool IsAbsolutePath(const std::string& path);
Options CreationMode(mode_t permissions);

class Options {
 public:
  Options() = default;

  void set_permissions(mode_t permissions) { permissions_ = permissions; }

  mode_t permissions() const { return permissions_; }

 private:
  mode_t permissions_ = S_IRWXU | S_IRWXG | S_IRWXO;
};

inline Options Defaults() { return Options(); }

}  // namespace file

class File {
 public:
  // Return the "basename" for "fname".  I.e. strip out everything
  // up to and including the last "/" in the name.
  static std::string Basename(const std::string& fname);

  static std::string StripBasename(const std::string& fname);

  static bool IsLocalFile(const std::string& fname);

  static bool Exists(const char* name);
  static bool Exists(const std::string& name) { return Exists(name.c_str()); }

  static const std::string CanonicalizeFileName(const char* fname);
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_ANDROID_FILE_BASE_FILE_H_
