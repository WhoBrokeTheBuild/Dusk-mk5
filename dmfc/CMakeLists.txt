PROJECT("Dusk Model Format Compiler" VERSION 0.0.1)

FIND_PACKAGE(FlatBuffers REQUIRED)
FIND_PACKAGE(ZLIB        REQUIRED)
FIND_PACKAGE(Dusk)

SET(dmfc_OUT dmfc)

SET(
    dmfc_SRCS
    src/dmfc.cpp
    src/tiny_obj_loader.h
)

SET(
  dmfc_FBS
  ${CMAKE_SOURCE_DIR}/libdusk/src/dmf.fbs
)

INCLUDE_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_BINARY_DIR}
)

IF(FLATBUFFERS_FOUND)
    FLATBUFFERS_GENERATE_C_HEADERS(
        dmfc_flatc
        ${dmfc_FBS}
    )
ENDIF(FLATBUFFERS_FOUND)

ADD_EXECUTABLE(
    ${dmfc_OUT}
    ${dmfc_SRCS}
    ${dmfc_flatc_OUTPUTS}
)

IF(ZLIB_FOUND)
  INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIRS})
  TARGET_LINK_LIBRARIES(${dmfc_OUT} ${ZLIB_LIBRARIES})
ENDIF()

SOURCE_GROUP("Sources" FILES ${dmfc_SRCS})
