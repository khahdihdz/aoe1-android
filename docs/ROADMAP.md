# AoE1 Web roadmap

## v0.1.0 — browser foundation
- [x] Web-only repository structure
- [x] Responsive browser launcher
- [x] WebAssembly and WebGL 2 capability checks
- [x] GitHub Pages and Cloudflare Pages workflows
- [x] Pin upstream C++ engine source
- [x] Remove native Android / Gradle / NDK / APK project
- [ ] First Emscripten build artifact

## v0.2.x — browser engine
- Emscripten SDL2 and WebGL 2 integration
- SDL_mixer / browser audio compatibility
- Browser event loop and rendering validation
- Desktop mouse/keyboard and mobile touch input

## v0.3.x — game data
- DRS container inspection
- SLP decoder with test fixtures
- DAT schema research and parser tests
- User-selected local game data import and validation

## v0.4.x — gameplay
- Integrate upstream engine runtime
- Validate maps, units, economy, construction and combat
- Save/load through browser storage with user consent

## v0.5.x — multiplayer
- Browser-compatible WebSocket transport
- Lobby, deterministic lockstep and state hashes
- Desync detection, reconnect and room lifecycle tests

## v1.0.0 — cross-device release
- Desktop, Android browser, iPhone/iPad and tablet testing
- Responsive HUD, touch gestures, accessibility and performance
- GitHub Pages and Cloudflare Pages production release
