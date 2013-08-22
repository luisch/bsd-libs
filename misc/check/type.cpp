// check.cpp : アプリケーションのエントリ ポイントを定義します。
//
#include "stdafx.h"
#include "check.h"
#include "../type.hpp"


/*! バイト固定変数が指定したとおりになっているか
*/
TEST(type,size)
{
	EXPECT_EQ(1, sizeof(lui::type::  i8));
	EXPECT_EQ(1, sizeof(lui::type:: ui8));
	EXPECT_EQ(2, sizeof(lui::type:: i16));
	EXPECT_EQ(2, sizeof(lui::type::ui16));
	EXPECT_EQ(4, sizeof(lui::type:: i32));
	EXPECT_EQ(4, sizeof(lui::type::ui32));
	EXPECT_EQ(8, sizeof(lui::type:: i64));
	EXPECT_EQ(8, sizeof(lui::type::ui64));
}
