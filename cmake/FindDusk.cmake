# Copyright 2017 Stephen Lane-Walsh
# Licensed under the MIT License

# Find the Dusk Library
#
# Output Variables
# - DUSK_FOUND
# - DUSK_INCLUDE_DIRS
# - DUSK_LIBRARIES
#

# Determine 32-bit or 64-bit library path suffixes
IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
	SET(LIB_PATH_SUFFIXES lib64 lib)
ELSE()
	set(LIB_PATH_SUFFIXES lib)
ENDIF()

# Search DUSK_PATH
FIND_PATH(
  DUSK_INCLUDE_DIRS
  NAMES dusk/dusk.h
  PATHS ${DUSK_PATH}
  PATH_SUFFIXES include
  NO_DEFAULT_PATH
)
FIND_LIBRARY(
  DUSK_LIBRARY
  NAMES dusk
  PATHS ${DEVIL_PATH}
  PATH_SUFFIXES ${LIB_PATH_SUFFIXES}
  NO_DEFAULT_PATH
)

# Search pkg-config
FIND_PACKAGE(PkgConfig QUIET)
PKG_CHECK_MODULES(DUSK QUIET DUSK)

FIND_PATH(
  DUSK_INCLUDE_DIRS
  NAMES dusk/dusk.h
)
FIND_LIBRARY(
  DUSK_LIBRARY
  NAMES dusk
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  Dusk
  REQUIRED_VARS
  DUSK_INCLUDE_DIRS
  DUSK_LIBRARIES
)
