# AOE1 Android

Android ARM64 port of the open-source [FolkertVanVerseveld/aoe](https://github.com/FolkertVanVerseveld/aoe) Age of Empires / The Rise of Rome reimplementation.

## Status

The upstream engine is now pinned as a Git submodule at `third_party/aoe-upstream`. The current Android APK workflow still builds the original SDL2/GLES foundation shell; **the upstream engine is not yet linked into the APK**. See [the Android port audit](docs/UPSTREAM_PORT_AUDIT.md) for the verified blockers and implementation stages.

## Game data

The upstream project requires the player to provide data from a legitimate copy of Age of Empires or Rise of Rome. This repository does not distribute Microsoft's game assets, executable files, fonts, audio, or other proprietary data.

## Checkout

```sh
git clone --recurse-submodules https://github.com/khahdihdz/aoe1-android.git
```

## Build

See [docs/BUILD.md](docs/BUILD.md). The current build produces a foundation/debug APK, not yet a playable port of the original game.
