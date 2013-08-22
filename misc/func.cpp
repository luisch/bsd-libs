/*! @file func.cpp

   @brief ヘルパー関数の実装

   FUNCTION COLLECTION on MISC LIBRARY.
   This program has been written in C and C++.
   Distributed under the BSD License. (See accompanying file LICENSE.TXT.)
   Copyright (C) 2006 ゑ, All Rights Reserved.

   @author ゑ.
*/
#include "misc.h"
#include "type.hpp"
#include "func.h"

// for UTF-8 //
#include <boost/filesystem/config.hpp>
#define BOOST_UTF8_BEGIN_NAMESPACE namespace boost { namespace filesystem { namespace detail {
#define BOOST_UTF8_END_NAMESPACE }}}
#define BOOST_UTF8_DECL BOOST_FILESYSTEM_DECL
#include <boost/detail/utf8_codecvt_facet.hpp>
#undef BOOST_UTF8_BEGIN_NAMESPACE
#undef BOOST_UTF8_END_NAMESPACE
#undef BOOST_UTF8_DECL


std::string lui::os_info()
{
	std::ostringstream str;

#ifdef  LUI_USE_WIN32
#define win32
	
	OSVERSIONINFOEX info;
	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	BOOL isInfoEx = win32::GetVersionEx( reinterpret_cast<OSVERSIONINFO*>(&info) );

	if( !isInfoEx )
	{
		info.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
		win32::GetVersionEx( reinterpret_cast<OSVERSIONINFO*>(&info) );
	}

	// 結果を文字化
	str << "Windows ";
	if( info.dwPlatformId & VER_PLATFORM_WIN32_NT )
		str << "NT ";

	str << info.dwMajorVersion << "." << info.dwMinorVersion
		<< "(" << info.dwBuildNumber << ") ";

	if( isInfoEx )
	{
		switch( info.wProductType )
		{
		case VER_NT_WORKSTATION:
			str << "Workstation ";
			break;
		case VER_NT_SERVER:
			str << "Server ";
			break;
		case VER_NT_DOMAIN_CONTROLLER:
			str << "DC ";
			break;
		}

		if( info.wSuiteMask & VER_SUITE_BACKOFFICE )
		{ str << "BackOffice "; }
		if( info.wSuiteMask & VER_SUITE_BLADE )
		{ str << "SuiteBlade "; }
		if( info.wSuiteMask & VER_SUITE_COMPUTE_SERVER )
		{ str << "ComputeServer "; }
		if( info.wSuiteMask & VER_SUITE_DATACENTER )
		{ str << "Datacenter "; }
		if( info.wSuiteMask & VER_SUITE_ENTERPRISE )
		{ str << "Enterprise "; }
		if( info.wSuiteMask & VER_SUITE_EMBEDDEDNT )
		{ str << "Embedded "; }
		if( info.wSuiteMask & VER_SUITE_PERSONAL )
		{ str << "Personal " ;}
		if( info.wSuiteMask & VER_SUITE_SINGLEUSERTS )
		{ str << "Single " ;}
		if( info.wSuiteMask & VER_SUITE_SMALLBUSINESS )
		{ str << "SmallBusiness "; }
		if( info.wSuiteMask & VER_SUITE_SMALLBUSINESS_RESTRICTED )
		{ str << "SmallBusiness(Restricted) "; }
		if( info.wSuiteMask & VER_SUITE_STORAGE_SERVER )
		{ str << "StrageServer "; }
		if( info.wSuiteMask & VER_SUITE_TERMINAL )
		{ str << "Terminal "; }

		str << "ServicePack " << info.wServicePackMajor << "." << info.wServicePackMinor;
	}
#undef win32
#else
	str << "unknown os.";

#endif
	return str.str();
}

// encode ///////////////////////////////////////////////////////////
std::string lui::encodeString( const std::string& in_str )
{
	std::ostringstream result;
	
	std::string::const_iterator it;
	for( it = in_str.begin(); it != in_str.end(); ++it )
	{
		if( isascii(*it) )
		{
			result << *it;
		}
		else
		{
			result << '%' << std::uppercase << std::hex << ( 0xFF&static_cast<unsigned>(*it) );
		}
	}
	return result.str();
}

// unicodeとの相互変換 ///////////////////////////////////////////////
#ifdef LUI_USE_WIN32
#   define LUI_MISC_FUNC_SYSTEMLANG ".ACP"
#else
#   define LUI_MISC_FUNC_SYSTEMLANG ""
#endif
const std::wstring lui::a2w( const std::string&  str )
{ return a2w( str, std::locale(LUI_MISC_FUNC_SYSTEMLANG) ); }
const std::string  lui::w2a( const std::wstring& str )
{ return w2a( str, std::locale(LUI_MISC_FUNC_SYSTEMLANG) ); }
const std::string lui::w2u( const std::wstring& src )
{ return w2a( src, std::locale(std::locale(LUI_MISC_FUNC_SYSTEMLANG), new boost::filesystem::detail::utf8_codecvt_facet )); }
const std::wstring lui::u2w( const std::string& src )
{ return a2w( src, std::locale(std::locale(LUI_MISC_FUNC_SYSTEMLANG), new boost::filesystem::detail::utf8_codecvt_facet )); }

const std::wstring lui::a2w( const std::string& src, const std::locale& l )
{
	typedef std::codecvt<wchar_t, char, std::mbstate_t> cc_t;
	const cc_t& cvt = std::use_facet<cc_t>(l);
	
	const unsigned int buflen = src.length()+1L;
	std::wstring temp;

	boost::scoped_array<wchar_t> buffer( new wchar_t[buflen] );
	std::mbstate_t state = {0};
	cc_t::result result;

	const char* const pwbegin = src.c_str();
	const char* const pwend = pwbegin + src.length();
	const char* pwnext = pwbegin;
	wchar_t* const pbegin = buffer.get();
	wchar_t* const pend = pbegin + buflen;
	wchar_t* pnext = pbegin;

	for(;;)
	{
		result = cvt.in(state, pwbegin, pwend, pwnext, pbegin, pend, pnext);
		temp.append(pbegin, pnext - pbegin);
		if(result == cc_t::ok) {
			break;
		}
		else if (result != cc_t::partial) {
			throw std::runtime_error("misc::a2w() convert failed.");
		}
	}
	return temp;
}

const std::string lui::w2a( const std::wstring& src, const std::locale& l )
{
	typedef std::codecvt<wchar_t, char, std::mbstate_t> cc_t;
	const cc_t& cvt = std::use_facet<cc_t>(l);

	const unsigned int buflen = src.length() * 4L + 1L;
	std::string temp;

	boost::scoped_ptr<char> buffer( new char[buflen] );
	std::mbstate_t state = {0};
	cc_t::result result;

	const wchar_t* const pwbegin = src.c_str();
	const wchar_t* const pwend = pwbegin + src.length();
	const wchar_t* pwnext = pwbegin;
	char* const pbegin = buffer.get();
	char* const pend = pbegin + buflen;
	char* pnext = pbegin;

	for(;;)
	{
		result = cvt.out(state, pwbegin, pwend, pwnext, pbegin, pend, pnext);
		temp.append(pbegin, pnext - pbegin);
		if(result == cc_t::ok) {
			break;
		}
		else if (result != cc_t::partial) {
			throw std::runtime_error("misc::w2a() convert failed.");
		}
	}

	return temp;
}
