// check.cpp : アプリケーションのエントリ ポイントを定義します。
//
#include "stdafx.h"
#include "check.h"
#include "../math.hpp"

/*! 平準化移動平均の算出テスト
*/
TEST(func,EMA)
{
	std::list<float> seed = boost::assign::list_of
		(1)(1)(1);
	EXPECT_NEAR(
		misc::math::exp_average(seed.begin(),seed.end(),0.f),
		1.f,
		1e-7
	);

	seed = boost::assign::list_of
		(1)(2)(3)(4)(5)(6)(7)(8)(9);
	EXPECT_NEAR(
		misc::math::exp_average(seed.begin(),seed.end(),0.f),
		5.671088,
		1e-5
	);

	seed = boost::assign::list_of
		(95.028)
		(94.972)
		(94.909)
		(95.037)
		(95.083)
		(94.970)
		(95.050)
		(94.895)
		(95.159)
		(95.385)
		(95.427)
		(95.227)
		(95.201)
		(95.277)
		(95.373)
		(95.357)
		(95.467)
		(95.608)
		(95.704)
		(95.555)
		(95.577)
		(95.530)
		(95.537)
		(95.463);
	EXPECT_NEAR(
		misc::math::exp_average(seed.begin(),seed.end(),0.f),
		95.3465424f,
		1e-6
	);
}

/*! 逆行列の導出(double)
*/
TEST(func,invert_double)
{
	namespace ublas = boost::numeric::ublas;

	typedef ublas::matrix<double> M;
	M m(3, 3);

	m(0, 0) = 1; m(0, 1) =  2; m(0, 2) =  3;
	m(1, 0) = 2; m(1, 1) =  1; m(1, 2) = -3;
	m(2, 0) = 4; m(2, 1) = -3; m(2, 2) =  2;

	M mi = misc::math::invert(m); // mi => m^{-1}

	ublas::identity_matrix<typename M::value_type> e(m.size1()); // 単位行列
	
	M r1 = boost::numeric::ublas::prod(m, mi); // 単位行列になるはず
	M r2 = boost::numeric::ublas::prod(mi, m); // 単位行列になるはず
	for( int x=0; x<3; ++x )
	{
		for( int y=0; y<3; ++y )
		{
			// 単位行列になるはず
			EXPECT_NEAR( x==y ? 1.0 : 0.0, r1(x,y), 1e-15 );
			EXPECT_NEAR( x==y ? 1.0 : 0.0, r2(x,y), 1e-15 );
		}
	}
}

/*! 逆行列の導出(float)
*/
TEST(func,invert_float)
{
	namespace ublas = boost::numeric::ublas;

	typedef ublas::matrix<float> M;
	M m(3, 3);

	m(0, 0) = 1.f; m(0, 1) =  2.f; m(0, 2) =  3.f;
	m(1, 0) = 2.f; m(1, 1) =  1.f; m(1, 2) = -3.f;
	m(2, 0) = 4.f; m(2, 1) = -3.f; m(2, 2) =  2.f;

	M mi = misc::math::invert(m); // mi => m^{-1}

	ublas::identity_matrix<typename M::value_type> e(m.size1()); // 単位行列
	
	M r1 = boost::numeric::ublas::prod(m, mi); // 単位行列になるはず
	M r2 = boost::numeric::ublas::prod(mi, m); // 単位行列になるはず
	for( int x=0; x<3; ++x )
	{
		for( int y=0; y<3; ++y )
		{
			// 単位行列になるはず
			EXPECT_NEAR( x==y ? 1.f : 0.f, r1(x,y), 1e-6f );
			EXPECT_NEAR( x==y ? 1.f : 0.f, r2(x,y), 1e-6f );
		}
	}
}


