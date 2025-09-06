add_library(NPG4Beam SHARED NPG4Beam.cxx NPG4Beam.h)
target_link_libraries(NPG4Beam PUBLIC ROOT::RIO ROOT::Tree ROOT::Physics ROOT::Imt ROOT::Hist ROOT::Net ROOT::RHTTP NPPhysicsEssential nptool NPRoot NPGeant4 ${Geant4_LIBRARIES} ) 
install(DIRECTORY ./ DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY} FILES_MATCHING PATTERN "*.h" )
