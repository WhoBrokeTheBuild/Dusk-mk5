# - Locate SOIL library
# This module defines
# SOIL_LIBRARY, the name of the library to link against
# SOIL_FOUND
# SOIL_INCLUDE_DIR, where to find SOIL.h
# To Adding search path, set SOIL_ROOT_DIR as follows
# set(SOIL_ROOT_DIR "path/to/soil")
# or launch cmake with -DSOIL_ROOT_DIR="/path/to/SOIL_ROOT_DIR".
#
# author: Kazunori Kimura
# email : kazunori.abu@gmail.com
#
# revisions: github.com/zwookie

SET(
  SOIL_SEARCH_PATHS
  ${SOIL_PATH}
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
  SOIL_INCLUDE_DIR
  NAMES SOIL/SOIL.h SOIL.h
  HINTS
  PATH_SUFFIXES include
  PATHS ${DUSK_SEARCH_PATHS}
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(PATH_SUFFIXES lib64 lib/x64 lib/amd64 lib)
else()
	set(PATH_SUFFIXES lib/x86 lib)
endif()

find_library(
  SOIL_LIBRARY
  NAMES SOIL
  HINTS
  PATH_SUFFIXES ${PATH_SUFFIXES}
  PATHS ${DUSK_SEARCH_PATHS}
)

IF(SOIL_INCLUDE_DIR AND SOIL_LIBRARY)
	SET( SOIL_FOUND TRUE )
	SET( SOIL_LIBRARIES ${SOIL_LIBRARY} )
ENDIF(SOIL_INCLUDE_DIR AND SOIL_LIBRARY)
IF(SOIL_FOUND)
	IF(NOT SOIL_FIND_QUIETLY)
	MESSAGE(STATUS "Found SOIL: ${SOIL_LIBRARY}")
	ENDIF(NOT SOIL_FIND_QUIETLY)
ELSE(SOIL_FOUND)
	IF(SOIL_FIND_REQUIRED)
	MESSAGE(FATAL_ERROR "Could not find libSOIL")
	ENDIF(SOIL_FIND_REQUIRED)
ENDIF(SOIL_FOUND)
