# - Locate DEVIL library
# This module defines
# DEVIL_LIBRARY, the name of the library to link against
# DEVIL_FOUND
# DEVIL_INCLUDE_DIR, where to find DEVIL.h
# To Adding search path, set DEVIL_ROOT_DIR as follows
# set(DEVIL_ROOT_DIR "path/to/DEVIL")
# or launch cmake with -DDEVIL_ROOT_DIR="/path/to/DEVIL_ROOT_DIR".
#
# author: Kazunori Kimura
# email : kazunori.abu@gmail.com
#
# revisions: github.com/zwookie

SET(
  DEVIL_SEARCH_PATHS
  ${DEVIL_PATH}
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

find_path(
  DEVIL_INCLUDE_DIR
  NAMES IL/il.h
  HINTS
  PATH_SUFFIXES include
  PATHS ${DEVIL_SEARCH_PATHS}
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(PATH_SUFFIXES lib64 lib/x64 lib/amd64 lib)
else()
	set(PATH_SUFFIXES lib/x86 lib)
endif()

find_library(
  DEVIL_LIBRARY_IL
  NAMES IL
  HINTS
  PATH_SUFFIXES ${PATH_SUFFIXES}
  PATHS ${DEVIL_SEARCH_PATHS}
)

find_library(
  DEVIL_LIBRARY_ILU
  NAMES ILU
  HINTS
  PATH_SUFFIXES ${PATH_SUFFIXES}
  PATHS ${DEVIL_SEARCH_PATHS}
)

SET(DEVIL_LIBRARIES ${DEVIL_LIBRARY_IL} ${DEVIL_LIBRARY_ILU})

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  DevIL
  REQUIRED_VARS
  DEVIL_INCLUDE_DIR
  DEVIL_LIBRARIES
  DEVIL_LIBRARY_IL
  DEVIL_LIBRARY_ILU
)
