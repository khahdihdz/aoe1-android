#include "aoe/render/gl_renderer.hpp"
#include <GLES3/gl3.h>
#include <string>
namespace aoe {
namespace {
GLuint shader(GLenum type,const char* source) {
  GLuint id=glCreateShader(type); glShaderSource(id,1,&source,nullptr); glCompileShader(id);
  GLint ok=GL_FALSE; glGetShaderiv(id,GL_COMPILE_STATUS,&ok);
  if(ok!=GL_TRUE) { GLint n=0; glGetShaderiv(id,GL_INFO_LOG_LENGTH,&n); std::string log(static_cast<std::size_t>(n),'\\0'); glGetShaderInfoLog(id,n,nullptr,log.data()); SDL_Log("GL shader: %s",log.c_str()); glDeleteShader(id); return 0; }
  return id;
}
}
bool GlRenderer::initialize(SDL_Window*) {
  const char* vs=R"(#version 300 es
  layout(location=0) in vec2 aPosition; uniform vec2 uOffset; uniform float uZoom;
  void main(){ gl_Position=vec4((aPosition+uOffset)*uZoom,0.0,1.0); })";
  const char* fs=R"(#version 300 es
  precision mediump float; uniform vec4 uColor; out vec4 fragColor;
  void main(){ fragColor=uColor; })";
  GLuint v=shader(GL_VERTEX_SHADER,vs), f=shader(GL_FRAGMENT_SHADER,fs); if(!v||!f) return false;
  program_=glCreateProgram(); glAttachShader(program_,v); glAttachShader(program_,f); glLinkProgram(program_); glDeleteShader(v); glDeleteShader(f);
  GLint linked=GL_FALSE; glGetProgramiv(program_,GL_LINK_STATUS,&linked); if(linked!=GL_TRUE){ SDL_Log("GL link failed"); return false; }
  color_location_=glGetUniformLocation(program_,"uColor");
  zoom_location_=glGetUniformLocation(program_,"uZoom");
  offset_location_=glGetUniformLocation(program_,"uOffset");
  return true;
}
void GlRenderer::resize(int w,int h){ width_=w>0?w:1; height_=h>0?h:1; glViewport(0,0,width_,height_); }
void GlRenderer::render(float cx,float cy,float zoom,int selected) {
  glViewport(0,0,width_,height_); glClearColor(0.055F,0.075F,0.065F,1.0F); glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(program_); glUniform1f(zoom_location_,zoom); glUniform2f(offset_location_,cx,cy);
  constexpr int cols=12, rows=9; const float tw=0.145F, th=0.105F;
  glEnableVertexAttribArray(0);
  for(int y=0;y<rows;++y) for(int x=0;x<cols;++x) {
    const float px=(x-y)*tw*0.5F, py=(x+y)*th*0.5F-0.42F;
    const float p[]={px,py-th*0.5F,px+tw*0.5F,py,px,py+th*0.5F,px-tw*0.5F,py,px,py-th*0.5F,px,py+th*0.5F};
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,p);
    if(y*cols+x==selected) glUniform4f(color_location_,0.75F,0.67F,0.30F,1.0F);
    else if((x+y)%3==0) glUniform4f(color_location_,0.22F,0.43F,0.25F,1.0F);
    else glUniform4f(color_location_,0.30F,0.50F,0.29F,1.0F);
    glDrawArrays(GL_TRIANGLES,0,6);
  }
  glDisableVertexAttribArray(0);
}
void GlRenderer::shutdown(){ if(program_){glDeleteProgram(program_);program_=0;} }
}
