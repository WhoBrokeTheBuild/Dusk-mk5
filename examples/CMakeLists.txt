SET(FOLDER "examples")

INCLUDE("ExampleProject")

ADD_SUBDIRECTORY(triangle)
ADD_SUBDIRECTORY(flat)
ADD_SUBDIRECTORY(phong)
ADD_SUBDIRECTORY(textured)

IF(UNIX)
  ADD_CUSTOM_TARGET(
    format-examples
    DEPENDS format-triangle format-flat format-phong format-textured
  )
ENDIF()
