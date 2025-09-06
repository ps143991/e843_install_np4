# - Find MFM library
# This module tries to find the MFM lib installation on your system.
# It tries to find the gru-config script which gives you all the needed information.
# If the system variable MFMDIR is set this is straight forward.
# If not the module uses the pathes given in MFM_CONFIG_SEARCHPATH.
# If you need an other path you should add this path to this variable.  
# The root-config script is then used to detect basically everything else.
# This module defines a number of key variables and macros.

# Antoine Lemasson 15/4/2014

MESSAGE(STATUS "Looking for MFM ()...")

  SET(MFM_LIBRARY_DIR ${MFM_DIR}/lib)
  # Set MFM_BIN_DIR
  SET(MFM_BIN_DIR ${MFM_DIR}/bin)
  # Set MFM_INCLUDE_DIR
  SET(MFM_INCLUDES ${MFM_DIR}/include)
  SET(MFM_INCLUDE_DIRS  ${MFM_DIR}/include )
  
  SET(MFM_LIBRARIES "-L${MFM_LIBRARY_DIR} -lMFM")
  MESSAGE(STATUS "Looking for MFM... - found ${MFM_LIBRARY_DIR}")
  MESSAGE(STATUS " INCLUDE  ${MFM_INCLUDE_DIR}")
