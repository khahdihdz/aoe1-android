# Build guide

## Requirements
- JDK 17
- Android SDK platform 35
- Android NDK 27.2.12479018
- CMake 3.22.1
- Gradle 8.9
- Bash, curl and tar

## Bootstrap SDL2
From repository root:
```bash
bash scripts/bootstrap-sdl.sh
```
This downloads upstream SDL 2.30.11 and copies its Android Java activity classes into the app. SDL source and Java glue are generated dependencies, not committed.

## Build ARM64 debug APK
```bash
gradle :app:assembleDebug
```
Output: `android/app/build/outputs/apk/debug/app-debug.apk`.

The foundation build is a debug APK signed with Android's debug key. Do not distribute it as a production release. Configure a private release keystore through CI secrets before publishing.

## Install
```bash
adb install -r app/build/outputs/apk/debug/app-debug.apk
```
Requires OpenGL ES 3.0 and arm64-v8a.

## Troubleshooting
- If CMake cannot find SDL, rerun the bootstrap script.
- Install SDK platform 35, NDK 27.2.12479018 and CMake 3.22.1.
- If SDLActivity is missing, confirm bootstrap copied files into `android/app/src/main/java/org/libsdl/app`.
