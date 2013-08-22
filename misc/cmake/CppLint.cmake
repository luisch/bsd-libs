# Check CppLint
# USAGE:
#  CHECK_CPPLINT(MySources)


MACRO(CHECK_CPPLINT TestName SourcesVar)

  FIND_PACKAGE(PythonInterp)
  FIND_PROGRAM(CPPLINT_PY cpplint.py DOC "cpplint.py script")
  MARK_AS_ADVANCED(CPPLINT_PY)

  IF( PYTHON_EXECUTABLE AND CPPLINT_PY )
    SET(CPPLINT_FILTER
      -runtime/rttr,-build/include_order,-whitespace/tab,-whitespace/parens
    )
    SET(CPPLINT_OPTION
    )

    IF(MSVC)
      SET(CPPLINT_OPTION ${CPPLINT_OPTION} --output=vs7)
    ENDIF()

    SET(Sources ${${SourcesVar}})
    ADD_CUSTOM_TARGET(cpplint-${TestName}
	COMMAND ${PYTHON_EXECUTABLE} ${CPPLINT_PY}
        --filter=${CPPLINT_FILTER}
        ${CPPLINT_OPTION}
	${Sources}
	VERBATIM
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
  ENDIF()
ENDMACRO()
