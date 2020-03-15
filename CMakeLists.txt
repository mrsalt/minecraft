cmake_minimum_required(VERSION 3.0.0)
project(Minecraft VERSION 0.1.0)

set(CMAKE_PREFIX_PATH "./vcpkg/installed/x86-windows")
find_library(CAIRO_LIB "cairo")
find_path(CAIRO_INCLUDE_DIR "cairo.h")
message("CAIRO_LIB=${CAIRO_LIB}")
message("CAIRO_INCLUDE_DIR=${CAIRO_INCLUDE_DIR}")

add_subdirectory(construction)