CMAKE_MINIMUM_REQUIRED(VERSION 3.2 FATAL_ERROR)

PROJECT(Dusk)

### Global Configuration
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

### Disable in-source builds
SET(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
IF("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  MESSAGE(SEND_ERROR "In-source builds are not allowed.")
ENDIF()

### MSVC Fixes
IF(MSVC)
  # A global variable for the path to the VC directory
  SET(VC_ROOT $ENV{VS140COMNTOOLS}../../VC)
  STRING(REPLACE "\\" "/" VC_ROOT ${VC_ROOT})

  # Force searching for 64-bit libraries first, if building 64-bit
  SET(VC_CMAKE_PREFIX ${VC_ROOT})
  IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET(VC_CMAKE_PREFIX ${VC_ROOT}/lib/amd64 ${VC_ROOT}/bin/amd64 ${VC_ROOT})
  ENDIF()

  # Prepend the cmake module search path with our visual studio path
  SET(CMAKE_PREFIX_PATH ${VC_CMAKE_PREFIX} ${CMAKE_PREFIX_PATH})

  # Prevent 'lib' from being prepended to libdusk
  SET(CMAKE_SHARED_LIBRARY_PREFIX "")
ENDIF()

### Set compiler-specific flags
IF(CMAKE_COMPILER_IS_GNUCC OR ${CMAKE_C_COMPILER_ID} MATCHES "Clang")
  SET(COMMON_FLAGS "-D_XOPEN_SOURCE=700 \
                    -Wall -Wextra -pedantic \
                    -Wno-unused-parameter -Wno-gnu-zero-variadic-macro-arguments -Wno-missing-braces")
  SET(CMAKE_C_FLAGS   "-std=c11 ${COMMON_FLAGS} ${CMAKE_CXX_FLAGS}")
  SET(CMAKE_CXX_FLAGS "-std=c++11 ${COMMON_FLAGS} ${CMAKE_CXX_FLAGS}")
ELSEIF(MSVC)
  SET(COMMON_FLAGS "/D_CRT_SECURE_NO_WARNINGS")
  SET(CMAKE_C_FLAGS "${COMMON_FLAGS} ${CMAKE_C_FLAGS}")
  SET(CMAKE_CXX_FLAGS "${COMMON_FLAGS} ${CMAKE_CXX_FLAGS}")
  SET(CMAKE_EXE_LINKER_FLAGS "/NODEFAULTLIB:MSVCRT /NODEFAULTLIB:LIBCMT ${CMAKE_EXE_LINKER_FLAGS}")
ENDIF()

### Main Library
ADD_SUBDIRECTORY(libdusk)

### Dusk Model Format Compiler
ADD_SUBDIRECTORY(dmfc)

### Example Projects
ADD_SUBDIRECTORY(examples)

### Special rule to allow for "make format" on linux
IF(UNIX)
  ADD_CUSTOM_TARGET(
    format
    COMMENT "Formatting Source Files"
    DEPENDS format-dusk format-dmfc format-examples
    SOURCES .clang-format
  )
ENDIF()
