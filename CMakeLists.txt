CMAKE_MINIMUM_REQUIRED(VERSION 3.2 FATAL_ERROR)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

SET(ALL_SRC "")

### Set compiler-specific flags
IF(CMAKE_COMPILER_IS_GNUCC OR ${CMAKE_C_COMPILER_ID} MATCHES "Clang")
  SET(
    CMAKE_C_FLAGS
    " -std=c11 -D_XOPEN_SOURCE=700 \
      -Wall -Wextra -pedantic -Werror \
      -Wno-unused-parameter -Wno-gnu-zero-variadic-macro-arguments"
  )
  SET(
    CMAKE_CXX_FLAGS
    " -std=c++11 -D_XOPEN_SOURCE=700 \
      -Wall -Wextra -pedantic -Werror \
      -Wno-unused-parameter -Wno-gnu-zero-variadic-macro-arguments"
  )
ELSEIF(MSVC)
  SET(CMAKE_C_FLAGS "/NODEFAULTLIB:MSVCRT")
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
