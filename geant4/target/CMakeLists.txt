include_directories(../core)

add_library(NPTarget SHARED NPTarget.cxx NPTarget.h)
target_link_libraries(NPTarget PUBLIC ROOT::RIO ROOT::Tree ROOT::Physics ROOT::Imt ROOT::Hist ROOT::Net ROOT::RHTTP NPPhysicsEssential nptool NPRoot ${Geant4_LIBRARIES} ) 

add_library(NPG4Target SHARED NPG4Target.cxx NPG4Target.h)
target_link_libraries(NPG4Target PUBLIC ROOT::RIO ROOT::Tree ROOT::Physics ROOT::Imt ROOT::Hist ROOT::Net ROOT::RHTTP NPPhysicsEssential nptool NPRoot NPGeant4 NPTarget ${Geant4_LIBRARIES} ) 

install(DIRECTORY ./ DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY} FILES_MATCHING PATTERN "*.h" )
