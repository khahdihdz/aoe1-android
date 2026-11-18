#include "aoe/render/gl_renderer.hpp"
#include <GLES3/gl3.h>
#include <string>

namespace aoe {
namespace {
GLuint compile_shader(GLenum type, const char* source) {
  const GLuint id = glCreateShader(type);
  glShaderSource(id, 1, &source, nullptr);
  glCompileShader(id);
  GLint ok = GL_FALSE;
  glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
  if (ok != GL_TRUE) {
    GLint length = 0;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    std::string log(static_cast<std::size_t>(length), ' ');
    glGetShaderInfoLog(id, length, nullptr, log.data());
    SDL_Log("Shader compile error: %s", log.c_str());
    glDeleteShader(id);
    return 0;
  }
  return id;
}
}

bool GlRenderer::initialize(SDL_Window*) {
  const char* vertex = R"(#version 300 es
    layout(location=0) in vec2 aPosition;
    uniform vec2 uOffset;
    uniform float uZoom;
    void main() { gl_Position = vec4((aPosition + uOffset) * uZoom, 0.0, 1.0); }
  )";
  const char* fragment = R"(#version 300 es
    precision mediump float;
    uniform vec4 uColor;
    out vec4 fragColor;
    void main() { fragColor = uColor; }
  )";
  const GLuint vs = compile_shader(GL_VERTEX_SHADER, vertex);
  const GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragment);
  if (!vs || !fs) return false;

  program_ = glCreateProgram();
  glAttachShader(program_, vs);
  glAttachShader(program_, fs);
  glLinkProgram(program_);
  glDeleteShader(vs);
  glDeleteShader(fs);

  GLint linked = GL_FALSE;
  glGetProgramiv(program_, GL_LINK_STATUS, &linked);
  if (linked != GL_TRUE) {
    SDL_Log("OpenGL program link failed");
    return false;
  }

  glGenBuffers(1, &vertex_buffer_);
  color_location_ = glGetUniformLocation(program_, "uColor");
  zoom_location_ = glGetUniformLocation(program_, "uZoom");
  offset_location_ = glGetUniformLocation(program_, "uOffset");
  return vertex_buffer_ != 0;
}

void GlRenderer::resize(int width, int height) {
  width_ = width > 0 ? width : 1;
  height_ = height > 0 ? height : 1;
  glViewport(0, 0, width_, height_);
}

void GlRenderer::render(float camera_x, float camera_y, float zoom, int selected_tile) {
  glViewport(0, 0, width_, height_);
  glClearColor(0.055F, 0.075F, 0.065F, 1.0F);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(program_);
  glUniform1f(zoom_location_, zoom);
  glUniform2f(offset_location_, camera_x, camera_y);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
  glEnableVertexAttribArray(0);

  constexpr int columns = 12;
  constexpr int rows = 9;
  constexpr float tile_w = 0.145F;
  constexpr float tile_h = 0.105F;
  for (int y = 0; y < rows; ++y) {
    for (int x = 0; x < columns; ++x) {
      const float cx = (x - y) * tile_w * 0.5F;
      const float cy = (x + y) * tile_h * 0.5F - 0.42F;
      const float vertices[] = {
        cx, cy - tile_h * 0.5F,
        cx + tile_w * 0.5F, cy,
        cx, cy + tile_h * 0.5F,
        cx - tile_w * 0.5F, cy,
        cx, cy - tile_h * 0.5F,
        cx, cy + tile_h * 0.5F
      };
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
      if (y * columns + x == selected_tile) glUniform4f(color_location_, 0.75F, 0.67F, 0.30F, 1.0F);
      else if ((x + y) % 3 == 0) glUniform4f(color_location_, 0.22F, 0.43F, 0.25F, 1.0F);
      else glUniform4f(color_location_, 0.30F, 0.50F, 0.29F, 1.0F);
      glDrawArrays(GL_TRIANGLES, 0, 6);
    }
  }
  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GlRenderer::shutdown() {
  if (program_) { glDeleteProgram(program_); program_ = 0; }
  if (vertex_buffer_) { glDeleteBuffers(1, &vertex_buffer_); vertex_buffer_ = 0; }
}
}
