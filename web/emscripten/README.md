# Emscripten browser smoke test

This is an isolated toolchain check, **not the AoE game engine**.

It verifies that GitHub Actions can compile a minimal SDL2 application to
WebAssembly with a WebGL 2 / OpenGL ES 3 context. The generated HTML, JavaScript,
and WASM files are uploaded as a workflow artifact for manual browser testing.

Run locally after installing and activating the Emscripten SDK:

```sh
mkdir -p dist
em++ web/emscripten/smoke.cpp -O2 \
  -sUSE_SDL=2 \
  -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -sFULL_ES3=1 \
  -sALLOW_MEMORY_GROWTH=1 -sEXIT_RUNTIME=0 \
  -o dist/aoe1-webgl-smoke.html
```

Serve `dist/` over HTTP(S), then open `aoe1-webgl-smoke.html` in a browser
with WebGL 2 enabled. A dark green canvas indicates that SDL2, the browser event
loop, and the WebGL 2 context initialized.

The production engine still needs separate work for its GL loader, ImGui
backends, threading, file system, audio, and native TCP networking. Passing
this smoke test does not mean the game itself has been ported.
