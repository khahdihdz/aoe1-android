#!/usr/bin/env python3
"""Apply a small, reproducible Android compatibility layer to the pinned AoE source."""
from pathlib import Path

root = Path(__file__).resolve().parents[1]
up = root / "third_party" / "aoe-upstream"

def replace(path, old, new, label):
    p = up / path
    s = p.read_text()
    if new in s:
        return
    if old not in s:
        raise SystemExit(f"Android patch failed ({label}): expected source block not found in {path}")
    p.write_text(s.replace(old, new, 1))

p = up / "game/src/engine/gl.hpp"
s = p.read_text()
if "#include <GLES3/gl3.h>" not in s:
    p.write_text(s.replace("#include <gl3w.h>", "#if defined(__ANDROID__)\n#include <GLES3/gl3.h>\n#else\n#include <gl3w.h>\n#endif", 1))

replace("game/src/engine.cpp",
'''#if _WIN32 || defined(AOE_SDL_NO_PREFIX)
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif
#else
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL2/SDL_opengles2.h>
#else
#include <SDL2/SDL_opengl.h>
#endif
#endif''',
'''#if defined(__ANDROID__)
#include <GLES3/gl3.h>
#elif _WIN32 || defined(AOE_SDL_NO_PREFIX)
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif
#else
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL2/SDL_opengles2.h>
#else
#include <SDL2/SDL_opengl.h>
#endif
#endif''', "GLES3 engine header")

replace("game/src/engine/sdl.cpp", "#include <SDL2/SDL_syswm.h>",
'''#if !defined(__ANDROID__)
#include <SDL2/SDL_syswm.h>
#endif''', "exclude desktop SDL system window header")

replace("game/src/engine/sdl.cpp",
'''SDLguard::SDLguard(Uint32 flags) : flags(flags)
#if defined(IMGUI_IMPL_OPENGL_ES2)
	, glsl_version("#version 100")
#elif defined(__APPLE__)''',
'''SDLguard::SDLguard(Uint32 flags) : flags(flags)
#if defined(__ANDROID__)
	, glsl_version("#version 300 es")
#elif defined(IMGUI_IMPL_OPENGL_ES2)
	, glsl_version("#version 100")
#elif defined(__APPLE__)''', "Android GLSL version")

replace("game/src/engine/sdl.cpp",
'''	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)''',
'''	// Decide GL+GLSL versions
#if defined(__ANDROID__)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(IMGUI_IMPL_OPENGL_ES2)''', "Android GLES context")

replace("game/src/engine/sdl.cpp",
'''	cursors.emplace_back(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW), SDL_FreeCursor);

	SDL_SetCursor(cursors.front().get());''',
'''#if !defined(__ANDROID__)
	cursors.emplace_back(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW), SDL_FreeCursor);
	SDL_SetCursor(cursors.front().get());
#endif''', "no system cursor on Android")

replace("game/src/engine/sdl.cpp",
'''void SDL::set_cursor(unsigned idx) {
	const auto &c = cursors.at(idx);
	SDL_SetCursor(c.get());
}''',
'''void SDL::set_cursor(unsigned idx) {
#if defined(__ANDROID__)
	(void)idx;
#else
	const auto &c = cursors.at(idx);
	SDL_SetCursor(c.get());
#endif
}''', "no-op cursor setter on Android")

replace("game/src/engine/sdl.cpp",
'''bool Window::is_fullscreen() {
	return (SDL_GetWindowFlags(win.get()) & SDL_WINDOW_FULLSCREEN) != 0;
}''',
'''bool Window::is_fullscreen() {
#if defined(__ANDROID__)
	return true;
#else
	return (SDL_GetWindowFlags(win.get()) & SDL_WINDOW_FULLSCREEN) != 0;
#endif
}''', "Android fullscreen state")

replace("game/src/engine/sdl.cpp",
'''void Window::set_fullscreen(bool v) {
	if (v == is_fullscreen()) {''',
'''void Window::set_fullscreen(bool v) {
#if defined(__ANDROID__)
	(void)v;
	UpdateUiFullscreen(true);
	return;
#else
	if (v == is_fullscreen()) {''', "Android fullscreen no-op")

replace("game/src/engine/sdl.cpp",
'''#if _WIN32
	reclip();
#endif

	UpdateUiFullscreen(v);
}''',
'''#if _WIN32
	reclip();
#endif

	UpdateUiFullscreen(v);
#endif
}''', "close Android fullscreen guard")

replace("game/src/engine/sdl.cpp",
'''#else
void Window::reclip() {}

bool Window::set_clipping(bool enable) {
	return false;
}
#endif''',
'''#else
void Window::reclip() {}

bool Window::set_clipping(bool enable) {
	(void)enable;
	return false;
}
#endif''', "unused clipping parameter")

replace("game/src/engine/audio.cpp",
'''	int flags = MIX_INIT_MP3;
	int ret;

	if (((ret = Mix_Init(flags)) & flags) != flags) {''',
'''#if defined(__ANDROID__)
	int flags = 0; // WAV/PCM only in the first Android bring-up.
#else
	int flags = MIX_INIT_MP3;
#endif
	int ret;

	if (((ret = Mix_Init(flags)) & flags) != flags) {''', "Android audio codec initialization")

replace("game/src/os.cpp",
'''#if _WIN32
#include <shellapi.h>
#elif defined(__linux__) || defined(__APPLE__)''',
'''#if defined(__ANDROID__)
#include <SDL.h>
#elif _WIN32
#include <shellapi.h>
#elif defined(__linux__) || defined(__APPLE__)''', "Android URL include")

replace("game/src/os.cpp",
'''#if _WIN32
void open_url(const char *url) {''',
'''#if defined(__ANDROID__)
void open_url(const char *url) {
	if (SDL_OpenURL(url) != 0 && eng)
		eng->push_error(__func__, std::string("Cannot open URL: ") + SDL_GetError());
}
#elif _WIN32
void open_url(const char *url) {''', "Android URL launcher")

compat = root / "third_party" / "android-compat" / "SDL2"
compat.mkdir(parents=True, exist_ok=True)
sdl_include = root / "third_party" / "SDL" / "include"
for header in sdl_include.glob("SDL*.h"):
    link = compat / header.name
    if not link.exists():
        link.symlink_to(header.resolve())

p = up / "CMakeLists.txt"
s = p.read_text()
marker = 'option(BUILD_TESTS "Build unit tests" OFF)'
if "if(ANDROID) # AOE_ANDROID_PORT" not in s:
    block = '''
if(ANDROID) # AOE_ANDROID_PORT
include(FetchContent)
set(SDL2MIXER_INSTALL OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_SAMPLES OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_VENDORED ON CACHE BOOL "" FORCE)
set(SDL2MIXER_CMD OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_FLAC OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_MOD OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_MP3 OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_MIDI OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_OPUS OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_VORBIS "" CACHE STRING "" FORCE)
set(SDL2MIXER_WAVE OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_WAVPACK OFF CACHE BOOL "" FORCE)
FetchContent_Declare(SDL2_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG release-2.8.1
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(SDL2_mixer)

set(PROJECT_SRCDIR ${CMAKE_CURRENT_SOURCE_DIR})
set(GAME_DIR ${PROJECT_SRCDIR}/game)
set(GAME_SRCDIR ${GAME_DIR}/src)
file(GLOB_RECURSE GAME_SOURCES ${GAME_SRCDIR}/*.cpp ${GAME_SRCDIR}/*.c)
list(FILTER GAME_SOURCES EXCLUDE REGEX "/external/gl3w\\.c$")
set(IMGUI_DIR ${PROJECT_SRCDIR}/imgui)
set(IMGUI_BACKENDS_DIR ${IMGUI_DIR}/backends)
file(GLOB IMGUI_SRC ${IMGUI_DIR}/*.cpp
    ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.cpp
    ${IMGUI_BACKENDS_DIR}/imgui_impl_sdl2.cpp)
add_library(empires SHARED
    ${GAME_DIR}/main.cpp
    ${GAME_SOURCES}
    ${IMGUI_SRC}
    ${CMAKE_CURRENT_LIST_DIR}/../../engine/src/android_entry.cpp)
target_include_directories(empires PRIVATE
    ${IMGUI_DIR} ${IMGUI_BACKENDS_DIR}
    ${GAME_SRCDIR}/core ${GAME_SRCDIR}/external
    ${PROJECT_SRCDIR}/tracy/public ${PROJECT_SRCDIR}/miniz
    ${CMAKE_CURRENT_LIST_DIR}/../../third_party/android-compat)
target_compile_definitions(empires PRIVATE
    SDL_MAIN_HANDLED AOE_SDL_NO_PREFIX IMGUI_IMPL_OPENGL_ES3
    main=aoe_upstream_main)
target_link_libraries(empires PRIVATE SDL2 SDL2_mixer GLESv3 android log)
set_target_properties(empires PROPERTIES OUTPUT_NAME main)
return()
endif() # AOE_ANDROID_PORT

'''
    if marker not in s:
        raise SystemExit("Android patch failed: CMake insertion point missing")
    p.write_text(s.replace(marker, block + marker, 1))

print("Applied Android compatibility layer to pinned AoE source.")
