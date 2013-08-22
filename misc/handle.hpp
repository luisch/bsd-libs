/*! @file handle.h

   @brief SDC-LIB ハンドル管理クラスのテンプレート定義

   Definition of class for internal handle in sdclib.
   This program has been written in C and C++.
   Distributed under the BSD License. (See accompanying file LICENSE.TXT.)
   Copyright (C) 2006 ゑ, All Rights Reserved.

   @author ゑ.
*/
#ifndef MISC_SDC_SDCLIB_HANDLE_HPP
#define MISC_SDC_SDCLIB_HANDLE_HPP

#include <utility>
#include <hash_map>
#include <queue>
#include <boost/thread.hpp>

namespace lui {
	//! ハンドルの型
	typedef int handle;
	//! ハンドルの管理クラス
	/*!
		あるオブジェクトのポインタにハンドル値を割り当てる。
	*/
	template<
		class IT, //!< 管理するときの対象となる物のイテレータ（通常はポインタ）を指定
		int INITIAL_SIZE = 8192 //!< 初期のハンドルプールサイズを指定する。足りなくなったらこの単位で増える。
	>
	class handler
	{
	public:
		typedef IT                         container_t;
		typedef std::hash_map<handle,IT> active_list_t;
		typedef std::pair<handle,IT>            pair_t;
		typedef std::queue<handle>         free_list_t;

	private:
		typedef boost::recursive_mutex::scoped_lock scoped_lock;

	public:
		//! コンストラクタ
		handler()
		{
			for( int n = 0; n < INITIAL_SIZE; ++n ) {
				m_free.push(n);
			}
		};
		//! デストラクタ
		~handler() {};
		//! 新しいハンドルをitに結びつけ、割り当てられたハンドルを返す
		handle add( IT it )
		{
			scoped_lock L(m_lock);
			const handle h = reserveNextFree();
			m_active.insert(pair_t( h, it ));
			return h+1;
		};
		//! 既にあるハンドルと結びついているITを切り離す。
		/*!
			_hと結び付けられているオブジェクトを切り離す。
			切り離されたハンドルは再利用され、近い将来のaddで再び割り当てられる。
			無効なハンドルが指定されたときは、std::runtime_errorを投げる
		*/
		void del( const handle _h )
		{
			scoped_lock L(m_lock);
			const handle h = _h-1;
			if( m_active.find(h) != m_active.end() )
			{
				pushHandleFree(h);
				m_active.erase(h);
			}
			else throw std::runtime_error("handle: an invalid handle is deleted.");
		}
		//! hに結び付けられているオブジェクトのイテレータを返す
		/*!
			無効なハンドルが指定された場合、アクセス違反エラーとなることに注意する。
		*/
		IT seek( const handle h ) const
		{
			scoped_lock L(m_lock);
			return m_active.find(h-1)->second;
		}
		//! hに結び付けられているオブジェクトが存在するかを判別し、ある場合はイテレータを返す
		/*!
			@param[in]  h オブジェクトを取得したいハンドル値を指定する
			@param[out] out_it 有効なハンドルの場合、ここにオブジェクトの実体が格納される
			@ret 有効なハンドルが指定されたときtrue、無効なハンドルであった場合はfalseが帰る
		*/
		bool seek( const handle h, IT& out_it ) const
		{
			scoped_lock L(m_lock);
			typename active_list_t::const_iterator it = m_active.find(h-1);
			if( m_active.end() != it )
			{
				out_it = it->second;
				return true;
			}
			else {
				return false;
			}
		}
		
		const active_list_t& getActiveList() const {
			return m_active;
		}
		
	private:
		//! フリーのハンドルを探し、返す
		/*!
			フリーのハンドルがハンドルが無かった場合、ハンドルを補充する。
		*/
		handle reserveNextFree()
		{
			if( m_free.empty() )
			{
				const int size = m_active.size();

				for( int n = 0; n < INITIAL_SIZE; ++n )
				{
					m_free.push( n+size );
				}
			}
			handle r = m_free.front();
			m_free.pop();
			
			return r;
		}
		//! 指定のハンドルをフリーのリストに加える
		void pushHandleFree( handle f )
		{
			m_free.push(f);
		}

	private:
		active_list_t m_active; //!< 嫁さんを持ったハンドルのリスト
		free_list_t   m_free;   //!< 嫁がいないハンドルのリスト

	private:
		mutable boost::recursive_mutex m_lock;
	};
}// namespace
#endif
