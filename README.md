# AOE1 Android

Android ARM64 port of the open-source [FolkertVanVerseveld/aoe](https://github.com/FolkertVanVerseveld/aoe) Age of Empires / The Rise of Rome reimplementation.

## Status

The upstream engine is pinned as a recursive Git submodule at `third_party/aoe-upstream`. The Android CMake path selects the upstream engine, applies GLES3/SDL Android compatibility adjustments, and builds toward SDL's `libmain.so` entry point. This milestone is under CI and runtime validation; it is not yet a confirmed playable Android release.

See [docs/UPSTREAM_PORT_AUDIT.md](docs/UPSTREAM_PORT_AUDIT.md) for details and remaining blockers.

## Game data

This repository does not distribute Microsoft's proprietary game assets, executable files, fonts, audio, or other game data. Players must provide data from a legitimate copy of Age of Empires or Rise of Rome.

## Checkout and build

```sh
git clone --recurse-submodules https://github.com/khahdihdz/aoe1-android.git
bash scripts/bootstrap-sdl.sh
./gradlew :app:assembleDebug
```
