include(FindPackageHandleStandardArgs)
SET(LUI_LIBPATH_SUFFIXES lib lib64 src src64)

FUNCTION( LUI_APPEND_DEBUGS _endvar _library )
    if(${_library} AND ${_library}_DEBUG)
        set(_output optimized ${${_library}} debug ${${_library}_DEBUG})
    else()
        set(_output ${${_library}})
    endif()
    set(${_endvar} ${_output} PARENT_SCOPE)
ENDFUNCTION()

FUNCTION( LUI_FIND_LIBRARY name )
	find_library(${name}
		NAMES ${ARGN}
		HINTS
		    $ENV{LUILIB_ROOT}
		    ${LUILIB_ROOT}
		PATH_SUFFIXES ${LUI_LIBPATH_SUFFIXES}
	)
	mark_as_advanced(${name})
	
	find_library(${name}_RELWITHDEBINFO
		NAMES ${ARGN}T
		HINTS
		    $ENV{LUILIB_ROOT}
		    ${LUILIB_ROOT}
		PATH_SUFFIXES ${LUI_LIBPATH_SUFFIXES}
	)
	mark_as_advanced(${name}_RELWITHDEBINFO)

	find_library(${name}_DEBUG
		NAMES ${ARGN}D
		HINTS
		    $ENV{LUILIB_ROOT}
		    ${LUILIB_ROOT}
		PATH_SUFFIXES ${LUI_LIBPATH_SUFFIXES}
	)
	mark_as_advanced(${name}_DEBUG)
	find_package_handle_standard_args(${name} DEFAULT_MSG ${name})
	
	IF( ${name}_FOUND )
		LUI_APPEND_DEBUGS(ALL_LIBS ${name} )
		SET(${name}S ${ALL_LIBS} PARENT_SCOPE)
	ENDIF()
ENDFUNCTION()


