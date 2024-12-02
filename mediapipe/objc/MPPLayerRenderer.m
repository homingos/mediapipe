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

#import "MPPLayerRenderer.h"

@implementation MPPLayerRenderer {
  /// Used for rendering to a CAEAGLLayer.
  MPPGLViewRenderer *_glRenderer;

  /// Used for rendering.
  GLuint framebuffer_;
  GLuint renderbuffer_;
}

- (instancetype)init {
  self = [super init];
  if (self) {
    _layer = [[CAEAGLLayer alloc] init];
    // The default drawable properties are ok.
    _layer.opaque = YES;
    // Synchronizes presentation with CoreAnimation transactions.
    // Avoids desync between GL and CA updates.
    // TODO: should this be on by default? It's not on in GLKView,
    // but if we add support for AVSampleBufferDisplayLayer we may want to
    // make sure the behavior is similar.
    if ([_layer respondsToSelector:@selector(setPresentsWithTransaction:)]) {
      _layer.presentsWithTransaction = YES;
    }
    _layer.contentsScale = [[UIScreen mainScreen] scale];
    _glRenderer = [[MPPGLViewRenderer alloc] init];
  }
  return self;
}

- (void)dealloc {
}

- (void)setupFrameBuffer {
  [EAGLContext setCurrentContext:_glRenderer.glContext];
  glDisable(GL_DEPTH_TEST);
  glGenFramebuffers(1, &framebuffer_);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
  glGenRenderbuffers(1, &renderbuffer_);
  glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer_);
  BOOL success __unused = [_glRenderer.glContext renderbufferStorage:GL_RENDERBUFFER
                                                        fromDrawable:_layer];
  NSAssert(success, @"could not create renderbuffer storage for layer with bounds %@",
           NSStringFromCGRect(_layer.bounds));
  GLenum status __unused = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  NSAssert(status == GL_FRAMEBUFFER_COMPLETE,
          @"failed to make complete framebuffer object %x", status);
}

- (void)renderPixelBuffer:(CVPixelBufferRef)pixelBuffer {
  if (!framebuffer_) {
    [self setupFrameBuffer];
  }
  GLfloat drawWidth = _layer.bounds.size.width * _layer.contentsScale;
  GLfloat drawHeight = _layer.bounds.size.height * _layer.contentsScale;
  [EAGLContext setCurrentContext:_glRenderer.glContext];
  glViewport(0, 0, drawWidth, drawHeight);
  [_glRenderer drawPixelBuffer:pixelBuffer width:drawWidth height:drawHeight];
  BOOL success = [_glRenderer.glContext presentRenderbuffer:GL_RENDERBUFFER];
  if (!success) NSLog(@"presentRenderbuffer failed");
}

- (MPPFrameRotation)frameRotationMode {
  return _glRenderer.frameRotationMode;
}

- (void)setFrameRotationMode:(MPPFrameRotation)frameRotationMode {
  _glRenderer.frameRotationMode = frameRotationMode;
}

- (MPPFrameScaleMode)frameScaleMode {
  return _glRenderer.frameScaleMode;
}

- (void)setFrameScaleMode:(MPPFrameScaleMode)frameScaleMode {
  _glRenderer.frameScaleMode = frameScaleMode;
}

- (BOOL)mirrored {
  return _glRenderer.mirrored;
}

- (void)setMirrored:(BOOL)mirrored {
  _glRenderer.mirrored = mirrored;
}

@end
