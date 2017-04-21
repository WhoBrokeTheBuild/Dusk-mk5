# Copyright 2017 Stephen Lane-Walsh
# Licensed under the MIT License

# Find the DMF Model File Library
#
# Output Variables
# - DMF_FOUND
# - DMF_INCLUDE_DIR
# - DMF_LIBRARY
#

SET(
  DMF_SEARCH_PATHS
  ${DMF_PATH}
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
  DMF_INCLUDE_DIR
  NAMES dmf/dmf.h
  HINTS
  PATH_SUFFIXES include src
  PATHS ${DMF_SEARCH_PATHS}
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(PATH_SUFFIXES lib64 lib/x64 lib/amd64 lib)
else()
	set(PATH_SUFFIXES lib/x86 lib)
endif()

FIND_LIBRARY(
  DMF_LIBRARY
  NAMES dmf
  HINTS
  PATH_SUFFIXES ${PATH_SUFFIXES}
  PATHS ${DMF_SEARCH_PATHS}
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  dmf
  REQUIRED_VARS
  DMF_INCLUDE_DIR
  DMF_LIBRARY
)
