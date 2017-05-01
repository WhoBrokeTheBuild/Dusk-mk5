# Copyright 2017 Stephen Lane-Walsh
# Licensed under the MIT License

# Find GLMM Include Files
#
# Output Variables
# - GLMM_FOUND
# - GLMM_INCLUDE_DIRS
#

# Search GLMM_PATH
FIND_PATH(
  GLMM_INCLUDE_DIRS
  NAMES glmm/glmm.h
  PATHS ${GLMM_PATH}
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
