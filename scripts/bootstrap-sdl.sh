#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
VER="2.30.11"
DEST="$ROOT/third_party/SDL"
TMP="$ROOT/.tmp-sdl"
mkdir -p "$ROOT/third_party" "$TMP"
if [[ -f "$DEST/CMakeLists.txt" ]]; then echo "SDL2 already bootstrapped"; exit 0; fi
curl -fL "https://github.com/libsdl-org/SDL/releases/download/release-$VER/SDL2-$VER.tar.gz" -o "$TMP/sdl.tar.gz"
mkdir -p "$TMP/unpack"
tar -xzf "$TMP/sdl.tar.gz" -C "$TMP/unpack"
mv "$TMP/unpack/SDL2-$VER" "$DEST"
mkdir -p "$ROOT/android/app/src/main/java/org/libsdl/app"
cp "$DEST/android-project/app/src/main/java/org/libsdl/app/"*.java "$ROOT/android/app/src/main/java/org/libsdl/app/"
rm -rf "$TMP"
echo "SDL2 $VER ready."
