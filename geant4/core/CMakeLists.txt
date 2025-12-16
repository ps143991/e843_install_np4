add_library(NPGeant4 SHARED NPG4Session.cxx NPG4RunAction.h NPG4RunAction.cxx NPG4EventAction.h NPG4EventAction.cxx NPG4VDetector.h NPG4VDetector.cxx NPG4VPrimaryGenerator.h NPG4DetectorConstruction.cxx NPG4VPhysicsList.cxx NPG4PrimaryGeneratorAction.cxx NPG4MaterialManager.cxx NPG4VBeamProcess.cxx)
set(duplicate_include_dir ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)

include_directories(AFTER $ENV{NPTOOL_HOME}/$ENV{NPTOOL_ENV}/include)
include_directories(AFTER $ENV{NPTOOL}/include)
# Create a library for the InteractionCoordinates dictionary
add_library(InteractionCoordinates SHARED
    InteractionCoordinates.cxx
    # InteractionCoordinatesDict.cxx
    InteractionCoordinates.h
)
# target_link_libraries(InteractionCoordinates PUBLIC PUBLIC ROOT::RIO ROOT::Tree ROOT::Physics ROOT::Imt NPRoot nptool PRIVATE ROOT::Core ${ROOT_LIBRARIES})
target_link_libraries(InteractionCoordinates PUBLIC ROOT::RIO ROOT::Tree ROOT::Physics ROOT::Imt NPRoot nptool)

target_link_libraries(NPGeant4 nptool ${Geant4_LIBRARIES} InteractionCoordinates) 
add_executable(npgeant4 npgeant4.cxx) 
target_link_libraries(npgeant4 nptool NPGeant4 ${Geant4_LIBRARIES}) 
install(DIRECTORY ./ DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY} FILES_MATCHING PATTERN "*.h" )

# Generate the ROOT dictionary for InteractionCoordinates
ROOT_GENERATE_DICTIONARY(
    InteractionCoordinatesDict
    InteractionCoordinates.h
    LINKDEF InteractionCoordinateslinkdef.hh
    MODULE InteractionCoordinates
)

