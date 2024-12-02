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
// Declares DeletingFile.

#ifndef MEDIAPIPE_FRAMEWORK_FORMATS_DELETING_FILE_H_
#define MEDIAPIPE_FRAMEWORK_FORMATS_DELETING_FILE_H_

#include <string>

namespace mediapipe {

// A DeletingFile conveys the path to a file and takes care of cleanup
// (generally deletion of a file if it is a local temporary).
class DeletingFile {
 public:
  DeletingFile(const DeletingFile&) = delete;
  DeletingFile& operator=(const DeletingFile&) = delete;

  // DeletingFile is movable. The moved-from object remains in valid but
  // unspecified state and will not perform any operations on destruction.
  DeletingFile(DeletingFile&& other);
  DeletingFile& operator=(DeletingFile&& other);

  // Provide the path to the file and whether the file should be deleted
  // when this object is destroyed.
  DeletingFile(const std::string& path, bool delete_on_destruction);

  // Takes care of cleaning up the file (deletes it if
  // delete_on_destruction was true on construction, otherwise leaves
  // it alone).
  virtual ~DeletingFile();

  // Return the path to the file.
  const std::string& Path() const;

 private:
  std::string path_;
  bool delete_on_destruction_;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_FORMATS_DELETING_FILE_H_
