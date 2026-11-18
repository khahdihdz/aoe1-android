#include "aoe/platform/sdl_app.hpp"
extern "C" int SDL_main(int, char**) { return aoe::run_sdl_app(); }
