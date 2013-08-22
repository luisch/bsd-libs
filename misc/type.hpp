/*! @file type.hpp

   @brief ビットサイズを確定させる変数型

   TYPE list on MISC LIBRARY.
   This program has been written in C and C++.
   Distributed under the BSD License. (See accompanying file LICENSE.TXT.)
   Copyright (C) 2004 , All Rights Reserved.

   @author .
*/
#include <boost/cstdint.hpp>

//////////////////////////////////////////////////////////////////////////
// type.h
// 汎用定義ヘッダファイル
#ifndef LUI_TYPE_H
#define LUI_TYPE_H

#include <string>

namespace lui
{
#ifdef UNICODE
	typedef std::wstring tstring;
#else
	typedef std::string tstring;
#endif
	// バイトサイズをあわせた型
	namespace type
	{
		// 1byte  変数
		typedef boost::int8_t     i8;
		typedef boost::uint8_t   ui8;

		// 2bytes 変数
		typedef boost::int16_t    i16;
		typedef boost::uint16_t  ui16;

		// 4bytes 変数
		typedef boost::int32_t    i32;
		typedef boost::uint32_t  ui32;

		// 8bytes 変数
		typedef boost::int64_t    i64;
		typedef boost::uint64_t  ui64;
	}
};

#endif// LUI_TYPE_H
