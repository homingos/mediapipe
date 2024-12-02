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

#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/gpu/gl_simple_calculator.h"
#include "mediapipe/gpu/gl_simple_shaders.h"
#include "mediapipe/gpu/shader_util.h"

enum { ATTRIB_VERTEX, ATTRIB_TEXTURE_POSITION, NUM_ATTRIBUTES };

namespace mediapipe {

// Converts RGB images into luminance images, still stored in RGB format.
// See GlSimpleCalculatorBase for inputs, outputs and input side packets.
class LuminanceCalculator : public GlSimpleCalculator {
 public:
  absl::Status GlSetup() override;
  absl::Status GlRender(const GlTexture& src, const GlTexture& dst) override;
  absl::Status GlTeardown() override;

 private:
  GLuint program_ = 0;
  GLint frame_;
};
REGISTER_CALCULATOR(LuminanceCalculator);

absl::Status LuminanceCalculator::GlSetup() {
  // Load vertex and fragment shaders
  const GLint attr_location[NUM_ATTRIBUTES] = {
      ATTRIB_VERTEX,
      ATTRIB_TEXTURE_POSITION,
  };
  const GLchar* attr_name[NUM_ATTRIBUTES] = {
      "position",
      "texture_coordinate",
  };

  const GLchar* frag_src = GLES_VERSION_COMPAT
      R"(
#if __VERSION__ < 130
  #define in varying
#endif  // __VERSION__ < 130

#ifdef GL_ES
  #define fragColor gl_FragColor
  precision highp float;
#else
  #define lowp
  #define mediump
  #define highp
  #define texture2D texture
  out vec4 fragColor;
#endif  // defined(GL_ES)

  in vec2 sample_coordinate;
  uniform sampler2D video_frame;
  const highp vec3 W = vec3(0.2125, 0.7154, 0.0721);

  void main() {
    vec4 color = texture2D(video_frame, sample_coordinate);
    float luminance = dot(color.rgb, W);
    fragColor.rgb = vec3(luminance);
    fragColor.a = color.a;
  }

  )";

  // shader program
  GlhCreateProgram(kBasicVertexShader, frag_src, NUM_ATTRIBUTES,
                   (const GLchar**)&attr_name[0], attr_location, &program_);
  RET_CHECK(program_) << "Problem initializing the program.";
  frame_ = glGetUniformLocation(program_, "video_frame");
  return absl::OkStatus();
}

absl::Status LuminanceCalculator::GlRender(const GlTexture& src,
                                           const GlTexture& dst) {
  static const GLfloat square_vertices[] = {
      -1.0f, -1.0f,  // bottom left
      1.0f,  -1.0f,  // bottom right
      -1.0f, 1.0f,   // top left
      1.0f,  1.0f,   // top right
  };
  static const GLfloat texture_vertices[] = {
      0.0f, 0.0f,  // bottom left
      1.0f, 0.0f,  // bottom right
      0.0f, 1.0f,  // top left
      1.0f, 1.0f,  // top right
  };

  // program
  glUseProgram(program_);
  glUniform1i(frame_, 1);

  // vertex storage
  GLuint vbo[2];
  glGenBuffers(2, vbo);
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // vbo 0
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(GLfloat), square_vertices,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(ATTRIB_VERTEX);
  glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, nullptr);

  // vbo 1
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(GLfloat), texture_vertices,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(ATTRIB_TEXTURE_POSITION);
  glVertexAttribPointer(ATTRIB_TEXTURE_POSITION, 2, GL_FLOAT, 0, 0, nullptr);

  // draw
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  // cleanup
  glDisableVertexAttribArray(ATTRIB_VERTEX);
  glDisableVertexAttribArray(ATTRIB_TEXTURE_POSITION);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(2, vbo);

  return absl::OkStatus();
}

absl::Status LuminanceCalculator::GlTeardown() {
  if (program_) {
    glDeleteProgram(program_);
    program_ = 0;
  }
  return absl::OkStatus();
}

}  // namespace mediapipe
