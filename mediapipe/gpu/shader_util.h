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

#ifndef MEDIAPIPE_GPU_SHADER_UTIL_H_
#define MEDIAPIPE_GPU_SHADER_UTIL_H_

#include <string>
#include <unordered_map>

#include "mediapipe/gpu/gl_base.h"

namespace mediapipe {

// TODO: Remove the C-style helpers.
// Compiles a GLSL shader, logs errors, returns the compile status
// (GL_TRUE for success, GL_FALSE for failure).
GLint GlhCompileShader(GLenum target, const GLchar* source, GLuint* shader,
                       bool force_log_errors = false);

// Links a GLSL program, logs errors, returns the link status
// (GL_TRUE for success, GL_FALSE for failure).
GLint GlhLinkProgram(GLuint program, bool force_log_errors = false);

// Validates a GLSL program, logs errors, returns the validate status
// (GL_TRUE for success, GL_FALSE for failure).
GLint GlhValidateProgram(GLuint program);

// Creates a GLSL program by compiling and linking the provided shaders.
// Also obtains the locations of the requested attributes.
// Return GL_TRUE for success, GL_FALSE for failure.
GLint GlhCreateProgram(const GLchar* vert_src, const GLchar* frag_src,
                       GLsizei attr_count, const GLchar* const* attr_names,
                       const GLint* attr_locations, GLuint* program,
                       bool force_log_errors = false);

// Compiles a shader specified by shader_source. Returns true on success.
bool CompileShader(GLenum shader_type, const std::string& shader_source,
                   GLuint* shader);

// Creates a shader program using the supplied vertex shader, fragment shader
// and attributes and stores in program. Returns true on success.
bool CreateShaderProgram(
    GLuint vertex_shader, GLuint fragment_shader,
    const std::unordered_map<GLuint, std::string>& attributes, GLuint* program);
}  // namespace mediapipe

#endif  // MEDIAPIPE_GPU_SHADER_UTIL_H_
