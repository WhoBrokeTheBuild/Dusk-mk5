macro(DUSK_EXAMPLE EX_NAME EX_SRC)

  PROJECT(${EX_NAME})

  SET(${EX_NAME}_OUT ${EX_NAME}.bin)

  FIND_PACKAGE(Dusk   REQUIRED)
  FIND_PACKAGE(GLMM   REQUIRED)
  FIND_PACKAGE(OpenGL REQUIRED)
  FIND_PACKAGE(GLEW   REQUIRED)
  FIND_PACKAGE(GLUT   REQUIRED)
  find_package(ZLIB   REQUIRED)
  find_package(FlatCC REQUIRED)

  ADD_EXECUTABLE(
    ${${EX_NAME}_OUT}
    ${EX_SRC}
  )

  IF(DUSK_FOUND)
    INCLUDE_DIRECTORIES(${DUSK_INCLUDE_DIR})
    TARGET_LINK_LIBRARIES(${${EX_NAME}_OUT} ${DUSK_LIBRARY})
  ENDIF()

  IF(GLMM_FOUND)
    INCLUDE_DIRECTORIES(${GLMM_INCLUDE_DIR})
  ENDIF()

  IF(OPENGL_FOUND)
    INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
    TARGET_LINK_LIBRARIES(${${EX_NAME}_OUT} ${OPENGL_LIBRARIES})
  ENDIF()

  IF(GLEW_FOUND)
    INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(${${EX_NAME}_OUT} ${GLEW_LIBRARIES})
  ENDIF()

  IF(GLUT_FOUND)
    INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})
    TARGET_LINK_LIBRARIES(${${EX_NAME}_OUT} ${GLUT_LIBRARIES})
  ENDIF()

  IF(ZLIB_FOUND)
    INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(${${EX_NAME}_OUT} ${ZLIB_LIBRARIES})
  ENDIF()

  IF(FLATCC_FOUND)
    INCLUDE_DIRECTORIES(${FLATCC_INCLUDE_DIR})
    TARGET_LINK_LIBRARIES(${${EX_NAME}_OUT} ${FLATCC_RUNTIME_LIBRARY})
  ENDIF()

  IF(UNIX)
    ADD_CUSTOM_TARGET(
      run-${EX_NAME}
      COMMAND ${PROJECT_BINARY_DIR}/${${PROJECT_NAME}_OUT}
      DEPENDS ${${PROJECT_NAME}_OUT}
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )
    ADD_CUSTOM_TARGET(
      valgrind-${EX_NAME}
      COMMAND valgrind ${PROJECT_BINARY_DIR}/${${PROJECT_NAME}_OUT}
      DEPENDS ${${PROJECT_NAME}_OUT}
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )
  ENDIF()

  SOURCE_GROUP("Sources" FILES ${EX_SRC})

  LIST(APPEND ALL_SRC ${hello_SRCS})
  SET(ALL_SRC ${ALL_SRC} PARENT_SCOPE)

endmacro()
