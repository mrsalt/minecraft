cmake_minimum_required(VERSION 3.0.0)
project(Minecraft VERSION 0.1.0)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  message("Detected 64-bit architecture")
  set(CMAKE_PREFIX_PATH "./vcpkg/installed/x64-windows")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
  message("Detected 32-bit architecture")
  set(CMAKE_PREFIX_PATH "./vcpkg/installed/x86-windows")
else()
  message(FATAL_ERROR "CMAKE_SIZEOF_VOID_P must be 4 or 8")
endif()

find_library(CAIRO_LIB "cairo")
find_path(CAIRO_INCLUDE_DIR "cairo.h")
message("CAIRO_LIB=${CAIRO_LIB}")
message("CAIRO_INCLUDE_DIR=${CAIRO_INCLUDE_DIR}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Tell visual studio to compile with Just My Code, so that you don't
# have to step through STL like crazy (try stepping through <functional>...
# without pulling your hair out)
if(MSVC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "19.15")
    add_compile_options( /JMC )
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Allow INSTALL_GTEST to be set to OFF without a cmake policy warning.
if (POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif (POLICY CMP0077)
set(INSTALL_GTEST OFF)
add_subdirectory(../googletest ${CMAKE_BINARY_DIR}/googletest)

add_subdirectory(cairo)
add_subdirectory(construction)