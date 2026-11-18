# Architecture

The foundation is a clean-room native application shell. It contains no Microsoft game code or proprietary AOE data.

- Android Activity: lifecycle and SDL2 host.
- SDL2: window, input and event loop.
- OpenGL ES 3.0: procedural isometric tile renderer.
- C++20: future simulation and game systems.
- TouchController: pan, tap selection and mouse-wheel zoom for desktop testing.

Planned modules: local asset import (DRS/SLP/DAT), deterministic simulation, entities, economy, combat, pathfinding, AI, save/load and lockstep networking.

The demo tile field is generated mathematically and is not copied from the commercial game.
