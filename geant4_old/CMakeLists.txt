cmake_minimum_required (VERSION 3.0...3.5)                                            
project(NPgeant4 CXX)                                                              
set(CMAKE_BUILD_TYPE Release)                                                   
include(CMakePackageConfigHelpers)
          
#Build parameter
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)
              
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_INSTALL_PREFIX}/bin )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib )
set(CMAKE_INCLUDE_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/include )
set(CMAKE_INSTALL_LIBDIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} )

### nptool ###
find_package(nptool QUIET)
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

### Geant4 ###
find_package(Geant4 REQUIRED ui_all vis_all)
if(${Geant4_FOUND})
  message(STATUS "Geant4 found: ${Geant4_VERSION}" )
  # use this definition to isolate Geant4 dependent code
  add_compile_definitions(Geant4_FOUND)
  include(${Geant4_USE_FILE})
  add_definitions(${Geant4_DEFINITIONS})
  include_directories(${Geant4_INCLUDE_DIRS})
else()
  message(STATUS "Geant4 not found: cannot compile!" )
endif()


set (duplicate_include_dir ${CMAKE_CURRENT_SOURCE_DIR}/core ${CMAKE_CURRENT_SOURCE_DIR}/beam ${CMAKE_CURRENT_SOURCE_DIR}/isotropic-source ${CMAKE_CURRENT_SOURCE_DIR}/base-physics ${CMAKE_CURRENT_SOURCE_DIR}/target PARENT_SCOPE)
include_directories(BEFORE core)
include_directories(AFTER $ENV{NPTOOL_HOME}/$ENV{NPTOOL_ENV}/include)
include_directories(AFTER $ENV{NPTOOL}/include)

add_subdirectory(core)
add_subdirectory(isotropic-source)
add_subdirectory(beam)
add_subdirectory(base-physics)
add_subdirectory(target)

