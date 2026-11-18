# Upstream AoE Android port status

## Current integration milestone

The Android CMake path now selects the upstream `empires` engine instead of the standalone demo renderer. It applies a small Android compatibility layer at configure time and builds the upstream executable as `libmain.so`, which SDL's Android Activity loads.

Pinned upstream source: `8894335d54371648b46d3c18bb2c6283fd2a604d`.

## Android compatibility work in this milestone

- Use NDK GLES3 headers instead of desktop gl3w.
- Request an OpenGL ES 3.0 context and GLSL ES 3.00.
- Exclude desktop SDL system-window headers and use SDL_OpenURL on Android.
- Skip system mouse cursor and desktop fullscreen/window-clipping behavior.
- Build SDL2_mixer for Android with optional desktop codecs disabled for the first bring-up. WAV/PCM is the initial audio profile; MP3 music support is deferred.
- Compile Dear ImGui's OpenGL3 backend in ES3 mode.
- Rename upstream main() at compile time and bridge SDL_main to SDL's Android Activity entry point.

## Not yet complete / must be validated

1. CI may reveal more desktop assumptions in upstream code or its bundled ImGui revision; compiler output is authoritative.
2. Android pause/resume, back button, audio focus and GL context recreation require device testing.
3. The upstream file browser expects desktop paths. Android Storage Access Framework (SAF) integration is still required for game data selection.
4. Touch input is not yet a complete RTS command model. Validate SDL touch/mouse behavior, then add pinch zoom, multi-selection and context commands.
5. Proprietary game data is not included; users must import data from a legitimate copy.
6. A successful APK compile does not prove gameplay compatibility. Test on a physical ARM64 device with valid data.

## Build

```sh
git clone --recurse-submodules https://github.com/khahdihdz/aoe1-android.git
bash scripts/bootstrap-sdl.sh
./gradlew :app:assembleDebug
```
