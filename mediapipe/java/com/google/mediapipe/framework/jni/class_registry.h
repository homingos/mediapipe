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

#ifndef JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_CLASS_REGISTRY_H_
#define JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_CLASS_REGISTRY_H_

#include <jni.h>

#include <string>

#include "absl/container/node_hash_map.h"

namespace mediapipe {
namespace android {

// ClassRegistry maintains the correct names of Java classes and methods and
// should be queried before any calls to FindClass() or GetMethodID().
class ClassRegistry {
 public:
  static ClassRegistry& GetInstance();
  void InstallRenamingMap(
      absl::node_hash_map<std::string, std::string> renaming_map);
  std::string GetClassName(std::string cls);
  std::string GetMethodName(std::string cls, std::string method);
  std::string GetFieldName(std::string cls, std::string field);

  // TODO: Just have the prefix instead of all these constants.
  static constexpr char const* kAndroidAssetUtilClassName =
      "com/google/mediapipe/framework/AndroidAssetUtil";
  static constexpr char const* kAndroidPacketCreatorClassName =
      "com/google/mediapipe/framework/AndroidPacketCreator";
  static constexpr char const* kCompatClassName =
      "com/google/mediapipe/framework/Compat";
  static constexpr char const* kGraphClassName =
      "com/google/mediapipe/framework/Graph";
  static constexpr char const* kGraphProfilerClassName =
      "com/google/mediapipe/framework/GraphProfiler";
  static constexpr char const* kPacketClassName =
      "com/google/mediapipe/framework/Packet";
  static constexpr char const* kMediaPipeExceptionClassName =
      "com/google/mediapipe/framework/MediaPipeException";
  static constexpr char const* kPacketCallbackClassName =
      "com/google/mediapipe/framework/PacketCallback";
  static constexpr char const* kPacketListCallbackClassName =
      "com/google/mediapipe/framework/PacketListCallback";
  static constexpr char const* kPacketCreatorClassName =
      "com/google/mediapipe/framework/PacketCreator";
  static constexpr char const* kPacketGetterClassName =
      "com/google/mediapipe/framework/PacketGetter";
  static constexpr char const* kPacketWithHeaderCallbackClassName =
      "com/google/mediapipe/framework/PacketWithHeaderCallback";
  static constexpr char const* kProtoUtilSerializedMessageClassName =
      "com/google/mediapipe/framework/ProtoUtil$SerializedMessage";

 private:
  ClassRegistry();
  absl::node_hash_map<std::string, std::string> renaming_map_;
};

}  // namespace android
}  // namespace mediapipe

#endif  // JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_CLASS_REGISTRY_H_
