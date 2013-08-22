# GTest用の環境を設定する
#
# gcovによるカバレッジを有効にしたい時はCMAKE_BUILD_TYPE=DEBUGを指定すること。
#
FIND_PACKAGE(GTest)

IF( GTEST_FOUND )
	IF(CMAKE_COMPILER_IS_GNUCXX)
		# gcov対応
		SET(CMAKE_C_FLAGS_DEBUG          "${CMAKE_C_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
		SET(CMAKE_CXX_FLAGS_DEBUG        "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
		SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
	ENDIF()
ENDIF()
