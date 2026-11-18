# AoE1 Web port plan

## Repository decision

The web target lives beside the Android target in this repository. Android files and workflows remain intact. The web shell is a truthful launcher/status page until the upstream C++ engine has a working browser build; it is not a JavaScript recreation of AoE.

## Upstream audit

Upstream: https://github.com/FolkertVanVerseveld/aoe (Apache-2.0; default branch `master`).

The upstream project is C++17 and currently uses SDL2, SDL2_mixer, OpenGL, Dear ImGui and several vendored/submodule dependencies. Its README describes basic gameplay and multiplayer as the current playable path; single-player is approximated by hosting a multiplayer session and adding computer players, whose behavior is limited. It requires the user to own and supply original AoE / Rise of Rome data.

The Android repository currently contains a separate C++20 SDL2/OpenGL ES demo shell and touch controller. It does not contain the upstream engine source. Keep this native Android foundation intact while introducing a separate web target.

## Port strategy

1. **Baseline and provenance**
   - Keep upstream Apache-2.0 notices and preserve attribution.
   - Pin an upstream commit before importing source; do not track a moving branch in release builds.
   - Keep proprietary AoE data out of Git, CI artifacts, Pages and Cloudflare.
2. **Build a browser target**
   - Use Emscripten with its SDL2 port and WebGL 2.
   - Audit and adapt the upstream CMake target rather than compiling the Android demo as the game.
   - Exclude desktop-only APIs/backends and profiler/tools from the first browser target.
   - Replace unsupported filesystem assumptions with SDL/Emscripten virtual filesystem and an explicit user data import flow (IDBFS or OPFS persistence only after user consent).
3. **Graphics and UI**
   - Start with WebGL 2 / GLES3-compatible rendering; audit OpenGL loader and shaders.
   - Use SDL2 browser event loop (no blocking main loop); verify audio gesture unlock and browser resize/fullscreen behavior.
   - Add pointer/touch mapping for pan, pinch zoom, selection, drag-box and context commands.
4. **Networking**
   - Audit upstream socket code. Native TCP/UDP sockets cannot be carried directly into browser WebAssembly.
   - Introduce a transport interface and implement WebSocket transport in the browser. A Cloudflare Worker can broker signaling/room traffic, but the game protocol, authoritative ownership, reconnect and packet framing must be implemented and tested; Workers are not a drop-in TCP server.
   - Preserve deterministic lockstep semantics if that is what upstream gameplay uses. Do not claim multiplayer until two real browser clients pass synchronization tests.
5. **Validation**
   - Build native upstream first; then Emscripten compile; then run in Chromium/Firefox.
   - Test data import, DRS/SLP/DAT parsing, audio, resize, context loss, long sessions, reconnect and two-player desync detection.
   - Publish only the launcher until a real wasm engine artifact exists.

## Known blockers / engineering risks

- Upstream CMake currently requires SDL2, SDL2_mixer and OpenGL and builds a desktop executable. Emscripten SDL2 is available, but SDL_mixer port/version compatibility and audio codecs must be verified.
- ImGui SDL/OpenGL backends and the upstream OpenGL loader may need WebGL2-specific changes.
- Native networking, threads, file paths, dynamic loading and OS compatibility shims need browser-specific alternatives.
- The upstream project itself reports limited gameplay/AI completeness; porting does not add missing gameplay features.
- A browser build may need COOP/COEP headers if SharedArrayBuffer/pthreads are used. Avoid pthreads initially unless profiling proves they are required.

## Current milestone

- [x] Add responsive web launcher and WebAssembly/WebGL capability checks.
- [x] Add this source audit and staged port plan.
- [ ] Pin and import upstream C++ source with attribution.
- [ ] Produce first Emscripten `.wasm/.js/.data` artifact.
- [ ] Implement browser filesystem/data validation.
- [ ] Port and validate multiplayer transport.
- [ ] Publish playable build to both Pages and Cloudflare Pages.
