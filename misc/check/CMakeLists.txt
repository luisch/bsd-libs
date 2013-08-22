#
# CMake for misc check.
#
## ソース一覧
SET(src
	exception.cpp
	type.cpp
	func.cpp
	math.cpp
	zstream.cpp
)
SET(hpp
	check.h
	stdafx.h
)

## プリコンパイル
ADD_MSVC_PRECOMPILED_HEADER("stdafx.h" "stdafx.cpp" src)

## 利用する必須ライブラリ
FIND_PACKAGE(ZLIB REQUIRED)

## 実行ファイル
ADD_EXECUTABLE(check ${src} ${hpp} ${rc})
TARGET_LINK_LIBRARIES(check
	misc
	${ZLIB_LIBRARIES}
	${GTEST_BOTH_LIBRARIES}
)

# インストールしない
