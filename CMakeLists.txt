CMAKE_MINIMUM_REQUIRED(VERSION 3.2 FATAL_ERROR)

SET(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
SET(CMAKE_COLOR_MAKEFILE          ON)

SET(SOLUTION_NAME "Dusk")

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

IF("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  MESSAGE(SEND_ERROR "In-source builds are not allowed.")
ENDIF()

### Set compiler-specific flags
IF(CMAKE_COMPILER_IS_GNUCC OR ${CMAKE_C_COMPILER_ID} MATCHES "Clang")
  SET(COMMON_FLAGS
    "-D_XOPEN_SOURCE=700 \
     -Wall -Wextra -pedantic -Werror \
     -Wno-unused-parameter -Wno-gnu-zero-variadic-macro-arguments"
  )
  SET(CMAKE_C_FLAGS   " -std=c11 ${COMMON_FLAGS}")
  SET(CMAKE_CXX_FLAGS " -std=c++11 ${COMMON_FLAGS}")
ELSEIF(MSVC)
  SET(
  	CMAKE_PREFIX_PATH
  	${CMAKE_PREFIX_PATH}
  	$ENV{VS140COMNTOOLS}../../VC # VS 2015
  )
  SET(
    CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} /D_CRT_SECURE_NO_WARNINGS /DGLEW_STATIC"
  )
  SET(
    CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:MSVCRT /NODEFAULTLIB:LIBCMT"
  )
  SET(CMAKE_SHARED_LIBRARY_PREFIX "")
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
