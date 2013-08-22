/*! @file data_stream.h

   @brief SDC-BASE データストリーム

   Data streaming library for SDC. To use this only with linked static.
   This program has been written in C and C++.
   Distributed under the BSD License. (See accompanying file LICENSE.TXT.)
   Copyright (C) 2004-2005 ゑ, All Rights Reserved.

   @author ゑ.
*/
#ifndef MISC_ML_SDC_FSTREAM_HPP
#define MISC_ML_SDC_FSTREAM_HPP

#include <ios>
#include <boost/shared_ptr.hpp>

namespace lui {

	/*!
		@brief 汎用streamを格納するためのインターフェイス
		環境によってstreamの実態が異なる場合に、それをシームレスに切り替える。
		streamをnewで確保しなければならない場合にもデストラクタで自動的に閉じられるようにできる。
	*/
	template< typename fs_type >
	class stream_ptr
	{
	public:
		//! コンストラクタ。basic_system::openData()のみ制御する
		stream_ptr( fs_type* fs )
			: m_fs(fs){};
		//! コピーコンストラクタ。
		stream_ptr( const stream_ptr<fs_type>& r )
			: m_fs( r.m_fs ){};
		//! fstreamのポインタをえる。
		fs_type* operator ->() { return this->stream(); }
		fs_type* stream() { return m_fs.get(); }

	private:
		boost::shared_ptr<fs_type> m_fs; //!< fstreamの共有ポインタ。コピーコンストラクタに対応のため。
	};
	
	typedef stream_ptr<std::istream> istream_ptr;
	typedef stream_ptr<std::ostream> ostream_ptr;

}//namepsace sdc

#endif//MISC_ML_SDC_FSTREAM_HPP
