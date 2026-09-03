# CMake build for FreeImage 3.18.0
#
# FreeImage ships no CMake build of its own -- only a GNU makefile and MSVC
# project files. This supplies one, building a static `freeimage` target from
# the sources the top-level CMakeLists.txt downloaded, whose location it passes
# in as HB_FREEIMAGE_DIR.
#
# Note this file deliberately lives in the repository and reads the extracted
# tree from outside, rather than being copied into it: a FetchContent
# PATCH_COMMAND runs only on first population, so edits here would not be
# picked up on subsequent configures.
#
# Rather than hardcoding 471 source paths, the source list and include
# directories are parsed out of FreeImage's own Makefile.srcs, which upstream
# generates from its MSVC project files (see gensrclist.sh). That keeps this
# file correct if the pinned version is ever bumped.
#
# Compile definitions are taken from FreeImage's own static-library project,
# Source/FreeImageLib/FreeImageLib.2017.vcxproj, and from Makefile.gnu for
# non-MSVC toolchains.

project(freeimage C CXX)

if(NOT HB_FREEIMAGE_DIR)
  message(FATAL_ERROR "HB_FREEIMAGE_DIR must point at the extracted FreeImage sources")
endif()

# --- source list -------------------------------------------------------------
# Makefile.srcs holds one very long "SRCS = ./Source/a.cpp ./Source/b.c ..." line.
file(STRINGS "${HB_FREEIMAGE_DIR}/Makefile.srcs" _fi_srcs_line REGEX "^SRCS[ \t]*=")
if(NOT _fi_srcs_line)
  message(FATAL_ERROR "FreeImage: could not read the SRCS list from Makefile.srcs")
endif()

string(REGEX REPLACE "^SRCS[ \t]*=[ \t]*" "" _fi_srcs_line "${_fi_srcs_line}")
string(REGEX REPLACE "[ \t]+" ";" _fi_srcs "${_fi_srcs_line}")

set(FREEIMAGE_SOURCES "")
foreach(_src IN LISTS _fi_srcs)
  if(_src MATCHES "\\.(c|cpp)$")
    string(REGEX REPLACE "^\\./" "" _src "${_src}")
    list(APPEND FREEIMAGE_SOURCES "${HB_FREEIMAGE_DIR}/${_src}")
  endif()
endforeach()

list(LENGTH FREEIMAGE_SOURCES _fi_count)
if(_fi_count LESS 400)
  message(FATAL_ERROR "FreeImage: parsed only ${_fi_count} sources from Makefile.srcs; expected ~471")
endif()
message(STATUS "FreeImage: building ${_fi_count} source files")

# --- include directories -----------------------------------------------------
# Parsed from the "INCLUDE = -I. -ISource -ISource/Metadata ..." line.
file(STRINGS "${HB_FREEIMAGE_DIR}/Makefile.srcs" _fi_inc_line REGEX "^INCLUDE[ \t]*=")
string(REGEX REPLACE "^INCLUDE[ \t]*=[ \t]*" "" _fi_inc_line "${_fi_inc_line}")
string(REGEX REPLACE "[ \t]+" ";" _fi_incs "${_fi_inc_line}")

set(FREEIMAGE_PRIVATE_INCLUDES "")
foreach(_inc IN LISTS _fi_incs)
  if(_inc MATCHES "^-I(.+)$")
    list(APPEND FREEIMAGE_PRIVATE_INCLUDES "${HB_FREEIMAGE_DIR}/${CMAKE_MATCH_1}")
  endif()
endforeach()

# --- target ------------------------------------------------------------------
add_library(freeimage STATIC ${FREEIMAGE_SOURCES})

set_target_properties(freeimage PROPERTIES
  C_STANDARD 99
  CXX_STANDARD 11
  POSITION_INDEPENDENT_CODE ON
  FOLDER "deps")

target_include_directories(freeimage
  PUBLIC  "${HB_FREEIMAGE_DIR}/Source"
  PRIVATE ${FREEIMAGE_PRIVATE_INCLUDES})

# FREEIMAGE_LIB and FREEIMAGE_COLORORDER are both read by the *public*
# FreeImage.h, so they must be PUBLIC -- the library and every consumer have to
# agree on them.
#
#   FREEIMAGE_LIB       marks the static build. HTMLButcher's wrapper guards its
#                       FreeImage_Initialise()/DeInitialise() calls on this
#                       (src/wxFreeImage.cpp). Without it no image format plugin
#                       is ever registered and every load and save fails at
#                       runtime, silently.
#   FREEIMAGE_COLORORDER  pinned to BGR (0). src/wxFreeImage.cpp unconditionally
#                       swaps red and blue when handing scanlines to wxImage, so
#                       an RGB-ordered build inverts every image.
target_compile_definitions(freeimage
  PUBLIC
    FREEIMAGE_LIB
    FREEIMAGE_COLORORDER=0
  PRIVATE
    OPJ_STATIC
    LIBRAW_NODLL
    # LibJXR's perf timer is declared in image/sys/perfTimer.h but only defined
    # in perfTimerANSI.c, which FreeImage's MSVC project -- and therefore the
    # source list in Makefile.srcs -- does not include. Without this the link
    # fails on PerfTimerNew/Start/Stop/Delete/CopyStartTime. Makefile.gnu
    # defines it for the same reason; it just goes for every toolchain here.
    DISABLE_PERF_MEASUREMENT)

if(MSVC)
  target_compile_definitions(freeimage PRIVATE
    _CRT_SECURE_NO_DEPRECATE
    _CRT_SECURE_NO_WARNINGS)
  # This is 2018 third-party C: warnings are noise, and MSVC's conformance
  # defaults break several of the bundled libraries.
  target_compile_options(freeimage PRIVATE /W0 /wd4996 /bigobj)
else()
  # Mirrors Makefile.gnu.
  target_compile_definitions(freeimage PRIVATE
    NO_LCMS
    __ANSI__)
  target_compile_options(freeimage PRIVATE
    -fexceptions
    -fvisibility=hidden
    -w)
  target_compile_options(freeimage PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-Wno-ctor-dtor-privacy>)
endif()

# LibJXR needs the Windows-style types on Windows and the ANSI shims elsewhere;
# on Unix it also wants these to avoid redefining Windows primitives.
if(WIN32)
  target_compile_definitions(freeimage PRIVATE WIN32)
endif()

add_library(FreeImage::FreeImage ALIAS freeimage)
