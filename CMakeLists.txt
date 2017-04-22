CMAKE_MINIMUM_REQUIRED(VERSION 3.2 FATAL_ERROR)

SET(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
SET(CMAKE_COLOR_MAKEFILE          ON)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

IF("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  MESSAGE(SEND_ERROR "In-source builds are not allowed.")
ENDIF()

SET(ALL_SRC "")

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
  SET(CMAKE_C_FLAGS "/NODEFAULTLIB:MSVCRT")
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
    COMMAND clang-format -i ${ALL_SRC}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    SOURCES .clang-format
  )
ENDIF()
