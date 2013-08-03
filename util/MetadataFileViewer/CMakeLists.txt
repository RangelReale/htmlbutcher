set(SOURCES MetadataFileViewerApp.cpp MetadataFileViewerApp.h MetadataFileViewerMain.cpp MetadataFileViewerMain.h
	${HTMLButcher_SOURCE_DIR}/src/ButcherMetadataFile.cpp ${HTMLButcher_SOURCE_DIR}/src/ButcherMetadataFile.h )

#if (WIN32)
#	set(SOURCES ${SOURCES} resource.rc)
#endif (WIN32)

add_executable(MetadataFileViewer WIN32 MACOSX_BUNDLE ${SOURCES})

include_directories (${HTMLButcher_SOURCE_DIR}/src) 


target_link_libraries(MetadataFileViewer ${wxWidgets_LIBRARIES} ${CPPCOMP_LIBRARIES} ${Cryptopp_LIBRARIES})

set_target_properties(MetadataFileViewer PROPERTIES COMPILE_DEFINITIONS_DEBUG "HTMLBUTCHER_DEBUG;HTMLBUTCHER_KEEPOLDSAVE;__WXDEBUG__")