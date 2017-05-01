# Copyright 2017 Stephen Lane-Walsh
# Licensed under the MIT License

# Find GLMM Include Files
#
# Output Variables
# - GLMM_FOUND
# - GLMM_INCLUDE_DIRS
#

SET(
  GLMM_SEARCH_PATHS
  ${GLMM_PATH}
  $ENV{VS140COMNTOOLS}../../VC # VS 2015
  $ENV{VS120COMNTOOLS}../../VC # VS 2013
  $ENV{VS110COMNTOOLS}../../VC # VS 2012
  $ENV{VS100COMNTOOLS}../../VC # VS 2010
)

# Search GLMM_PATH and special paths
FIND_PATH(
  GLMM_INCLUDE_DIRS
  NAMES glmm/glmm.h
  PATHS ${GLMM_SEARCH_PATHS}
  PATH_SUFFIXES include
  NO_DEFAULT_PATH
)

# Search pkg-config
FIND_PACKAGE(PkgConfig)
PKG_CHECK_MODULES(GLMM QUIET glmm)

# Search CMake standard paths
FIND_PATH(
  GLMM_INCLUDE_DIRS
  NAMES glmm/glmm.h
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  GLMM
  REQUIRED_VARS
  GLMM_INCLUDE_DIRS
)
