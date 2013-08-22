/*! @file exception.h

   @brief  独自の例外エラーを保持して投げられるクラス群

   EXCEPTION on MISC LIBRARY, transfered from SDC.
   This program has been written in C and C++.
   Distributed under the BSD License. (See accompanying file LICENSE.TXT.)
   Copyright (C) 2005 , All Rights Reserved.

   @author .
*/
#ifndef MISC_EXCEPTION_H
#define MISC_EXCEPTION_H
#include "func.h"

#ifndef LUI_EXPORT
#	define LUI_EXPORT
#endif

namespace lui {

	//! SDCLOGのレベル
	enum log_level {
		debug_info,   //!< デバッグ用メッセージ
		trace_info,   //!< デバッグトレース用メッセージ
		warning_info, //!< 警告
		message_info, //!< メッセージ表示
		error_info,   //!< エラー
		critical_info,//!< 致命エラー
		all_info = 0xFFFF,
	};
	/*!
		ログに出力する。デバッグモードの場合、
		画面上にコンソールに表示する。
		@param[in] str 出力するログ
	*/
	LUI_EXPORT void syslog( const std::string& str, log_level lv = trace_info );
	LUI_EXPORT void syslog( const std::wstring& str, log_level lv = trace_info );
	LUI_EXPORT void syslog( const char* str, log_level lv = trace_info );
	LUI_EXPORT void syslog( const wchar_t* str, log_level lv = trace_info );

	//! SDC_TRACEが実際に出力するレベルを決める
	LUI_EXPORT void setTraceLevel( log_level minimum_level );
	LUI_EXPORT void setOutput( std::streambuf* target );

	typedef std::string (*LOGFILTERFUNC)( const std::string&, log_level );
	LUI_EXPORT void setLogFilterFunc(LOGFILTERFUNC);
}

#ifdef _MSC_VER
#pragma comment(lib,"imagehlp.lib")
#endif//_WIN32

#endif//MISC_EXCEPTION_H
