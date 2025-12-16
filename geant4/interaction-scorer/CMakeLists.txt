# cmake_minimum_required(VERSION 3.0...3.5)
# project(NPG4InteractionScorer CXX)
# set(CMAKE_BUILD_TYPE Release)
# include(CMakePackageConfigHelpers)

# # Build parameter
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_BUILD_TYPE Release)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_INSTALL_PREFIX}/bin)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib)
# set(CMAKE_INCLUDE_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/include)
# set(CMAKE_INSTALL_LIBDIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# ### nptool ###
# find_package(nptool QUIET)
# link_directories($ENV{NPTOOL}/lib)
# link_directories(../install/lib)
# link_directories($ENV{NPTOOL_HOME}/$ENV{NPTOOL_ENV}/lib)

# ### ROOT ###
# # Find package
# find_package(ROOT QUIET)
# if(${ROOT_FOUND})
#   message(STATUS "ROOT found: ${ROOT_VERSION}")
#   # Use this definition to isolate ROOT-dependent code
#   add_compile_definitions(ROOT_FOUND)
#   # Add useful ROOT functions for dictionary definition
#   include(${ROOT_USE_FILE})
#   # Add include directory
#   include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
# else()
#   message(STATUS "ROOT not found: compiling with no ROOT support")
# endif()

# ### Geant4 ###
# find_package(Geant4 REQUIRED ui_all vis_all)
# if(${Geant4_FOUND})
#   message(STATUS "Geant4 found: ${Geant4_VERSION}")
#   # Use this definition to isolate Geant4-dependent code
#   add_compile_definitions(Geant4_FOUND)
#   include(${Geant4_USE_FILE})
#   add_definitions(${Geant4_DEFINITIONS})
#   include_directories(${Geant4_INCLUDE_DIRS})
# else()
#   message(STATUS "Geant4 not found: cannot compile!")
# endif()

# set(duplicate_include_dir ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
# include_directories(AFTER $ENV{NPTOOL_HOME}/$ENV{NPTOOL_ENV}/include)
# include_directories(AFTER $ENV{NPTOOL}/include)

# # Create a library for the InteractionCoordinates dictionary
# add_library(InteractionCoordinates SHARED
#     InteractionCoordinates.cxx
#     # InteractionCoordinatesDict.cxx
#     InteractionCoordinates.h
# )

# # target_link_libraries(InteractionCoordinates PUBLIC PUBLIC ROOT::RIO ROOT::Tree ROOT::Physics ROOT::Imt NPRoot nptool PRIVATE ROOT::Core ${ROOT_LIBRARIES})
# target_link_libraries(InteractionCoordinates PUBLIC ROOT::RIO ROOT::Tree ROOT::Physics ROOT::Imt NPRoot nptool)

# Create a library for NPG4InteractionScorer
add_library(NPG4InteractionScorer SHARED
    NPG4InteractionScorer.cxx
    NPG4InteractionScorer.h
)

target_link_libraries(NPG4InteractionScorer PUBLIC InteractionCoordinates nptool ${Geant4_LIBRARIES})

# Install headers
install(DIRECTORY ./ DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY} FILES_MATCHING PATTERN "*.h")




