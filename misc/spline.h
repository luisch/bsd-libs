/*! @file spline.h

   @brief  ３次のスプライン曲線による数学的補完テンプレートライブラリ

   This is a library on MISC LIBRARY for software uses RAY-TRACING.
   Distributed under the BSD License. (See accompanying file LICENSE.TXT.)
   Copyright (C) 2005 , All Rights Reserved.

   @author ゑ.
*/
#ifndef RTC_RAYTRACE_SPLINE_INTERPOLATION_H
#define RTC_RAYTRACE_SPLINE_INTERPOLATION_H

namespace lui { namespace interpolation { // -----------------------------------------------------

	template < class D = double >
	class spline
	{
	private:
		template < class D >
		class spline_element
		{
			friend class spline<D>;

		public:
			spline_element( const D& _x, const D& _y )
			: x(_x),
			  y(_y),
			  b(0),c(0),d(0)
			{}
			D x, y;

		public:
			// algorithm用のメンバ関数
			bool operator <( const spline_element& r ) const
			{ return x < r.x; }

			struct upper_search
			{
				bool operator ()(
					const D                  b,
					const spline_element<D>& a
				) const {
					return a.x > b;
				}
			};

		private:
			D b, c, d;
		};
	
	public:
		typedef spline_element<D>        element_t;
		typedef std::vector< element_t > dataset_t;
		
	public:
		void add( const D& x, const D& y );
		void establish();
		
		const D get( const D& x ) const;
		
	private:
		dataset_t m_data;
	};

	#include "spline.inl"
}}

#endif
