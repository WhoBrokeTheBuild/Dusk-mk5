SET(FOLDER "examples")

INCLUDE("ExampleProject")

ADD_SUBDIRECTORY(hello)
ADD_SUBDIRECTORY(cube)

IF(UNIX)
  ADD_CUSTOM_TARGET(
    format-examples
    DEPENDS format-hello format-cube
  )
ENDIF()
