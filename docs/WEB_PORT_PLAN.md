# AoE1 Web port plan

## Repository decision

This repository is **Web-only**. Android-native source, Gradle, Android NDK targets, APK workflows, and platform-specific shells have been removed. Android phones and tablets remain supported as browser clients; users open the same web app in Chrome, Firefox, Safari, or another modern browser.

The current web shell is a truthful launcher/status page until the upstream C++ engine has a working browser build. It is not a JavaScript recreation of AoE and is not yet a playable game.

## Upstream audit

Upstream: https://github.com/FolkertVanVerseveld/aoe (Apache-2.0).

The pinned upstream project is C++17 and uses SDL2, SDL2_mixer, OpenGL, Dear ImGui and several vendored/submodule dependencies. Its README describes basic gameplay and multiplayer as the current playable path; computer-player behavior is limited. It requires the user to own and supply original AoE / Rise of Rome data.

Pinned source commit: `8894335d54371648b46d3c18bb2c6283fd2a604d`, linked at `upstream/aoe`.

## Port strategy

1. **Browser-first build**
   - Use Emscripten and its SDL2 port with WebGL 2.
   - Create a dedicated browser CMake overlay; do not reintroduce Android or desktop-native targets.
   - Audit and adapt upstream CMake rather than compiling the old demo shell.
   - Exclude desktop-only APIs/backends and profiler/tools from the first browser target.
2. **Files and user data**
   - Replace native filesystem assumptions with SDL/Emscripten virtual filesystem.
   - Import game data from a user-selected folder or archive. Keep it local to the browser; never upload it to CI or hosting.
   - Consider IDBFS/OPFS persistence only after clear user consent.
3. **Graphics, input and responsive UI**
   - Start with WebGL 2 / GLES3-compatible rendering; audit OpenGL loader and shaders.
   - Use SDL2 browser event loop (no blocking main loop); verify audio gesture unlock and browser resize/fullscreen behavior.
   - Support mouse, keyboard, touch, pointer events, pan, pinch zoom, selection, drag-box and context commands.
   - Adapt HUD and controls for narrow portrait screens and landscape play on phones/tablets as well as desktop displays.
4. **Networking**
   - Native TCP/UDP sockets cannot be carried directly into browser WebAssembly.
   - Introduce a transport interface and implement WebSocket transport in the browser. A Cloudflare Worker can broker signaling/room traffic, but game protocol, ownership, reconnect and packet framing must be implemented and tested.
   - Preserve deterministic lockstep semantics if used upstream. Do not claim multiplayer until two real browser clients pass synchronization tests.
5. **Validation and publishing**
   - Build Emscripten in CI, then test on Chromium, Firefox and Safari/WebKit.
   - Test desktop mouse/keyboard and mobile touch, viewport changes, fullscreen, audio, data import, context loss, reconnect and two-player desync.
   - Publish only the launcher until a real WASM engine artifact exists.

## Known blockers

- Upstream CMake currently requires SDL2, SDL2_mixer and OpenGL and builds a desktop executable. Emscripten SDL2 is available, but SDL_mixer compatibility must be verified.
- ImGui SDL/OpenGL backends and the upstream OpenGL loader may need WebGL2-specific changes.
- Native networking, threads, file paths, dynamic loading and OS compatibility shims need browser-specific alternatives.
- Upstream gameplay/AI is itself incomplete in places; porting does not add missing gameplay features.
- COOP/COEP headers may be needed if SharedArrayBuffer/pthreads are introduced. Avoid pthreads initially.

## Current milestone

- [x] Add responsive web launcher and WebAssembly/WebGL capability checks.
- [x] Add source audit and staged browser-port plan.
- [x] Pin upstream C++ source as a Git submodule with attribution.
- [x] Remove Android-native project, Gradle, NDK and APK workflows.
- [ ] Produce first Emscripten `.wasm/.js/.data` artifact.
- [ ] Implement browser filesystem and game-data validation.
- [ ] Port and validate multiplayer transport.
- [ ] Publish playable build to GitHub Pages and Cloudflare Pages.
