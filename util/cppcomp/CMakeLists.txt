set(SOURCES ccufile.cpp ccustring.cpp wxccu_control.cpp wxccu_file.cpp wxccu_string.cpp)

set(INCS ccufile.h ccustring.h ccudefs.h wxccucontrol.h wxccufile.h wxccustring.h)


add_library(hbcppcomp STATIC ${SOURCES} ${INCS})

#INSTALL(FILES ${INCS} DESTINATION include/cppcomp)

#INSTALL(TARGETS ccutil ARCHIVE DESTINATION lib)
