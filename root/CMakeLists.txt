cmake_minimum_required (VERSION 3.0...3.5)                                            
project(root CXX)                                                              
set(CMAKE_BUILD_TYPE Release)                                                   
include(CMakePackageConfigHelpers)
          
#Build parameter
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)
              
set(CMAKE_BINARY_OUTPUT_DIRECTORY  ${CMAKE_INSTALL_PREFIX}/bin )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib )
set(CMAKE_INSTALL_LIBDIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} )
set(CMAKE_INCLUDE_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/include )
### nptool ###
find_package(nptool QUIET)
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
### GEANT4 ###
find_package(Geant4 QUIET)
if(${Geant4_FOUND})
  message(STATUS "Geant4 found: ${Geant4_VERSION}" )
  # use this definition to isolate Geant4 dependent code
  add_compile_definitions(Geant4_FOUND)
  # add usefull root fonction for dictionnary definition
  include(${Geant4_USE_FILE})
  # add include directory
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
else()
  message(STATUS "Geant4 not found: compiling with no Geant4 support" )
endif()


set (duplicate_include_dir ${CMAKE_CURRENT_SOURCE_DIR}/src PARENT_SCOPE)
include_directories("src")
include_directories($ENV{NPTOOL_HOME}/$ENV{NPTOOL_ENV}/include)
include_directories($ENV{NPTOOL}/include)
add_subdirectory("src")

