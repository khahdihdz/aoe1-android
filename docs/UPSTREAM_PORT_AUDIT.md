# Upstream AoE Android port audit

## Selected upstream

- Project: [FolkertVanVerseveld/aoe](https://github.com/FolkertVanVerseveld/aoe)
- Pinned revision: `8894335d54371648b46d3c18bb2c6283fd2a604d` (2026-11-07)
- License: Apache-2.0 (retain upstream LICENSE and notices)
- Import method: Git submodule at `third_party/aoe-upstream`, including its declared submodules.

The upstream project is a C++17 reimplementation of Age of Empires / The Rise of Rome. It expects original game data supplied by the player. No proprietary game data is included here.

## Findings from the source

The upstream desktop CMake target currently assumes:

- SDL2 and SDL2_mixer are discoverable as desktop packages.
- OpenGL desktop headers/libraries and the bundled `gl3w` loader.
- Dear ImGui core plus SDL2 and OpenGL3 backends.
- `miniz`, `tracy`, and `wepoll` git submodules.
- Desktop window/display/cursor/fullscreen behavior.
- Desktop filesystem paths and Linux process/browser helpers.

The existing Android shell builds a separate SDL2 + OpenGL ES 3 demo. It is not yet building the upstream `empires` target. This audit intentionally records that boundary instead of claiming an APK already runs the original game.

## Android blockers to resolve before enabling the upstream target

1. **Graphics loader:** replace the desktop `gl3w` initialization and GL headers with an Android GLES3 path; validate all used GL entry points and shader versions.
2. **Dear ImGui:** compile the SDL2 + OpenGL3 backend against GLES3 with an ES shader version; avoid desktop GL loader assumptions.
3. **CMake dependencies:** provide Android builds of SDL2_mixer (and its codecs), link Android system libraries, and omit desktop OpenGL / pkg-config / epoll-shim discovery on Android.
4. **SDL platform layer:** guard `SDL_syswm.h`, desktop display mode, cursor, window positioning and clipping code. Android owns the activity/window lifecycle.
5. **Touch interaction:** map finger events to game UI and world selection; add pinch zoom, drag camera, long-press/context commands and multi-touch-safe gesture handling.
6. **Storage:** add Android document-picker / SAF import for the user's legally obtained AoE data folder. Persist only an app-private URI/path grant and expose a clear data-folder setup screen.
7. **Lifecycle/audio:** handle pause/resume, audio focus, surface/context recreation, and safe shutdown/restart.
8. **Validation:** build and run on a physical ARM64 device with a user-supplied data set. CI compilation alone is not proof of gameplay compatibility.

## Port sequence

- **P0 (this change):** pin and fetch the upstream source as a submodule, preserve license, document blockers.
- **P1:** make the upstream source compile for Android ARM64 (GLES3, SDL2_mixer, platform guards).
- **P2:** launch the actual upstream engine in the Android Activity and add storage onboarding.
- **P3:** implement touch-first RTS controls and lifecycle hardening.
- **P4:** validate AoE and Rise of Rome data compatibility on real devices; produce signed release builds.

## Reproducible checkout

```sh
git clone --recurse-submodules https://github.com/khahdihdz/aoe1-android.git
# or, inside an existing clone:
git submodule update --init --recursive
```

The submodule is pinned by the gitlink in this repository; do not silently float it to upstream `master`.
