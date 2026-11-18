#pragma once
#include <SDL.h>
namespace aoe {
class GlRenderer {
public:
  bool initialize(SDL_Window* window);
  void resize(int width, int height);
  void render(float camera_x, float camera_y, float zoom, int selected_tile);
  void shutdown();
private:
  unsigned int program_ = 0;
  unsigned int vertex_buffer_ = 0;
  int width_ = 1, height_ = 1;
  int color_location_ = -1, zoom_location_ = -1, offset_location_ = -1;
};
}
