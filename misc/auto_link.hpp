/*! @file auto_link.hpp

   @brief ライブラリを状況に応じて適切に自動リンクする

   This program has been written in C++.
   Distributed under the BSD License. (See accompanying file LICENSE.TXT.)
   Copyright (C) 2011 ゑ, All Rights Reserved.

   @author ゑ.
*/

/*************************************************************************

USAGE:
~~~~~~

Before including this header you must define one or more of define the following macros:

LUI_LIB_NAME:           Required: A string containing the basename of the library,
                          for example sdclib.

***************************************************************************/
#ifndef LUI_LIB_NAME
#  error "Macro LUI_LIB_NAME not set (internal error)"
#endif

#ifndef LUI_USE_WIN32
#  error "misc/auto_link is not support without WIN32."
#endif

// DEBUG, TESTING or RELASE/UNICODE
#ifndef NDEBUG
#	define LUI_MISC_LIB_DEBUG_OPT "D"
#elif !defined LUI_RELEASE
#	define LUI_MISC_LIB_DEBUG_OPT "T"
#elif defined UNICODE
#	define LUI_MISC_LIB_DEBUG_OPT ""
#else
#	define LUI_MISC_LIB_DEBUG_OPT
#endif

#ifndef LUI_MISC_LIB_POST_NAME
#	define LUI_MISC_LIB_POST_NAME
#endif

#ifndef LUI_MISC_LIB_EXT_NAME
#	define LUI_MISC_LIB_EXT_NAME ".lib"
#endif

#pragma comment(lib, LUI_LIB_NAME LUI_MISC_LIB_DEBUG_OPT LUI_MISC_LIB_POST_NAME LUI_MISC_LIB_EXT_NAME )
//#pragma message("Linking to lib file: " LUI_LIB_NAME LUI_MISC_LIB_DEBUG_OPT LUI_MISC_LIB_POST_NAME LUI_MISC_LIB_EXT_NAME )


// undef all
#ifdef LUI_MISC_LIB_DEBUG_OPT
#	undef LUI_MISC_LIB_DEBUG_OPT
#endif

#ifdef LUI_LIB_NAME
#	undef LUI_LIB_NAME
#endif

#undef LUI_MISC_LIB_POST_NAME
#undef LUI_MISC_LIB_EXT_NAME
