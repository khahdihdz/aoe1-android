#include "aoe/platform/sdl_app.hpp"
#include "aoe/input/touch_controller.hpp"
#include "aoe/render/gl_renderer.hpp"
#include <SDL.h>
namespace aoe {
int run_sdl_app() {
  SDL_SetMainReady();
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
    SDL_Log("SDL_Init failed: %s", SDL_GetError()); return 1;
  }
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_Window* window = SDL_CreateWindow("AOE1 Android Foundation",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if (!window) { SDL_Log("Window failed: %s", SDL_GetError()); SDL_Quit(); return 2; }
  SDL_GLContext context = SDL_GL_CreateContext(window);
  if (!context) { SDL_Log("GL context failed: %s", SDL_GetError()); SDL_DestroyWindow(window); SDL_Quit(); return 3; }
  SDL_GL_SetSwapInterval(1);
  GlRenderer renderer;
  if (!renderer.initialize(window)) { SDL_GL_DeleteContext(context); SDL_DestroyWindow(window); SDL_Quit(); return 4; }
  int width=1, height=1; SDL_GL_GetDrawableSize(window,&width,&height); renderer.resize(width,height);
  CameraState camera; TouchController touch; bool running=true;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type==SDL_QUIT) running=false;
      if (event.type==SDL_WINDOWEVENT && (event.window.event==SDL_WINDOWEVENT_SIZE_CHANGED || event.window.event==SDL_WINDOWEVENT_RESIZED)) {
        SDL_GL_GetDrawableSize(window,&width,&height); renderer.resize(width,height);
      }
      touch.handle(event,camera,width,height);
    }
    renderer.render(camera.x,camera.y,camera.zoom,camera.selected_tile);
    SDL_GL_SwapWindow(window);
  }
  renderer.shutdown(); SDL_GL_DeleteContext(context); SDL_DestroyWindow(window); SDL_Quit(); return 0;
}
}
