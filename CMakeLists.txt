CMAKE_MINIMUM_REQUIRED(VERSION 3.2 FATAL_ERROR)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

### Set compiler-specific flags
IF (CMAKE_COMPILER_IS_GNUCC)
  SET(CMAKE_C_FLAGS " -std=c11 -Wall -Wextra -pedantic -Werror -Wno-unused-parameter")
ELSEIF(${CMAKE_C_COMPILER_ID} MATCHES "Clang")
  SET(CMAKE_C_FLAGS " -std=c11 -Wall -Wextra -pedantic -Werror -Wno-unused-parameter")
ELSEIF(MSVC)
  SET(CMAKE_C_FLAGS "/NODEFAULTLIB:MSVCRT")
ENDIF()

### Main Library
ADD_SUBDIRECTORY(libdusk)

### Example Projects
ADD_SUBDIRECTORY(examples)

### Special rule to allow for "make format" on linux
IF(UNIX)
  STRING(REPLACE ";" " " ALL_SRC "${ALL_SRC}")
  ADD_CUSTOM_TARGET(
    format
    COMMAND "clang-format -i ${ALL_SRC}"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )
ENDIF()
