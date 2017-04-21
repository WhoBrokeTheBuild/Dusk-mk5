PROJECT(Dusk VERSION 0.0.1)

SET(Dusk_OUT dusk)

CONFIGURE_FILE(
  ${PROJECT_SOURCE_DIR}/config.h.in
  ${PROJECT_BINARY_DIR}/include/dusk/config.h
)

SET(
  Dusk_SRCS
  ${PROJECT_BINARY_DIR}/include/dusk/config.h
  ${PROJECT_SOURCE_DIR}/include/dusk/dusk.h
  ${PROJECT_SOURCE_DIR}/src/dusk.c
)

INCLUDE_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}/include
)

ADD_LIBRARY(
  ${Dusk_OUT}
  ${Dusk_SRCS}
  ${CONF_FILE}
)

SOURCE_GROUP("Sources" FILES ${Dusk_SRCS})

LIST(APPEND ALL_SRC ${Dusk_SRCS})
SET(ALL_SRC ${ALL_SRC} PARENT_SCOPE)

SET(DUSK_PATH ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR} PARENT_SCOPE)