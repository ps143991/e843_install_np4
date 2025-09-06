add_library(NPMust2 SHARED Must2Detector.cxx Must2Data.cxx Must2Physics.cxx Must2Spectra.cxx)
target_link_libraries(NPMust2 PUBLIC PUBLIC ROOT::RIO ROOT::Tree ROOT::Physics ROOT::Imt ROOT::Hist ROOT::Gpad NPRoot nptool )

if(MFM_FOUND)
  target_compile_definitions(NPMust2 PUBLIC MFM_FOUND)
  target_link_libraries(NPMust2 PUBLIC ${MFMCOMPLIB})
endif()

if(Geant4_FOUND)
  add_library(NPMust2Geant4 SHARED Must2Geant4.h Must2Geant4.cxx)
  target_link_libraries(NPMust2Geant4 PUBLIC NPMust2 NPGeant4 nptool ${Geant4_LIBRARIES})
endif()

install(DIRECTORY ./ DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY} FILES_MATCHING PATTERN "*.h" )
ROOT_GENERATE_DICTIONARY(Must2Dict Must2Data.h Must2Physics.h Must2Spectra.h LINKDEF Must2linkdef.hh MODULE NPMust2)
