MACRO(DUSK_EXAMPLE EX_NAME)

  PROJECT(${EX_NAME})

  IF(WIN32)
    SET(EXT "")
  ELSE()
    SET(EXT ".bin")
  ENDIF()

  SET(${EX_NAME}_SRCS ${ARGN})
  SET(${EX_NAME}_OUT ${EX_NAME}${EXT})

  FIND_PACKAGE(GLMM   REQUIRED)
  FIND_PACKAGE(OpenGL REQUIRED)
  FIND_PACKAGE(GLEW   REQUIRED)
  FIND_PACKAGE(SDL2   REQUIRED COMPONENTS image ttf)
  FIND_PACKAGE(ZLIB   REQUIRED)
  FIND_PACKAGE(FlatCC REQUIRED)

  ADD_EXECUTABLE(
    ${${EX_NAME}_OUT}
    ${${EX_NAME}_SRCS}
  )

  INCLUDE_DIRECTORIES(${Dusk_INCLUDE_DIRS})
  TARGET_LINK_LIBRARIES(${${EX_NAME}_OUT} dusk)

  IF(GLMM_FOUND)
    INCLUDE_DIRECTORIES(${GLMM_INCLUDE_DIRS})
  ENDIF()

  IF(OPENGL_FOUND)
    INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
    TARGET_LINK_LIBRARIES(${${EX_NAME}_OUT} ${OPENGL_LIBRARIES})
  ENDIF()

  IF(GLEW_FOUND)
    INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(${${EX_NAME}_OUT} ${GLEW_LIBRARIES})
  ENDIF()

  IF(SDL2_FOUND)
    INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(${${EX_NAME}_OUT} ${SDL2_LIBRARIES})
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
    TARGET_LINK_LIBRARIES(${${EX_NAME}_OUT} m pthread)
  ENDIF()

  SET_TARGET_PROPERTIES(${${EX_NAME}_OUT} PROPERTIES FOLDER "examples")
  SOURCE_GROUP("src" FILES ${${PROJECT_NAME}_SRCS})

  IF(WIN32)
    SET(DLLPATH "${VC_ROOT}/bin\\;${DLLPATH}")
    IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
      SET(DLLPATH "${VC_ROOT}/bin/amd64\\;${DLLPATH}")
    ENDIF()
    ADD_CUSTOM_TARGET(
      run-${EX_NAME}
      COMMENT "Running ${EX_NAME}"
      COMMAND ${CMAKE_COMMAND} -E env PATH="${DLLPATH}" "$<TARGET_FILE:${${EX_NAME}_OUT}>"
      DEPENDS ${${EX_NAME}_OUT}
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )
  ELSEIF(UNIX)
    ADD_CUSTOM_TARGET(
      run-${EX_NAME}
      COMMAND $<TARGET_FILE:${${EX_NAME}_OUT}>
      DEPENDS ${${EX_NAME}_OUT}
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )
    ADD_CUSTOM_TARGET(
      valgrind-${EX_NAME}
      COMMAND valgrind $<TARGET_FILE:${${EX_NAME}_OUT}>
      DEPENDS ${${EX_NAME}_OUT}
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )
    ADD_CUSTOM_TARGET(
      format-${EX_NAME}
      COMMAND clang-format -i ${${EX_NAME}_SRCS}
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )
  ENDIF()

  SET_TARGET_PROPERTIES(run-${EX_NAME} PROPERTIES FOLDER "examples")

  IF(NOT (${CMAKE_VERSION} VERSION_LESS "3.8"))
    SET_TARGET_PROPERTIES(${${EX_NAME}_OUT} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
  ENDIF()

ENDMACRO()
