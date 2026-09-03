#
# Find a system-installed FreeImage.
#
# Only used when HB_FETCH_DEPS=OFF (the default on Linux, where FreeImage comes
# from the distro -- libfreeimage-dev on Debian/Ubuntu). The fetch path builds
# FreeImage from source instead; see cmake/freeimage/CMakeLists.txt.
#
# Defines the imported target
#   FreeImage::FreeImage
# and, for compatibility, the variables
#   FreeImage_FOUND / FREEIMAGE_FOUND
#   FREEIMAGE_INCLUDE_PATH
#   FREEIMAGE_LIBRARY / FREEIMAGE_LIBRARIES
#
# Note this deliberately does NOT define FREEIMAGE_LIB: a system FreeImage is a
# shared library that registers its format plugins from its own library
# constructor. That definition is only correct for a static build, and the
# fetched target exports it itself.

find_path(FREEIMAGE_INCLUDE_PATH FreeImage.h
  HINTS
    ENV FREEIMAGE
  PATHS
    "$ENV{PROGRAMFILES}/FreeImage/include"
    "$ENV{FREEIMAGE}/Dist"
    "${PROJECT_SOURCE_DIR}/FreeImage/include"
  PATH_SUFFIXES Dist include
  DOC "The directory containing FreeImage.h")

find_library(FREEIMAGE_LIBRARY
  NAMES freeimage FreeImage
  HINTS
    ENV FREEIMAGE
  PATHS
    "$ENV{PROGRAMFILES}/FreeImage/lib"
    "$ENV{FREEIMAGE}/Dist"
    "${PROJECT_SOURCE_DIR}/FreeImage/bin"
    "${PROJECT_SOURCE_DIR}/FreeImage/lib"
  PATH_SUFFIXES Dist lib
  DOC "The FreeImage library")

# Using the standard handler is what makes find_package(FreeImage REQUIRED)
# actually fail when FreeImage is missing. The previous version of this module
# set a plain cache variable instead, so REQUIRED was inert and an absent
# FreeImage only surfaced later as a link error.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FreeImage
  REQUIRED_VARS FREEIMAGE_LIBRARY FREEIMAGE_INCLUDE_PATH)

if(FreeImage_FOUND)
  set(FREEIMAGE_LIBRARIES ${FREEIMAGE_LIBRARY})

  if(NOT TARGET FreeImage::FreeImage)
    add_library(FreeImage::FreeImage UNKNOWN IMPORTED)
    set_target_properties(FreeImage::FreeImage PROPERTIES
      IMPORTED_LOCATION "${FREEIMAGE_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${FREEIMAGE_INCLUDE_PATH}")
  endif()
endif()

mark_as_advanced(FREEIMAGE_INCLUDE_PATH FREEIMAGE_LIBRARY)
