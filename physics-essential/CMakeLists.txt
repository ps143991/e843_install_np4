cmake_minimum_required (VERSION 3.0...3.5)                            
project(physics-essential CXX)                                                              
set(CMAKE_BUILD_TYPE Release)                                                   
include(CMakePackageConfigHelpers)
          
#Build parameter 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_INSTALL_PREFIX}/bin )   
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib )   
set(CMAKE_INSTALL_LIBDIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} )
set(CMAKE_INCLUDE_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/include ) 


### nptool ###
find_package(nptool QUIET)

include_directories(AFTER $ENV{NPTOOL_HOME}/$ENV{NPTOOL_ENV}/include)
include_directories(AFTER $ENV{NPTOOL}/include)

link_directories($ENV{NPTOOL}/lib)
link_directories(../install/lib)
link_directories($ENV{NPTOOL_HOME}/$ENV{NPTOOL_ENV}/lib)

### ROOT ###
# find package 
find_package(ROOT QUIET)
if(${ROOT_FOUND})
  message(STATUS "ROOT found: ${ROOT_VERSION}" )
  # use this definition to isolate ROOT dependent code
  add_compile_definitions(ROOT_FOUND)
  # add usefull root fonction for dictionnary definition
  include(${ROOT_USE_FILE})
  # add include directory
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
else()
  message(STATUS "ROOT not found: compiling with no ROOT support" )
endif()
#find_package(Geant4 QUIET)
#if(${Geant4_FOUND})
#  message(STATUS "Geant4 found: ${Geant4_VERSION}" )
#  # use this definition to isolate Geant4 dependent code
#  add_compile_definitions(Geant4_FOUND)
#  # add usefull root fonction for dictionnary definition
#  include(${Geant4_USE_FILE})
#  # add include directory
#  include_directories(${Geant4_INCLUDE_DIRS})
#else()
#  message(STATUS "Geant4 not found: compiling with no Geant4 support" )
#endif()

include_directories(./)

set (duplicate_include_dir ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
ROOT_GENERATE_DICTIONARY(PhysicsEssentialDict NPBeam.h NPParticle.h NPReaction.h NPEnergyLoss.h DEPENDENCIES NPBeam.h NPParticle.h NPReaction.h NPEnergyLoss.h MODULE NPPhysicsEssential LINKDEF PhysicsEssentialLinkdef.h)

add_library(NPPhysicsEssential SHARED PhysicsEssentialDict NPBeam.cxx NPBeam.h NPParticle.cxx NPParticle.h NPReaction.cxx NPReaction.h NPEnergyLoss.cxx NPEnergyLoss.h )

target_link_libraries(NPPhysicsEssential PUBLIC ROOT::RIO ROOT::Tree ROOT::Physics ROOT::Imt ROOT::Hist ROOT::Gpad NPRoot nptool )
install(DIRECTORY ./ DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY} FILES_MATCHING PATTERN "*.h" )

