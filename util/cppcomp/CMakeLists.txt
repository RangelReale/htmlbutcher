set(SOURCES ccufile.cpp ccustring.cpp wxccu_control.cpp wxccu_file.cpp wxccu_string.cpp)

set(INCS ccufile.h ccustring.h ccudefs.h wxccucontrol.h wxccufile.h wxccustring.h)


add_library(hbcppcomp STATIC ${SOURCES} ${INCS})

# wxccucontrol.h includes <wx/wx.h>, so this needs wxWidgets. It used to get
# the include paths only as a side effect of the top-level
# INCLUDE(${wxWidgets_USE_FILE}); hb_wx now carries them explicitly.
target_link_libraries(hbcppcomp PUBLIC hb_wx)

#INSTALL(FILES ${INCS} DESTINATION include/cppcomp)

#INSTALL(TARGETS ccutil ARCHIVE DESTINATION lib)
