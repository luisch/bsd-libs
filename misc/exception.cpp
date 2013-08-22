/*! @file exception.cpp

   @brief  独自の例外エラーを保持して投げられるクラス群の実装

   EXCEPTION on MISC LIBRARY, transfered from SDC.
   This program has been written in C and C++.
   Distributed under the BSD License. (See accompanying file LICENSE.TXT.)
   Copyright (C) 2005 ゑ, All Rights Reserved.

   @author ゑ.
*/

#include "misc.h"
#include "exception.h"

// for win32 //
#ifdef LUI_USE_WIN32
#include <windows.h>
#include <imagehlp.h>

namespace lui {
	/*!
		Windowsで、例外エラー発生時のスタックトレースを得るプログラム
	*/
	const std::string stackWalk( const CONTEXT* ctx )
	{
		const int MAX_STACKFLAME_LENGTH = 10000;
		HANDLE hProc = ::GetCurrentProcess();
		
		PIMAGEHLP_SYMBOL pSym; 
		pSym = static_cast<PIMAGEHLP_SYMBOL>( GlobalAlloc(
			GMEM_FIXED, MAX_STACKFLAME_LENGTH
		));
		pSym->SizeOfStruct  = MAX_STACKFLAME_LENGTH;
		pSym->MaxNameLength = MAX_STACKFLAME_LENGTH-sizeof(IMAGEHLP_SYMBOL);
		
		STACKFRAME sf;
		std::memset( &sf, 0, sizeof(sf) );
#ifdef _M_IX86
		const DWORD imageFile = IMAGE_FILE_MACHINE_I386;
		sf.AddrPC.Offset = ctx->Eip;
		sf.AddrPC.Mode   = AddrModeFlat;
		sf.AddrStack.Offset = ctx->Esp;
		sf.AddrStack.Mode   = AddrModeFlat;
		sf.AddrFrame.Offset = ctx->Ebp;
		sf.AddrFrame.Mode   = AddrModeFlat;
#elif defined _M_AMD64
		const DWORD imageFile = IMAGE_FILE_MACHINE_AMD64;
		sf.AddrPC.Offset = ctx->Rip;
		sf.AddrPC.Mode   = AddrModeFlat;
		sf.AddrStack.Offset = ctx->Rsp;
		sf.AddrStack.Mode   = AddrModeFlat;
		sf.AddrFrame.Offset = ctx->Rbp;
		sf.AddrFrame.Mode   = AddrModeFlat;
#else
#	error "Not Implemented"
#endif
		
		std::ostringstream str;
		str << "walking stack:" << "\n";
		
		::SymInitialize( hProc, NULL, TRUE );
		for(;;) {
			HRESULT hres = ::StackWalk(
				imageFile,
				hProc,
				::GetCurrentThread(),
				&sf,
				NULL,
				NULL,
				SymFunctionTableAccess,
				SymGetModuleBase,
				NULL
			);
			if( !hres || !sf.AddrFrame.Offset ) break;
			
#ifdef _WIN64
			DWORD64 disp;
#else
			DWORD disp;
#endif
			hres = ::SymGetSymFromAddr(
				hProc,
				sf.AddrPC.Offset,
				&disp,
				pSym
			);
			if( hres ) {
				str << "  0x" << std::hex << std::setfill('0') << std::setw(8) << sf.AddrPC.Offset
				    << " " << pSym->Name << " + 0x" << disp;
			}
			else {
				str << "  0x" << std::hex << std::setfill('0') << std::setw(8) << sf.AddrPC.Offset
				    << " ---- ";
			}
			str << "\n";
		}
		SymCleanup(hProc);
		::GlobalFree(pSym);
		
		return str.str();
	}

	/*!
		Windowsで、例外エラー発生時にコールバックされる関数
		この関数をSetUnhandledExceptionFilter()に渡すことで
		有効化され、自前のstackWalkが利用されるようになる。
	*/
	LONG CALLBACK win32_exception_filter( EXCEPTION_POINTERS* info )
	{
		const EXCEPTION_RECORD* r = info->ExceptionRecord;
		const char* err_code = NULL;

		switch( r->ExceptionCode ) {
		case EXCEPTION_ACCESS_VIOLATION:         err_code = "access violation";         break;
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    err_code = "array bounds exceeded";    break;
		case EXCEPTION_BREAKPOINT:               err_code = "break point";              break;
		case EXCEPTION_DATATYPE_MISALIGNMENT:    err_code = "datatype misaligenment";   break;
		case EXCEPTION_FLT_DENORMAL_OPERAND:     err_code = "float denormal operand";   break;
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:       err_code = "float devide by zero";     break;
		case EXCEPTION_FLT_INEXACT_RESULT:       err_code = "float inexact result";     break;
		case EXCEPTION_FLT_INVALID_OPERATION:    err_code = "float invalid operation";  break;
		case EXCEPTION_FLT_OVERFLOW:             err_code = "float overflow";           break;
		case EXCEPTION_FLT_STACK_CHECK:          err_code = "float stack check";        break;
		case EXCEPTION_FLT_UNDERFLOW:            err_code = "float underflow";          break;
		case EXCEPTION_ILLEGAL_INSTRUCTION:      err_code = "illegal instruction";      break;
		case EXCEPTION_IN_PAGE_ERROR:            err_code = "in-page error";            break;
		case EXCEPTION_INT_DIVIDE_BY_ZERO:       err_code = "int devided by zero";      break;
		case EXCEPTION_INT_OVERFLOW:             err_code = "int overflow";             break;
		case EXCEPTION_INVALID_DISPOSITION:      err_code = "invalid disposition";      break;
		case EXCEPTION_NONCONTINUABLE_EXCEPTION: err_code = "noncontinuable exception"; break;
		case EXCEPTION_PRIV_INSTRUCTION:         err_code = "priv instruction";         break;
		case EXCEPTION_SINGLE_STEP:              err_code = "single step";              break;
		case EXCEPTION_STACK_OVERFLOW:           err_code = "stack overflow";           break;
		default:                                 err_code = "unknown";                  break;
		}

		std::ostringstream str;
		str <<
			"win32 SEH exception! \n"
			"Exception Code: " << std::hex << r->ExceptionCode <<"("<< err_code <<")"<< "\n"
			"Flags         : " << std::hex << r->ExceptionFlags          << "\n"
			"Address       : " << std::hex << r->ExceptionAddress        << "\n"
			"Params        : "
		;
	    for( unsigned i = 0; i < r->NumberParameters && i < EXCEPTION_MAXIMUM_PARAMETERS; ++i )
		{
			if( i % 2 == 0 ) { str << "\n"; }
			str << "0x" << std::hex << std::setfill('0') << std::setw(8) << r->ExceptionInformation[i] << " ";
		}
		r = r->ExceptionRecord;
		
		str << "\n"
		    << stackWalk( info->ContextRecord ).c_str(), lui::critical_info
		;
	
		syslog( str.str(), critical_info );
		return EXCEPTION_EXECUTE_HANDLER;
	}

	void win32_exception_trans( unsigned code, EXCEPTION_POINTERS* info )
	{
		win32_exception_filter( info );
	}
}//namespace lui;
#endif//LUI_USE_WIN32

namespace lui {
	
	std::string syslog_default_func( const std::string& msg, log_level lv )
	{
		return msg;
	}
	/*!
		システム全体のログを管理するクラス
		スレッドセーフであり、設定レベルに応じて実際に出力するか決められる。
		外部からはこのクラスに直接アクセスできず、API的な関数を経由してのみ
		ログを記述できる。
	*/
	class syslog_object
	{
	public:
		syslog_object() : m_minimumLevel(debug_info),
			m_stdout( std::cerr.rdbuf() ),
			filterFunc( &syslog_default_func )
		{
			
#ifdef LUI_USE_WIN32
			// これはプロセス毎に行う
			::SetUnhandledExceptionFilter( lui::win32_exception_filter );
#endif// LUI_USE_WIN32
			
			// syslog開始
			const boost::gregorian::date d = boost::gregorian::day_clock::local_day();
			const boost::posix_time::ptime t = boost::posix_time::second_clock::local_time();
		}
		virtual ~syslog_object()
		{
			boost::mutex::scoped_lock l(m_mutex);
		}
		void setOutputLevel( log_level ml )
		{
			boost::mutex::scoped_lock l(m_mutex);
			m_minimumLevel = ml;
		}
		
		void logging( const std::string& str, log_level lv )
		{
			boost::mutex::scoped_lock l(m_mutex);
			if( lv < m_minimumLevel ) { return; }
			
			try
			{
				const boost::posix_time::ptime t = boost::posix_time::second_clock::local_time();
				
				// 同じメッセージのまとめ
				if( m_lastLogStr == str )
				{
					m_lastLogTime = t;
					++m_lastLogCount;
					return;
				}
				m_lastLogStr = str;
				
				// else
				std::ostringstream out;
				out.imbue( std::locale(
					std::locale::classic(),
					new boost::date_time::time_facet<boost::posix_time::ptime, char>("%H:%M:%S")
				) );
				
				if( m_lastLogCount > 0 ) {
					out << "[" << m_lastLogTime << "]\t" << "last message repeated " << m_lastLogCount << " times\n";
					m_lastLogCount = 0;
				}
				
				// ログレベル
				const char* level_name = NULL;
				switch( lv )
				{
				case lui::debug_info:
					level_name = "debug";
					break;
				case lui::trace_info:
					level_name = "testing";
					break;
				case lui::warning_info:
					level_name = "warning";
					break;
				case lui::error_info:
					level_name = "error";
					break;
				case lui::critical_info:
					level_name = "critial";
					break;
				default: //!< 常にメッセージ
					level_name = "message";
					break;
				}
				std::string::size_type it = 0;
				out << "[" << t << "]\t[" << level_name << "]\t";
				for(;true;)
				{
					std::string::size_type first = it;
					it = str.find( '\n', first );

					out << str.substr(first,it-first) << "\n";

					if( it == std::string::npos ) {
						break;
					}
					else {
						++it;
						out << "          \t[" << level_name << "]\t";
					}
				}
				m_stdout << out.str() << std::flush;
			}
			catch( ... )
			{ /* do nothing */ }
		};

		void setOutput( std::streambuf* target )
		{
			boost::mutex::scoped_lock l(m_mutex);
			m_lastLogStr = "";
			m_lastLogCount = 0;
			m_stdout.rdbuf( target ? target : std::cerr.rdbuf() );
		}

		void setFilterFunc( LOGFILTERFUNC pff )
		{
			boost::mutex::scoped_lock l(m_mutex);
			if(pff) {
				filterFunc = pff;
			}
			else {
				filterFunc = &syslog_default_func;
			}
		}

	private:
		volatile log_level m_minimumLevel; //!< このレベル以下のエラーについて出力する
		boost::mutex m_mutex; //!< ログのmutex
		std::ostream m_stdout; //!< 標準出力

		LOGFILTERFUNC filterFunc;
		
		std::string m_lastLogStr;
		boost::posix_time::ptime m_lastLogTime;
		int         m_lastLogCount;
	};

	lui::syslog_object& getSyslogObject()
	{
		static syslog_object s;
		return s;
	}
}

// エクスポート関数
void lui::syslog( const std::string& str, log_level lv )
{
	lui::getSyslogObject().logging( str, lv );
}

void lui::syslog( const std::wstring& str, log_level lv )
{
	lui::getSyslogObject().logging( lui::w2a(str), lv );
}

void lui::syslog( const char* str, log_level lv )
{
	lui::getSyslogObject().logging( std::string(str), lv );
}

void lui::syslog( const wchar_t* str, log_level lv )
{
	lui::getSyslogObject().logging( lui::w2a( std::wstring(str) ), lv );
}

void lui::setTraceLevel( log_level minimum_level )
{
	lui::getSyslogObject().setOutputLevel( minimum_level );
}

void lui::setLogFilterFunc( LOGFILTERFUNC pfn )
{
	lui::getSyslogObject().setFilterFunc(pfn);
}
void lui::setOutput( std::streambuf* lfn ) {
	lui::getSyslogObject().setOutput(lfn);
}
