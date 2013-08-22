#include "stdafx.h"
#include "check.h"
#include "../zstream.hpp"


/*! zstreamの動作チェック用パラメータクラス(フィクスチャークラス)
*/
class zstreamTest : public testing::Test {
protected:
	virtual void SetUp() {
		file_name = "zfs.test";
	}
	virtual void TearDown() {
		std::remove(file_name.c_str());
	}

	std::string file_name; //!< テスト用に使うファイル名
};

/*! operator <<, >>による読み書き */
TEST_F(zstreamTest,operatorIntRW)
{
	const int p = 123456;

	// 書き込み
	{
		std::ofstream fs(file_name.c_str(),std::ios::binary);
		lui::ozstream oz(fs);
		oz << p;
	}
	// 読み取り
	{
		std::ifstream fs(file_name.c_str(),std::ios::binary);
		lui::izstream iz(fs);

		int d = 0;
		iz >> d;

		EXPECT_EQ(p,d);
	}
}
TEST_F(zstreamTest,operatorStringRW)
{
	const std::string p = "テスト用文字列";

	// 書き込み
	{
		std::ofstream fs(file_name.c_str(),std::ios::binary);
		lui::ozstream oz(fs);
		oz << p;
	}
	// 読み取り
	{
		std::ifstream fs(file_name.c_str(),std::ios::binary);
		lui::izstream iz(fs);

		std::string d;
		iz >> d;

		EXPECT_EQ(p,d);
	}
}

/*! read/writeによる読み書き */
TEST_F(zstreamTest,rawIntRW)
{
	const int p = 123456;

	// 書き込み
	{
		std::ofstream fs(file_name.c_str(),std::ios::binary);
		lui::ozstream oz(fs);
		oz.write(reinterpret_cast<const char*>(&p),sizeof(int));
	}
	// 読み取り
	{
		std::ifstream fs(file_name.c_str(),std::ios::binary);
		lui::izstream iz(fs);

		int d = 0;
		iz.read(reinterpret_cast<char*>(&d),sizeof(int));

		EXPECT_EQ(p,d);
	}
}
TEST_F(zstreamTest,rawStringRW)
{
	const std::string p = "テスト用文字列";

	// 書き込み
	{
		std::ofstream fs(file_name.c_str(),std::ios::binary);
		lui::ozstream oz(fs);
		int size = p.size();
		oz.write(reinterpret_cast<char*>(&size),sizeof(int));
		oz.write(p.c_str(),p.size());
	}
	// 読み取り
	{
		std::ifstream fs(file_name.c_str(),std::ios::binary);
		lui::izstream iz(fs);

		int size;
		iz.read(reinterpret_cast<char*>(&size),sizeof(int));
		boost::shared_ptr<char> d( new char[size+1] );
		std::memset( d.get(), 0, size+1 );
		iz.read(d.get(),size);

		EXPECT_EQ(p,std::string(d.get()));
	}
}
