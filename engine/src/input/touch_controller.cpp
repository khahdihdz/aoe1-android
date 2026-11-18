#include "aoe/input/touch_controller.hpp"
#include <algorithm>
namespace aoe {
void TouchController::handle(const SDL_Event& event, CameraState& camera, int width, int height) {
  (void)width; (void)height;
  if (event.type==SDL_FINGERDOWN) {
    dragging_=true; last_x_=event.tfinger.x; last_y_=event.tfinger.y;
  } else if (event.type==SDL_FINGERMOTION && dragging_) {
    const float dx=event.tfinger.x-last_x_, dy=event.tfinger.y-last_y_;
    camera.x=std::clamp(camera.x+dx*2.0F/camera.zoom,-2.0F,2.0F);
    camera.y=std::clamp(camera.y+dy*2.0F/camera.zoom,-2.0F,2.0F);
    last_x_=event.tfinger.x; last_y_=event.tfinger.y;
  } else if (event.type==SDL_FINGERUP) {
    dragging_=false;
    camera.selected_tile=static_cast<int>(event.tfinger.x*12.0F)+static_cast<int>(event.tfinger.y*9.0F)*12;
  } else if (event.type==SDL_MOUSEWHEEL) {
    camera.zoom=std::clamp(camera.zoom*(event.wheel.y>0?1.1F:0.9F),0.65F,2.5F);
  }
}
}
