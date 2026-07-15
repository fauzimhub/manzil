# zig-toolchain.cmake
#
# Use Zig's bundled Clang as the C/C++ compiler + linker, and force
# a Release build. Requires `zig` on PATH (any recent version, e.g. 0.13+).
#
# Usage:
#   cmake -S . -B build-zig \
#         -DCMAKE_TOOLCHAIN_FILE=cmake/zig-toolchain.cmake \
#         -DZIG_TARGET=x86_64-windows-gnu \
#         -G Ninja

#   cmake --build build-zig
#
# (You do NOT need to also pass -DCMAKE_BUILD_TYPE=Release; this file
#  pins it. Remove the force-set block below if you'd rather choose
#  the config at configure time.)

# --- Force Release ---------------------------------------------------------
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)

# --- Optional: pick a Zig target triple for cross-compiling ---------------
# Leave unset to build for the host. Example cross targets:
#   x86_64-linux-gnu, aarch64-macos, x86_64-windows-gnu
set(ZIG_TARGET "" CACHE STRING "Zig -target triple (empty = host)")

if(ZIG_TARGET)
  set(CMAKE_C_COMPILER_TARGET "${ZIG_TARGET}")
  set(CMAKE_CXX_COMPILER_TARGET "${ZIG_TARGET}")

  # Tell CMake this IS a cross-compile, and what the target platform is.
  # wxWidgets' own CMake (and everything else) reads CMAKE_SYSTEM_NAME to
  # decide which port/toolkit to configure for.
  if(ZIG_TARGET MATCHES "windows")
    set(CMAKE_SYSTEM_NAME "Windows")
    set(CMAKE_SYSTEM_PROCESSOR "AMD64")
    set(CMAKE_EXECUTABLE_SUFFIX ".exe")

    # RC compiler using zig rc
    find_program(ZIG_EXECUTABLE zig REQUIRED)
    set(CMAKE_RC_COMPILER "${ZIG_EXECUTABLE}" CACHE FILEPATH "" FORCE)
    set(CMAKE_RC_COMPILE_OBJECT
        "<CMAKE_RC_COMPILER> rc /fo <OBJECT> /i ${CMAKE_SOURCE_DIR}/third_party/wxwidgets/include <SOURCE>"
        CACHE STRING "" FORCE)
    set(CMAKE_RC_OUTPUT_EXTENSION ".res" CACHE STRING "" FORCE)
  elseif(ZIG_TARGET MATCHES "macos")
    set(CMAKE_SYSTEM_NAME "Darwin")
    set(CMAKE_SYSTEM_PROCESSOR "arm64")
  elseif(ZIG_TARGET MATCHES "linux")
    set(CMAKE_SYSTEM_NAME "Linux")
    set(CMAKE_SYSTEM_PROCESSOR "x86_64")
  endif()

  # Don't let CMake go looking in host system paths (/usr/lib etc.) for
  # libraries/headers when cross-compiling.
  set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
  set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
  set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
  set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
endif()

# --- Locate wrapper scripts (this file lives next to them) ----------------
get_filename_component(_ZIG_TOOLCHAIN_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

set(_ZIGCC "${_ZIG_TOOLCHAIN_DIR}/zigcc")
set(_ZIGCXX "${_ZIG_TOOLCHAIN_DIR}/zigcxx")
set(_ZIGAR "${_ZIG_TOOLCHAIN_DIR}/zigar")
set(_ZIGRANLIB "${_ZIG_TOOLCHAIN_DIR}/zigranlib")

if(NOT EXISTS "${_ZIGCC}")
  message(
    FATAL_ERROR
      "zig-toolchain.cmake: expected wrapper script at ${_ZIGCC} "
      "(zigcc/zigcxx/zigar/zigranlib must sit next to this .cmake file)")
endif()

# --- Bake -target into the wrappers via env var ----------------------------
# zig cc reads no env var for -target, so instead we pass it as extra flags.
if(ZIG_TARGET)
  set(CMAKE_C_COMPILER_TARGET "${ZIG_TARGET}")
  set(CMAKE_CXX_COMPILER_TARGET "${ZIG_TARGET}")
endif()

# --- Compiler / archiver setup ---------------------------------------------
set(CMAKE_C_COMPILER "${_ZIGCC}" CACHE FILEPATH "" FORCE)
set(CMAKE_CXX_COMPILER "${_ZIGCXX}" CACHE FILEPATH "" FORCE)
set(CMAKE_AR "${_ZIGAR}" CACHE FILEPATH "" FORCE)
set(CMAKE_RANLIB "${_ZIGRANLIB}" CACHE FILEPATH "" FORCE)

# NOTE: Do NOT force CMAKE_C_COMPILER_ID / CMAKE_CXX_COMPILER_ID here.
# zig cc/c++ identify correctly as Clang on their own during normal
# compiler detection, and that detection is also what sets
# CMAKE_EXECUTABLE_FORMAT (e.g. "ELF"). Forcing the ID skips detection
# entirely and breaks platform-format-dependent logic elsewhere (e.g.
# wxWidgets' webview/wxrc targets fail their install-RPATH relink step
# with "not supported with the Ninja generator unless on an ELF-based
# ... platform" because CMAKE_EXECUTABLE_FORMAT never gets set).

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Avoid the install-time RPATH relink step Ninja can't do for
# non-detected-ELF cases; harmless on real ELF platforms too, and
# is exactly what the CMake error message itself suggests.
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)

# --- wxWidgets/nlohmann_json build fine under Clang, but note: -------------
# -Weffc++ is a GCC-only warning flag. Zig's cc is Clang-based and will
# reject or ignore it depending on version. If configure/build fails on
# that flag, drop -Weffc++ from your Debug warnings list, or wrap it:
#   $<$<CONFIG:Debug>:$<$<CXX_COMPILER_ID:GNU>:-Weffc++>>
