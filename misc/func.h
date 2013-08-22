/*! @file func.h

   @brief ヘルパー関数のまとめ

   FUNCTION COLLECTION on MISC LIBRARY.
   This program has been written in C and C++.
   Distributed under the BSD License. (See accompanying file LICENSE.TXT.)
   Copyright (C) 2006 ゑ, All Rights Reserved.

   @author ゑ.
*/
#ifndef LUI_MISC_LIB_FUNC_H
#define LUI_MISC_LIB_FUNC_H

#include "type.hpp"

#ifndef LUI_EXPORT
#	define LUI_EXPORT
#endif

namespace lui {
	
	//! OSの情報を作成し、文字列にして返却する
	/*!
		@ret OS情報が格納された文字列
	*/
	LUI_EXPORT std::string os_info();

	// 文字列をエンコードする
	LUI_EXPORT  std::string encodeString( const std::string& in_str );

	// unicodeとの相互変換
	LUI_EXPORT const std::wstring a2w( const std::string&  str );
	LUI_EXPORT const std::string  w2a( const std::wstring& str );
	LUI_EXPORT const std::wstring a2w( const std::string&  str, const std::locale& l );
	LUI_EXPORT const std::string  w2a( const std::wstring& str, const std::locale& l );
	LUI_EXPORT const std::wstring u2w( const std::string& str  ); // UTF-8   => wchar_t
	LUI_EXPORT const std::string  w2u( const std::wstring& str ); // wchar_t => UTF-8
	
	// 相互変換補助関数
#	ifdef UNICODE
		inline const tstring a2t( const std::string& str )                  { return a2w(str);   }
		inline const tstring a2t( const std::string& str, const std::locale& l )  { return a2w(str,l); }
		inline const tstring w2t( const std::wstring& str )                 { return str; }
		inline const tstring w2t( const std::wstring& str, const std::locale& l ) { return str; }
		inline const std::string t2a( const tstring& str )                  { return w2a(str);   }
		inline const std::string t2a( const tstring& str, const std::locale& l )  { return w2a(str,l); }
		inline const std::wstring t2w( const tstring& str )                 { return str; }
		inline const std::wstring t2w( const tstring& str, const std::locale& l ) { return str; }
#	else
		inline const tstring a2t( const std::string& str )                  { return str; }
		inline const tstring a2t( const std::string& str, const std::locale& l )  { return str; }
		inline const tstring w2t( const std::wstring& str )                 { return w2a(str);   }
		inline const tstring w2t( const std::wstring& str, const std::locale& l ) { return w2a(str,l); }
		inline const std::string t2a( const tstring& str )                  { return str; }
		inline const std::string t2a( const tstring& str, const std::locale& l )  { return str; }
		inline const std::wstring t2w( const tstring& str )                 { return a2w(str);   }
		inline const std::wstring t2w( const tstring& str, const std::locale& l ) { return a2w(str,l); }
#	endif // unicode
	
	inline const std::string  u2a( const std::string& str ) { return w2a(u2w(str)); }
	inline const std::string  a2u( const std::string& str ) { return w2u(a2w(str)); }
}

#endif//LUI_MISC_LIB_FUNC_H
