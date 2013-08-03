set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH} )

cmake_minimum_required(VERSION 2.6)
project(HTMLButcher)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

IF (WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(-DUNICODE -D_UNICODE)
ENDIF (WIN32)

IF( APPLE )
  set( PROGNAME HTMLButcher )
  set( MACOSX_BUNDLE_ICON_FILE ${CMAKE_SOURCE_DIR}/resources/hbico_64.icns )
  set( MACOSX_BUNDLE_SHORT_VERSION_STRING 1.1.0.27 )
  set( MACOSX_BUNDLE_VERSION 1.1.0.25 )
  set( MACOSX_BUNDLE_LONG_VERSION_STRING 1.1.0.27 )
  set( CMAKE_OSX_ARCHITECTURES x86_64 )
ELSE( NOT APPLE )
  set( PROGNAME HTMLButcher )
ENDIF( APPLE )

find_package(FreeImage REQUIRED)
include_directories(${FREEIMAGE_INCLUDE_PATH})

include_directories(${CMAKE_SOURCE_DIR}/util)
set(CPPCOMP_LIBRARIES hbcppcomp)

if (UNIX)
	if (NOT APPLE)
		FIND_PACKAGE(GTK2 REQUIRED gtk)
  		include_directories(${GTK2_INCLUDE_DIRS})
		add_definitions(${GTK2_DEFINITIONS})	
	endif (NOT APPLE)
endif (UNIX)


find_package(wxWidgets COMPONENTS core base aui html xrc adv xml REQUIRED)
INCLUDE(${wxWidgets_USE_FILE})
#include_directories(${wxWidgets_INCLUDE_DIRS} ${wxWidgets_ROOT_DIR}/contrib/include)
include_directories(${wxWidgets_ROOT_DIR}/contrib/include)
#if (NOT WIN32)
#	add_definitions(${wxWidgets_DEFINITIONS} ${wxWidgets_CXX_FLAGS})
#endif (NOT WIN32)

find_package(wxstedit)
if (wxstedit_FOUND)
	include_directories(${wxstedit_INCLUDE_PATH})
endif (wxstedit_FOUND)





include_directories (${HTMLButcher_SOURCE_DIR}/secure) 



add_subdirectory(src)
add_subdirectory(util)

