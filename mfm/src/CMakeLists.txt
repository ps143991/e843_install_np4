add_library(NPMfm SHARED NPMFMInput.cxx NPMFMInput.h) 
include_directories(${INCLUDE_DIR})
message(STATUS ${MFMCOMPLIB})

target_compile_definitions(NPMfm PUBLIC MFM_FOUND)
target_compile_definitions(NPMfm PUBLIC MFM_FOUND)
if(ZMQ_FOUND)
  target_link_libraries(NPMfm PUBLIC nptool ${ZMQ_LIB_LINK} ${COMPLIB})
else()
  target_link_libraries(NPMfm PUBLIC nptool ${COMPLIB})
endif()

install(DIRECTORY ./ DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY} FILES_MATCHING PATTERN "*.h" )




