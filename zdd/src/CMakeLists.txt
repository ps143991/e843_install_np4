add_library(NPZdd SHARED ZddDetector.cxx ZddData.cxx ZddPhysics.cxx ZddSpectra.cxx)
target_link_libraries(NPZdd PUBLIC PUBLIC ROOT::RIO ROOT::Tree ROOT::Physics ROOT::Gpad ROOT::Imt ROOT::Hist NPRoot nptool )

if(MFM_FOUND)
  target_compile_definitions(NPZdd PUBLIC MFM_FOUND)
  target_link_libraries(NPZdd PUBLIC ${MFMCOMPLIB})
endif()

install(DIRECTORY ./ DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY} FILES_MATCHING PATTERN "*.h" )
ROOT_GENERATE_DICTIONARY(ZddDict ZddData.h ZddPhysics.h ZddSpectra.h LINKDEF Zddlinkdef.hh MODULE NPZdd)
