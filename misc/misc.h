/*! @file misc.h

   @brief プリコンパイル用のヘッダファイル

   THE HEADER-FILE for PRECOMPILE on MISC LIBRARY.
   This program has been written in C and C++.
   Distributed under the BSD License. (See accompanying file LICENSE.TXT.)
   Copyright (C) 2008 , All Rights Reserved.

   @author .
*/

/////////////////////////////////////////////////////////////////////
// misc.h - for precompile -
#ifndef LUI_MISC_LIB_H
#define LUI_MISC_LIB_H

// C library ////////////////////////////////////////////////////////
#include <cassert>
#include <cctype>

// STL //////////////////////////////////////////////////////////////
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <list>
#include <algorithm>
#include <locale>
#include <stdexcept>

// boost
#include <boost/config/warning_disable.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/timer.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/system/error_code.hpp>

// Windows //////////////////////////////////////////////////////////
#ifdef LUI_USE_WIN32
#	pragma warning(disable : 4819)

#	ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#	define WIN32_EXTRA_LEAN
#	endif

#	ifndef NOMINMAX
#	define NOMINMAX
#	endif

#	include <windows.h>
#	include <shellapi.h>
#	include <shlobj.h>
#endif

// for export //
#ifdef LUI_USE_WIN32
#	ifndef LUI_EXPORT
#		define LUI_EXPORT __declspec(dllexport)
#	endif
#else
#	define LUI_EXPORT
#endif

#endif//ML_MISC_LIB_H
