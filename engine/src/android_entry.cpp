#include <SDL.h>

extern int aoe_upstream_main(int argc, char **argv);

extern "C" int SDL_main(int argc, char **argv) {
    SDL_SetMainReady();
    return aoe_upstream_main(argc, argv);
}
