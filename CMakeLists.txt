cmake_minimum_required(VERSION 3.18)
project(HTMLButcher CXX C)

# CMP0135: URL downloads get the extraction timestamp, not the archive's.
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

# The code is C++11 (std::unique_ptr / std::shared_ptr throughout src/), but no
# standard used to be requested anywhere -- it relied on the compiler default.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(HB_WITH_DEMO  "Build DEMO version" OFF)
option(HB_WITH_UTILS "Build utils" OFF)

# Where the dependencies come from.
#
# On Linux the distro packages are the right answer: wxGTK needs GTK development
# packages present regardless, so a fetched wx would not make the build
# self-contained anyway. On Windows and macOS there is nothing to install from,
# so fetch and build wxWidgets and FreeImage from source.
if(UNIX AND NOT APPLE)
  option(HB_FETCH_DEPS "Download and build dependencies from source" OFF)
else()
  option(HB_FETCH_DEPS "Download and build dependencies from source" ON)
endif()

set(HB_WX_COMPONENTS core base aui html xrc xml stc)

# hb_wx carries wxWidgets to every target, however it was resolved.
add_library(hb_wx INTERFACE)

# =============================================================================
# Dependencies
#
# IMPORTANT: this block must stay ABOVE the add_definitions() calls and the
# CMAKE_*_OUTPUT_DIRECTORY settings further down. Both are directory-scoped and
# would otherwise leak into the fetched dependencies:
#
#   * -DUNICODE would reach FreeImage's ~15 bundled third-party C libraries
#     (zlib, libpng, libjpeg, libtiff, OpenEXR, LibRaw, ...), none of which are
#     UNICODE builds -- their Win32 TCHAR code would silently switch to the
#     wide-char APIs while still being handed char*.
#   * the output directories would send every dependency artifact into the
#     repository's own bin/ and lib/.
# =============================================================================
if(HB_FETCH_DEPS)
  include(FetchContent)

  # --- wxWidgets -------------------------------------------------------------
  # The release tarball is used rather than a git clone: it is far smaller and
  # already contains wx's bundled zlib/libpng/libjpeg/libtiff/expat/Scintilla,
  # which is what makes a Windows build self-contained.
  set(wxBUILD_SHARED       OFF CACHE BOOL "" FORCE)
  set(wxBUILD_SAMPLES      OFF CACHE BOOL "" FORCE)
  set(wxBUILD_TESTS        OFF CACHE BOOL "" FORCE)
  set(wxBUILD_DEMOS        OFF CACHE BOOL "" FORCE)
  set(wxBUILD_BENCHMARKS   OFF CACHE BOOL "" FORCE)
  set(wxUSE_STC            ON  CACHE BOOL "" FORCE)
  # src/ButcherFileDefs.h hard-errors on a non-Unicode build.
  set(wxUSE_UNICODE        ON  CACHE BOOL "" FORCE)

  FetchContent_Declare(wxwidgets
    URL      https://github.com/wxWidgets/wxWidgets/releases/download/v3.2.6/wxWidgets-3.2.6.tar.bz2
    URL_HASH SHA256=939e5b77ddc5b6092d1d7d29491fe67010a2433cf9b9c0d841ee4d04acb9dce7)

  FetchContent_MakeAvailable(wxwidgets)

  # --- FreeImage -------------------------------------------------------------
  # FreeImage ships no CMake build at all, so cmake/freeimage/CMakeLists.txt
  # supplies one. It is added as a subdirectory of *this* project, pointed at
  # the extracted sources through HB_FREEIMAGE_DIR, rather than being copied
  # into the extracted tree: a FetchContent PATCH_COMMAND only runs when the
  # source is first populated, so edits to our build script would silently not
  # be picked up on later configures.
  set(_fi_deps    "${CMAKE_BINARY_DIR}/_deps")
  set(_fi_src     "${_fi_deps}/freeimage-src")
  set(_fi_archive "${_fi_deps}/FreeImage3180.zip")

  if(NOT EXISTS "${_fi_src}/Makefile.srcs")
    message(STATUS "Downloading FreeImage 3.18.0")
    file(DOWNLOAD
      https://downloads.sourceforge.net/freeimage/FreeImage3180.zip
      "${_fi_archive}"
      EXPECTED_HASH SHA256=f41379682f9ada94ea7b34fe86bf9ee00935a3147be41b6569c9605a53e438fd
      SHOW_PROGRESS)
    file(ARCHIVE_EXTRACT INPUT "${_fi_archive}" DESTINATION "${_fi_deps}/freeimage-extract")
    # The archive holds a single top-level FreeImage/ directory.
    file(RENAME "${_fi_deps}/freeimage-extract/FreeImage" "${_fi_src}")
    file(REMOVE_RECURSE "${_fi_deps}/freeimage-extract")
  endif()

  set(HB_FREEIMAGE_DIR "${_fi_src}")
  add_subdirectory(cmake/freeimage "${CMAKE_BINARY_DIR}/freeimage-build")

  foreach(_comp IN LISTS HB_WX_COMPONENTS)
    target_link_libraries(hb_wx INTERFACE wx::${_comp})
  endforeach()
else()
  # --- system libraries ------------------------------------------------------
  find_package(wxWidgets 3.2 COMPONENTS ${HB_WX_COMPONENTS} REQUIRED)
  find_package(FreeImage REQUIRED)

  target_include_directories(hb_wx INTERFACE ${wxWidgets_INCLUDE_DIRS})
  target_link_libraries(hb_wx INTERFACE ${wxWidgets_LIBRARIES})
  target_compile_definitions(hb_wx INTERFACE ${wxWidgets_DEFINITIONS})
  if(wxWidgets_CXX_FLAGS)
    separate_arguments(_wx_cxx_flags NATIVE_COMMAND "${wxWidgets_CXX_FLAGS}")
    target_compile_options(hb_wx INTERFACE ${_wx_cxx_flags})
  endif()
endif()

# =============================================================================
# Project settings (directory-scoped -- deliberately after the dependencies)
# =============================================================================
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  add_definitions(-DUNICODE -D_UNICODE)
endif()

set(PROGNAME HTMLButcher)
if(APPLE)
  set(MACOSX_BUNDLE_ICON_FILE ${CMAKE_SOURCE_DIR}/resources/hbico_64.icns)
  set(MACOSX_BUNDLE_SHORT_VERSION_STRING 1.1.0.28)
  set(MACOSX_BUNDLE_VERSION 1.1.0.28)
  set(MACOSX_BUNDLE_LONG_VERSION_STRING 1.1.0.28)
  set(CMAKE_OSX_ARCHITECTURES x86_64)
endif()

include_directories(${CMAKE_SOURCE_DIR}/util)
set(CPPCOMP_LIBRARIES hbcppcomp)

add_subdirectory(src)
add_subdirectory(util)

if(APPLE)
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "HTMLButcher")
  set(CMAKE_INSTALL_PREFIX /Applications)
  set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
  set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
  set(CPACK_PACKAGE_VENDOR "SIB IT")
  set(CPACK_PACKAGE_VERSION_MAJOR "1")
  set(CPACK_PACKAGE_VERSION_MINOR "1")
  set(CPACK_PACKAGE_VERSION_PATCH "28")

  include(CPack)
endif()
