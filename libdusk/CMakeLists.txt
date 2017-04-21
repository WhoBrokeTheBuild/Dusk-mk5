PROJECT(Dusk VERSION 0.0.1)

FIND_PACKAGE(FlatCC REQUIRED)
FIND_PACKAGE(GLMM   REQUIRED)

SET(Dusk_OUT dusk)

SET(
  Dusk_SRCS
  ${PROJECT_SOURCE_DIR}/include/dusk/dusk.h
  ${PROJECT_SOURCE_DIR}/include/dusk/camera.h
  ${PROJECT_SOURCE_DIR}/include/dusk/shader.h
  ${PROJECT_SOURCE_DIR}/include/dusk/mesh.h
  ${PROJECT_SOURCE_DIR}/include/dusk/model.h
  ${PROJECT_SOURCE_DIR}/include/dusk/dmf.h

  ${PROJECT_BINARY_DIR}/include/dusk/config.h
  ${PROJECT_BINARY_DIR}/include/dusk/flatbuffers_common_reader.h
  ${PROJECT_BINARY_DIR}/include/dusk/dmf_reader.h
  ${PROJECT_BINARY_DIR}/include/dusk/dmf_verifier.h

  ${PROJECT_SOURCE_DIR}/src/dusk.c
  ${PROJECT_SOURCE_DIR}/src/debug.h
  ${PROJECT_SOURCE_DIR}/src/camera.c
  ${PROJECT_SOURCE_DIR}/src/shader.c
  ${PROJECT_SOURCE_DIR}/src/mesh.c
  ${PROJECT_SOURCE_DIR}/src/model.c
  ${PROJECT_SOURCE_DIR}/src/dmf.c
)

SET(
  Dusk_FBS
  ${PROJECT_SOURCE_DIR}/src/dmf.fbs
)

INCLUDE_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}/include
)

CONFIGURE_FILE(
  ${PROJECT_SOURCE_DIR}/config.h.in
  ${PROJECT_BINARY_DIR}/include/dusk/config.h
)

if(FLATCC_FOUND)
  MESSAGE(STATUS "${FLATCC_EXECUTABLE}")
  ADD_CUSTOM_COMMAND(
    OUTPUT ${PROJECT_BINARY_DIR}/include/dusk/flatbuffers_common_reader.h
           ${PROJECT_BINARY_DIR}/include/dusk/dmf_reader.h
           ${PROJECT_BINARY_DIR}/include/dusk/dmf_verifier.h
    DEPENDS ${Dusk_FBS}
    COMMENT "Generating Flatbuffer C Files"
    COMMAND ${FLATCC_EXECUTABLE} -cv -o ${PROJECT_BINARY_DIR}/include/dusk ${Dusk_FBS}
  )
endif(FLATCC_FOUND)

IF(GLMM_FOUND)
  INCLUDE_DIRECTORIES(${GLMM_INCLUDE_DIR})
ENDIF()

ADD_LIBRARY(
  ${Dusk_OUT}
  ${Dusk_SRCS}
  ${Dusk_FCC_OUTPUTS}
  ${PROJECT_BINARY_DIR}/include/dusk/config.h
)

SOURCE_GROUP("Sources" FILES ${Dusk_SRCS})

LIST(APPEND ALL_SRC ${Dusk_SRCS})
SET(ALL_SRC ${ALL_SRC} PARENT_SCOPE)

SET(DUSK_PATH ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR} PARENT_SCOPE)
