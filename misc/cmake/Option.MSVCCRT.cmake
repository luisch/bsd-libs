
macro(fix_default_compiler_settings_)
	if (MSVC)
		# For MSVC, CMake sets certain flags to defaults we want to override.
		# This replacement code is taken from sample in the CMake Wiki at
		# http://www.cmake.org/Wiki/CMake_FAQ#Dynamic_Replace.
		foreach (flag_var 
		         CMAKE_CXX_FLAGS
		         CMAKE_CXX_FLAGS_DEBUG
		         CMAKE_CXX_FLAGS_RELEASE
		         CMAKE_CXX_FLAGS_RELWITHDEBINFO
		        )
			if (LUI_BUILD_STATIC_LIBS)
				string(REPLACE "/MD" "-MT" ${flag_var} "${${flag_var}}")
			endif()
		endforeach()
	endif()
endmacro()


option(
  LUI_BUILD_STATIC_LIBS
  "Use static libraries."
  OFF)
  
fix_default_compiler_settings_()
