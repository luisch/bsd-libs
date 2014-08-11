#
# CMake for LUI-misc library.
#
cmake_minimum_required(VERSION 3.0.0)

## 追加のcmakeコード
SET(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${CMAKE_HOME_DIRECTORY}/../misc/cmake"
)

INCLUDE( BuildAndInstall )
## projectコマンドがキャッシュに色々書き込むことがあるので
## そういうのは↑でまとめて。

## パッケージ名称
project(misc)
SET(CPACK_PACKAGE_VENDOR "mint.moe")
SET(CPACK_PACKAGE_VERSION_MAJOR 1)
SET(CPACK_PACKAGE_VERSION_MINOR 0)

## 追加のcmakeコード
## projectコマンドの後ろでないとうまく動かない（環境依存系）はここで。
INCLUDE( Option.MSVCCRT )
INCLUDE( Check )
INCLUDE( CppLint )
INCLUDE( Env )
INCLUDE( Option.STLport )
INCLUDE( PreCompile )
INCLUDE( Install )

## 参照ライブラリのパス
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}
	"${CMAKE_HOME_DIRECTORY}/../lib"
	CACHE TYPE "LIB PATH"
)

## ソース一覧
SET(src
	exception.cpp 
	udp_service.cpp 
	func.cpp 
)

IF(MSVC)
	ADD_MSVC_PRECOMPILED_HEADER("misc.h" "misc.cpp" src)
ENDIF()

SET(hpp
	akima.h
	akima.inl
	auto_link.hpp
	exception.h
	func.h
	handle.hpp
	hash_map_list.hpp
	math.hpp
	misc.h
	spline.h
	spline.inl
	stream_ptr.hpp
	type.hpp
	udp_service.h
	zstream.hpp
)

## 利用する必須ライブラリ
SET(Boost_USE_STLPORT     ${LUI_USE_STLPORT})
SET(Boost_USE_STATIC_LIBS ${LUI_BUILD_STATIC_LIBS})
SET(Boost_USE_STATIC_RUNTIME ${LUI_BUILD_STATIC_LIBS})
FIND_PACKAGE(Boost REQUIRED
COMPONENTS
	chrono
	filesystem
	timer
	date_time
	regex
	system
	thread
)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

## CPP LINT
CHECK_CPPLINT( checklint-misc src )

## ライブラリ
IF(LUI_BUILD_STATIC_LIBS)
	ADD_LIBRARY(misc STATIC ${src} ${hpp} )
ELSE()
	ADD_LIBRARY(misc SHARED ${src} ${hpp} )
ENDIF()
TARGET_LINK_LIBRARIES(misc
	${Boost_LIBRARIES}
)

# インストーラ
LOCAL_INSTALL(misc)

## テスト用プロジェクト
if( GTEST_FOUND )
  ADD_SUBDIRECTORY(check)
endif()

