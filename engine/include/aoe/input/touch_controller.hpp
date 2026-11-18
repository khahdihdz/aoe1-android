#pragma once
#include <SDL.h>
namespace aoe {
struct CameraState { float x=0.0F, y=0.0F, zoom=1.0F; int selected_tile=-1; };
class TouchController {
public:
  void handle(const SDL_Event& event, CameraState& camera, int width, int height);
private:
  bool dragging_=false;
  float last_x_=0.0F, last_y_=0.0F;
};
}
