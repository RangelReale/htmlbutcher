#
# Try to find the wxstedit library and include path.
# Once done this will define
#
# wxstedit_FOUND
# wxstedit_INCLUDE_PATH
# wxstedit_LIBRARY
#

IF (WIN32)
  FIND_PATH( wxstedit_INCLUDE_PATH wx/stedit/stedit.h
    $ENV{PROGRAMFILES}/wxstedit/include
    $ENV{wxstedit}/include
    ${PROJECT_SOURCE_DIR}/wxstedit/include
    DOC "The directory where stedit.h resides")
  FIND_LIBRARY( wxstedit_LIBRARY
    NAMES wxcode_msw28u_stedit
    PATHS
    $ENV{PROGRAMFILES}/wxstedit/lib
    $ENV{wxstedit}/lib/vc_lib
    ${PROJECT_SOURCE_DIR}/wxstedit/bin
    ${PROJECT_SOURCE_DIR}/wxstedit/lib
    DOC "The wxstedit library")
  FIND_LIBRARY( wxsteditD_LIBRARY
    NAMES wxcode_msw28ud_stedit
    PATHS
    $ENV{PROGRAMFILES}/wxstedit/lib
    $ENV{wxstedit}/lib/vc_lib
    ${PROJECT_SOURCE_DIR}/wxstedit/bin
    ${PROJECT_SOURCE_DIR}/wxstedit/lib
    DOC "The wxsteditD library")
ELSE (WIN32)
  FIND_PATH( wxstedit_INCLUDE_PATH GL/glew.h
    /usr/include
    /usr/local/include
    /sw/include
    /opt/local/include
    DOC "The directory where ccudefs.h resides")
  FIND_LIBRARY( wxstedit_LIBRARY
    NAMES wxcode_msw28u_stedit
    PATHS
    /usr/lib64
    /usr/lib
    /usr/local/lib64
    /usr/local/lib
    /sw/lib
    /opt/local/lib
    DOC "The wxstedit library")
ENDIF (WIN32)

SET(wxstedit_LIBRARIES debug ${wxsteditD_LIBRARY} optimized ${wxstedit_LIBRARY})

IF (wxstedit_INCLUDE_PATH)
  SET( wxstedit_FOUND TRUE CACHE BOOL "Set to TRUE if wxstedit is found, FALSE otherwise")
ELSE (wxstedit_INCLUDE_PATH)
  SET( wxstedit_FOUND FALSE CACHE BOOL "Set to TRUE if wxstedit is found, FALSE otherwise")
ENDIF (wxstedit_INCLUDE_PATH)

MARK_AS_ADVANCED(
  wxstedit_FOUND
  wxstedit_LIBRARY
  wxstedit_LIBRARIES
  wxstedit_INCLUDE_PATH)