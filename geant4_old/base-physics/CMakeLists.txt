include_directories(../core)
add_library(NPG4BasePhysicsList SHARED NPG4BasePhysicsList.cxx NPG4BasePhysicsList.h)
target_link_libraries(NPG4BasePhysicsList PUBLIC ROOT::RIO ROOT::Tree ROOT::Physics ROOT::Imt ROOT::Hist ROOT::Net ROOT::RHTTP NPPhysicsEssential nptool NPRoot NPGeant4 ${Geant4_LIBRARIES} ) 
install(DIRECTORY ./ DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY} FILES_MATCHING PATTERN "*.h" )
