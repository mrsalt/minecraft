cmake_minimum_required(VERSION 3.0.0)
project(Minecraft VERSION 0.1.0)

set(CMAKE_PREFIX_PATH "./vcpkg/installed/x86-windows")
find_library(CAIRO_LIB "cairo")
find_path(CAIRO_INCLUDE_DIR "cairo.h")
message("CAIRO_LIB=${CAIRO_LIB}")
message("CAIRO_INCLUDE_DIR=${CAIRO_INCLUDE_DIR}")

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Allow INSTALL_GTEST to be set to OFF without a cmake policy warning.
if (POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif (POLICY CMP0077)
set(INSTALL_GTEST OFF)
add_subdirectory(../googletest ${CMAKE_BINARY_DIR}/googletest)

add_subdirectory(construction)