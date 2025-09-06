if(${ROOT_FOUND})
ROOT_GENERATE_DICTIONARY(NPRootDict  NPRootHTMLUserInterface.h NPRootAsciiFile.h NPRootTreeServer.h NPRootTreeClient.h LINKDEF NPRootlinkdef.hh MODULE NPRoot)
add_library(NPRoot SHARED NPRootDict NPRootHTMLUserInterface.h NPRootHTMLUserInterface.cxx NPRootPlugin.cxx NPRootPlugin.h NPRootInput.cxx NPRootInput.h NPRootOutput.cxx NPRootOutput.h NPRootAsciiFile.cxx NPRootAsciiFile.h NPRootTreeServer.cxx NPRootTreeServer.h NPRootTreeClient.cxx NPRootTreeClient.h) 
  target_link_libraries(NPRoot  PUBLIC ROOT::RIO ROOT::Tree ROOT::Physics ROOT::Imt ROOT::Hist ROOT::Net ROOT::RHTTP nptool )
install(DIRECTORY ./ DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY} FILES_MATCHING PATTERN "*.h" )
endif()

