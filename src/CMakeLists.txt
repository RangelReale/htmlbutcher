set(HB_CDEF "")
set(HBDEMO_CDEF HTMLBUTCHER_DEMO)


include_directories(${HTMLButcher_SOURCE_DIR}/src)
include_directories(${HTMLButcher_SOURCE_DIR}/src/dialogs)
include_directories(${HTMLButcher_SOURCE_DIR}/src/project)
include_directories(${HTMLButcher_SOURCE_DIR}/src/view)

#main
set(SOURCES_MAIN HTMLButcherApp.cpp HTMLButcherApp.h HTMLButcherMain.cpp HTMLButcherMain.h HTMLButcherSplash.cpp HTMLButcherSplash.h HTMLButcherVersion.h )

set(SOURCES ${SOURCES_MAIN})
SOURCE_GROUP(Main FILES ${SOURCES_MAIN})

# dialog
set(SOURCES_DIALOG dialogs/DialogAbout.cpp dialogs/DialogAbout.h dialogs/DialogAdjustSize.cpp 
dialogs/DialogAdjustSize.h dialogs/DialogAssortedFileEdit.cpp dialogs/DialogAssortedFileEdit.h 
dialogs/DialogCSSFileEdit.cpp dialogs/DialogCSSFileEdit.h dialogs/DialogFileEdit.cpp 
dialogs/DialogFileEdit.h dialogs/DialogFileLink.cpp dialogs/DialogFileLink.h dialogs/DialogFilePathEdit.cpp 
dialogs/DialogFilePathEdit.h dialogs/DialogImageFormatEdit.cpp dialogs/DialogImageFormatEdit.h 
dialogs/DialogListEdit.cpp dialogs/DialogListEdit.h dialogs/DialogMaskAreaEdit.cpp 
dialogs/DialogMaskAreaEdit.h dialogs/DialogMaskAreaFileAlternate.cpp dialogs/DialogMaskAreaFileAlternate.h 
dialogs/DialogMaskAreaGlobalEdit.cpp dialogs/DialogMaskAreaGlobalEdit.h dialogs/DialogMaskAreaImageFormat.cpp 
dialogs/DialogMaskAreaImageFormat.h dialogs/DialogMaskAreaListEdit.cpp dialogs/DialogMaskAreaListEdit.h 
dialogs/DialogMaskAreaMapEdit.cpp dialogs/DialogMaskAreaMapEdit.h dialogs/DialogMaskAreaSlice.cpp 
dialogs/DialogMaskAreaSlice.h dialogs/DialogMaskEdit.cpp dialogs/DialogMaskEdit.h dialogs/DialogOptions.cpp 
dialogs/DialogOptions.h dialogs/DialogProgress.cpp dialogs/DialogProgress.h dialogs/DialogProjectOptions.cpp 
dialogs/DialogProjectOptions.h dialogs/DialogViewEdit.cpp dialogs/DialogViewEdit.h dialogs/PopupCSSEdit.cpp 
dialogs/PopupCSSEdit.h dialogs/WizNewView.cpp dialogs/WizNewView.h  )

set(SOURCES ${SOURCES} ${SOURCES_DIALOG})
SOURCE_GROUP(Dialogs FILES ${SOURCES_DIALOG})

# project
set(SOURCES_PROJECT project/ButcherViewBase.cpp project/ButcherViewBase.h project/ButcherProjectEvent.cpp 
project/ButcherProjectEvent.h project/ButcherProjectEventNotify.h project/ButcherOutputFile.h
project/ButcherProjectBase.h project/ButcherProjectBaseModify.h project/ButcherProjectBaseModify.cpp
project/ButcherProjectBaseBLId.h project/ButcherProjectBaseAutoUpdate.cpp project/ButcherProjectBaseAutoUpdate.h
project/ButcherProjectBaseAutoDisable.cpp project/ButcherProjectBaseAutoDisable.h project/ButcherProjectBaseAutoProgress.cpp
project/ButcherProjectBaseAutoProgress.h project/ButcherProjectBaseIDList.h project/ButcherProjectColorScheme.cpp
project/ButcherProjectColorScheme.h project/ButcherProjectColorSchemes.cpp project/ButcherProjectColorSchemes.h
project/ButcherProjectImageFormat.cpp project/ButcherProjectImageFormat.h project/ButcherProjectImageFormats.cpp
project/ButcherProjectImageFormats.h project/ButcherProjectLine.cpp project/ButcherProjectLine.h project/ButcherProjectLineDefault.cpp
project/ButcherProjectLineDefault.h project/ButcherProjectLineStart.cpp project/ButcherProjectLineStart.h
project/ButcherProjectLineEnd.cpp project/ButcherProjectLineEnd.h project/ButcherProjectFileLink.cpp
project/ButcherProjectFileLink.h project/ButcherProjectTemplateParser.cpp project/ButcherProjectTemplateParser.h
project/ButcherProjectAreaImageInfo.cpp project/ButcherProjectAreaImageInfo.h project/ButcherProjectAreaConfigPreview.cpp
project/ButcherProjectAreaConfigPreview.h project/ButcherProjectAreaConfigBase.cpp project/ButcherProjectAreaConfigBase.h
project/ButcherProjectAreaConfig.cpp project/ButcherProjectAreaConfig.h project/ButcherProjectAreaConfigs.cpp
project/ButcherProjectAreaConfigs.h project/ButcherProjectArea.cpp project/ButcherProjectArea.h
project/ButcherProjectMaskLayout.cpp project/ButcherProjectMaskLayout.h project/ButcherProjectMaskSelect.h
project/ButcherProjectLines.cpp project/ButcherProjectLines.h project/ButcherProjectAreas.cpp project/ButcherProjectAreas.h
project/ButcherProjectMaskContainer.cpp project/ButcherProjectMaskContainer.h project/ButcherProjectMaskContainerJoined.cpp
project/ButcherProjectMaskContainerJoined.h project/ButcherProjectMaskContainerSplit.cpp project/ButcherProjectMaskContainerSplit.h
project/ButcherProjectMaskContainerArea.cpp project/ButcherProjectMaskContainerArea.h project/ButcherProjectMaskContainerAreaGlobal.cpp
project/ButcherProjectMaskContainerAreaGlobal.h project/ButcherProjectMaskContainerAreaMap.cpp project/ButcherProjectMaskContainerAreaMap.h
project/ButcherProjectMask.cpp project/ButcherProjectMask.h project/ButcherProjectMaskDrawSelection.cpp project/ButcherProjectMaskDrawSelection.h
project/ButcherProjectMaskRoot.cpp project/ButcherProjectMaskRoot.h project/ButcherProjectMaskInner.cpp project/ButcherProjectMaskInner.h
project/ButcherProjectFilePath.cpp project/ButcherProjectFilePath.h project/ButcherProjectFile.cpp project/ButcherProjectFile.h
project/ButcherProjectViewFileCheck.cpp project/ButcherProjectViewFileCheck.h project/ButcherProjectView.cpp
project/ButcherProjectView.h project/ButcherProjectFilePaths.cpp project/ButcherProjectFilePaths.h project/ButcherProjectFiles.cpp
project/ButcherProjectFiles.h project/ButcherProjectMasks.cpp project/ButcherProjectMasks.h project/ButcherProjectViews.cpp
project/ButcherProjectViews.h project/ButcherProjectAssortedFileGroup.cpp project/ButcherProjectAssortedFileGroup.h
project/ButcherProjectAssortedFileGroups.cpp project/ButcherProjectAssortedFileGroups.h project/ButcherProjectAssortedFile.cpp
project/ButcherProjectAssortedFile.h project/ButcherProjectAssortedFiles.cpp project/ButcherProjectAssortedFiles.h
project/ButcherProjectCSSFile.cpp project/ButcherProjectCSSFile.h project/ButcherProjectCSSFiles.cpp project/ButcherProjectCSSFiles.h
project/ButcherProjectOptions.cpp project/ButcherProjectOptions.h project/ButcherProjectMetadataFile.cpp project/ButcherProjectMetadataFile.h
project/ButcherProject.cpp project/ButcherProject.h )

set(SOURCES ${SOURCES} ${SOURCES_PROJECT}  )
SOURCE_GROUP(Project FILES ${SOURCES_PROJECT})

# view
set(SOURCES_VIEW view/ButcherRuler.cpp view/ButcherRuler.h view/ButcherDocument.cpp view/ButcherDocument.h view/ButcherDocumentMouseEvent.cpp
view/ButcherDocumentMouseEvent.h view/ButcherDocumentKeyEvent.cpp view/ButcherDocumentKeyEvent.h 
view/ButcherDocumentDrawEvent.cpp view/ButcherDocumentDrawEvent.h view/ButcherViewSelectEvent.cpp
view/ButcherViewSelectEvent.h view/ButcherStatusEvent.cpp view/ButcherStatusEvent.h view/ButcherViewChangedEvent.cpp view/ButcherViewChangedEvent.h
view/ButcherView.cpp view/ButcherView.h view/ButcherViewEditor.cpp view/ButcherViewEditor.h view/ButcherViewDisplay.cpp view/ButcherViewDisplay.h )

set(SOURCES ${SOURCES} ${SOURCES_VIEW}  )
SOURCE_GROUP(View FILES ${SOURCES_VIEW})


# utils
set(SOURCES_UTILS BControls.cpp BControls.h BConsts.h BExcept.h BHTML.cpp BHTML.h BImage.cpp BImage.h BImage_FreeImage.cpp BImage_FreeImage.h BImageView.cpp BImageView.h BLanguage.cpp BLanguage.h BList.h BProc.cpp BProc.h ButcherFileDefs.h ButcherMetadataFile.cpp ButcherMetadataFile.h BUtil.cpp BUtil.h wxFreeImage.cpp wxFreeImage.h wxpopupmenu.cpp wxpopupmenu.h ../resources/htmlbutcher_resources.cpp )

set(SOURCES ${SOURCES} ${SOURCES_UTILS})
SOURCE_GROUP(Utils FILES ${SOURCES_UTILS})

# resources
if (WIN32)
	set(SOURCES_RESOURCES resource.rc )

	set(SOURCES ${SOURCES} ${SOURCES_RESOURCES})
	SOURCE_GROUP(Resources FILES ${SOURCES_RESOURCES})
endif(WIN32)

add_executable(HTMLButcher WIN32 MACOSX_BUNDLE ${SOURCES})
add_executable(HTMLButcherDemo WIN32 MACOSX_BUNDLE ${SOURCES} dialogs/DialogDemo.cpp dialogs/DialogDemo.h)

SOURCE_GROUP(Demo FILES dialogs/DialogDemo.cpp dialogs/DialogDemo.h)


target_link_libraries(HTMLButcher ${FREEIMAGE_LIBRARIES} ${CPPCOMP_LIBRARIES} ${wxWidgets_LIBRARIES} )
target_link_libraries(HTMLButcherDemo ${FREEIMAGE_LIBRARIES} ${CPPCOMP_LIBRARIES} ${wxWidgets_LIBRARIES})

#if (wxstedit_FOUND)
#	add_definitions(-DBUTCHER_USE_STEDIT)
#endif (wxstedit_FOUND)


#set_target_properties(HTMLButcherDemo PROPERTIES COMPILE_DEFINITIONS HTMLBUTCHER_DEMO)

if (wxstedit_FOUND)
	set(HB_CDEF "${HB_CDEF};BUTCHER_USE_STEDIT")
	set(HBDEMO_CDEF "${HBDEMO_CDEF};BUTCHER_USE_STEDIT")

	#set_target_properties(HTMLButcher PROPERTIES COMPILE_DEFINITIONS BUTCHER_USE_STEDIT)
	#set_target_properties(HTMLButcherDemo PROPERTIES COMPILE_DEFINITIONS BUTCHER_USE_STEDIT)
	
	target_link_libraries(HTMLButcher ${wxstedit_LIBRARIES} debug ${wxWidgets_LIB_DIR}/wxmsw28ud_stc.lib optimized ${wxWidgets_LIB_DIR}/wxmsw28u_stc.lib)
	target_link_libraries(HTMLButcherDemo ${wxstedit_LIBRARIES} debug ${wxWidgets_LIB_DIR}/wxmsw28ud_stc.lib optimized ${wxWidgets_LIB_DIR}/wxmsw28u_stc.lib)
endif(wxstedit_FOUND)

if (UNIX)
	if (GTK_FOUND)
		TARGET_LINK_LIBRARIES(HTMLButcher ${GTK_LIBRARIES})
		TARGET_LINK_LIBRARIES(HTMLButcherDemo ${GTK_LIBRARIES})
	endif (GTK_FOUND)
endif (UNIX)


set_target_properties(HTMLButcher PROPERTIES COMPILE_DEFINITIONS "${HB_CDEF}")
set_target_properties(HTMLButcherDemo PROPERTIES COMPILE_DEFINITIONS "${HBDEMO_CDEF}")

set_target_properties(HTMLButcher PROPERTIES COMPILE_DEFINITIONS_DEBUG "${HB_CDEF};HTMLBUTCHER_DEBUG;HTMLBUTCHER_KEEPOLDSAVE;__WXDEBUG__")
set_target_properties(HTMLButcherDemo PROPERTIES COMPILE_DEFINITIONS_DEBUG "${HBDEMO_CDEF};HTMLBUTCHER_DEBUG;HTMLBUTCHER_KEEPOLDSAVE;__WXDEBUG__")

set(APPS HTMLButcher)  # paths to executables
set(DIRS )   # directories to search for prerequisites
INSTALL(CODE "
   include(BundleUtilities)
   fixup_bundle(\"${APPS}\"   \"\"   \"${DIRS}\")
   " COMPONENT Runtime)

