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

#include <utility>

#include "absl/memory/memory.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/port/status_builder.h"
#include "mediapipe/gpu/gl_context.h"
#include "mediapipe/gpu/gl_context_internal.h"

#if HAS_EAGL

#if !__has_feature(objc_arc)
#error This file must be built with ARC.
#endif

namespace mediapipe {

GlContext::StatusOrGlContext GlContext::Create(std::nullptr_t nullp,
                                               bool create_thread) {
  return Create(static_cast<EAGLSharegroup*>(nil), create_thread);
}

GlContext::StatusOrGlContext GlContext::Create(const GlContext& share_context,
                                               bool create_thread) {
  return Create(share_context.context_.sharegroup, create_thread);
}

GlContext::StatusOrGlContext GlContext::Create(EAGLContext* share_context,
                                               bool create_thread) {
  return Create(share_context.sharegroup, create_thread);
}

GlContext::StatusOrGlContext GlContext::Create(EAGLSharegroup* sharegroup,
                                               bool create_thread) {
  std::shared_ptr<GlContext> context(new GlContext());
  MP_RETURN_IF_ERROR(context->CreateContext(sharegroup));
  MP_RETURN_IF_ERROR(context->FinishInitialization(create_thread));
  return std::move(context);
}

absl::Status GlContext::CreateContext(EAGLSharegroup* sharegroup) {
  context_ = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3
                                   sharegroup:sharegroup];
  if (context_) {
    gl_major_version_ = 3;
  } else {
    context_ = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2
                                     sharegroup:sharegroup];
    gl_major_version_ = 2;
  }
  RET_CHECK(context_) << "Could not create an EAGLContext";

  CVOpenGLESTextureCacheRef cache;
  CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL,
                                              context_, NULL, &cache);
  RET_CHECK_EQ(err, kCVReturnSuccess)
      << "Error at CVOpenGLESTextureCacheCreate";
  texture_cache_.adopt(cache);

  return absl::OkStatus();
}

void GlContext::DestroyContext() {
  if (*texture_cache_) {
    // The texture cache must be flushed on tear down, otherwise we potentially
    // leak pixel buffers whose textures have pending GL operations after the
    // CVOpenGLESTextureRef is released in GlTexture::Release.
    CVOpenGLESTextureCacheFlush(*texture_cache_, 0);
  }
}

GlContext::ContextBinding GlContext::ThisContextBindingPlatform() {
  GlContext::ContextBinding result;
  result.context = context_;
  return result;
}

void GlContext::GetCurrentContextBinding(GlContext::ContextBinding* binding) {
  binding->context = [EAGLContext currentContext];
}

absl::Status GlContext::SetCurrentContextBinding(
    const ContextBinding& new_binding) {
  BOOL success = [EAGLContext setCurrentContext:new_binding.context];
  RET_CHECK(success) << "Cannot set OpenGL context";
  return absl::OkStatus();
}

bool GlContext::HasContext() const { return context_ != nil; }

bool GlContext::IsCurrent() const {
  return HasContext() && ([EAGLContext currentContext] == context_);
}

}  // namespace mediapipe

#endif  // HAS_EAGL
