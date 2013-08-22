/*! @file hash_map_list.hpp

   @brief std::hash_map<>とstd::listの融合コレクションクラス

   hash_map_list<> template library on MISC LIBRARY.
   This program has been written in C and C++.
   Distributed under the BSD License. (See accompanying file LICENSE.TXT.)
   Copyright (C) 2008 ゑ, All Rights Reserved.

   @author ゑ.
*/
/////////////////////////////////////////////////////////////////////
// hash_map_list
#ifndef LUI_MISC_LIB_HASH_MAP_LIST_H
#define LUI_MISC_LIB_HASH_MAP_LIST_H

#include <list>
#include <map>
#include <hash_map>


namespace lui {
	// hash_mapとlistの組み合わせで、挿入順にアクセス出来るがfindは速い。
	template< class Key, class V >
	class hash_map_list
	{
	public:
		typedef std::pair< Key, V > pair_t;
		typedef std::list< pair_t > list_t;
		typedef std::hash_map< Key, V > map_t;
		
		typedef typename list_t::iterator       iterator;
		typedef typename list_t::const_iterator const_iterator;
		
	public:
		hash_map_list()                              {}
		hash_map_list( int n ) : m_list(n), m_map(n) {}
		virtual ~hash_map_list()                     {}
		
	public:
		void push_front( Key k, V& v ) {
			m_list.push_front( typename list_t::value_type(k,v) );
			m_map.insert( typename map_t::value_type(k,v) );
		}
		void push_back( Key k, V& v ) {
			m_list.push_back( typename list_t::value_type(k,v) );
			m_map.insert( typename map_t::value_type(k,v) );
		}
		
		iterator       begin()       { return m_list.begin(); }
		const_iterator begin() const { return m_list.begin(); }
		iterator       end()         { return m_list.end(); }
		const_iterator end()   const { return m_list.end(); }
		
		V& at( Key h ) {
			return m_map.find(h)->second;
		}
		const V& at( const Key h ) const {
			return m_map.find(h)->second;
		}
		iterator find( Key h ) {
			return std::find(
				m_list.begin(), m_list.end(),
				typename list_t::value_type(h,m_map.find(h)->second)
			);
		}
		const_iterator find( const Key h ) const {
			return std::find(
				m_list.begin(), m_list.end(),
				typename list_t::value_type(h,m_map.find(h)->second)
			);
		}

		bool is_valid( Key h ) const {
			return m_map.find(h) != m_map.end();
		}
		
		iterator erase( iterator it )
		{
			m_map.erase(it->first);
			m_list.erase(it++);
			return it;
		}
		
		void clear() {
			m_list.clear();
			m_map.clear();
		}
		
		bool empty() const{
			return m_list.empty() && m_map.empty();
		}
		typename list_t::size_type size() const {
			assert( m_list.size() == m_map.size() );
			return m_list.size();
		}
		
	private:
		list_t m_list;
		map_t  m_map;
	};
}
#endif
