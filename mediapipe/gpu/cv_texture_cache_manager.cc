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

#include "mediapipe/gpu/cv_texture_cache_manager.h"

#include "absl/log/absl_check.h"
#include "mediapipe/framework/port/logging.h"

namespace mediapipe {

void CvTextureCacheManager::FlushTextureCaches() {
  absl::MutexLock lock(&mutex_);
  for (const auto& cache : texture_caches_) {
#if TARGET_OS_OSX
    CVOpenGLTextureCacheFlush(*cache, 0);
#else
    CVOpenGLESTextureCacheFlush(*cache, 0);
#endif  // TARGET_OS_OSX
  }
}

void CvTextureCacheManager::RegisterTextureCache(CVTextureCacheType cache) {
  absl::MutexLock lock(&mutex_);

  ABSL_CHECK(std::find(texture_caches_.begin(), texture_caches_.end(), cache) ==
             texture_caches_.end())
      << "Attempting to register a texture cache twice";
  texture_caches_.emplace_back(cache);
}

void CvTextureCacheManager::UnregisterTextureCache(CVTextureCacheType cache) {
  absl::MutexLock lock(&mutex_);

  auto it = std::find(texture_caches_.begin(), texture_caches_.end(), cache);
  ABSL_CHECK(it != texture_caches_.end())
      << "Attempting to unregister an unknown texture cache";
  texture_caches_.erase(it);
}

CvTextureCacheManager::~CvTextureCacheManager() {
  ABSL_CHECK_EQ(texture_caches_.size(), 0)
      << "Failed to unregister texture caches before deleting manager";
}

}  // namespace mediapipe
