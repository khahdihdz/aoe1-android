cmake_minimum_required(VERSION 3.22.1)
project(aoe1_android LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/SDL/CMakeLists.txt")
  message(FATAL_ERROR "SDL2 missing. Run scripts/bootstrap-sdl.sh first.")
endif()
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/aoe-upstream/CMakeLists.txt")
  message(FATAL_ERROR "Upstream AoE source missing. Run git submodule update --init --recursive.")
endif()

set(SDL_SHARED ON CACHE BOOL "" FORCE)
set(SDL_STATIC OFF CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
set(SDL_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/SDL EXCLUDE_FROM_ALL)

if(ANDROID)
  execute_process(
    COMMAND python3 "${CMAKE_CURRENT_SOURCE_DIR}/scripts/patch-upstream-android.py"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    RESULT_VARIABLE AOE_ANDROID_PATCH_RESULT)
  if(NOT AOE_ANDROID_PATCH_RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to apply Android compatibility layer to upstream AoE.")
  endif()
  add_subdirectory(third_party/aoe-upstream)
else()
  add_library(aoe_host_check STATIC engine/src/input/touch_controller.cpp)
  target_include_directories(aoe_host_check PRIVATE engine/include)
  target_link_libraries(aoe_host_check PRIVATE SDL2)
endif()
