# Copyright 2017 Stephen Lane-Walsh
# Licensed under the MIT License

# Find the Dusk Library
#
# Output Variables
# - DUSK_FOUND
# - DUSK_INCLUDE_DIR
# - DUSK_LIBRARY
#

SET(
  DUSK_SEARCH_PATHS
  ${DUSK_PATH}
  # Unix
  /usr/local
  /usr
  /opt
  # OSX
  ~/Library/Frameworks
  /Library/Frameworks
  # Visual Studio
  $ENV{VS140COMNTOOLS}../../VC # VS 2015
  $ENV{VS120COMNTOOLS}../../VC # VS 2013
  $ENV{VS110COMNTOOLS}../../VC # VS 2012
  $ENV{VS100COMNTOOLS}../../VC # VS 2010
)

FIND_PATH(
  DUSK_INCLUDE_DIR
  NAMES dusk/dusk.h
  HINTS
  PATH_SUFFIXES include src
  PATHS ${DUSK_SEARCH_PATHS}
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(PATH_SUFFIXES lib64 lib/x64 lib/amd64 lib)
else()
	set(PATH_SUFFIXES lib/x86 lib)
endif()

FIND_LIBRARY(
  DUSK_LIBRARY
  NAMES dusk
  HINTS
  PATH_SUFFIXES ${PATH_SUFFIXES}
  PATHS ${DUSK_SEARCH_PATHS}
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  Dusk
  REQUIRED_VARS
  DUSK_INCLUDE_DIR
  DUSK_LIBRARY
)
