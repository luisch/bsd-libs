#include "stdafx.h"
#include "check.h"
#include "../exception.h"


int exceptionLogFilter( const std::string& str, lui::log_level lv );

/*! exceptionの動作チェック用パラメータクラス(フィクスチャークラス)
*/
class exceptionTest : public testing::Test {
protected:
	virtual void SetUp() {
	}
	virtual void TearDown() {
	}
	
	void loggingAndCheck( const char* msg, lui::log_level lv )
	{
		for( int l=lui::debug_info; l <= lui::critical_info; ++l )
		{
			std::stringbuf buf;
			lui::setOutput( &buf );
			lui::setTraceLevel( lui::log_level(l) );
			lui::syslog(msg,lv);
			
			if( l <= lv ) {
				EXPECT_TRUE( buf.str().size() > 0 );
				EXPECT_TRUE(std::string::npos != buf.str().find(msg));
			}
			else {
				EXPECT_TRUE(std::string::npos == buf.str().find(msg));
			}
			lui::setOutput( NULL );
		}
	}
};

TEST_F(exceptionTest,logging)
{
	loggingAndCheck("test", lui::debug_info);
	loggingAndCheck("test", lui::trace_info);
	loggingAndCheck("test", lui::warning_info);
	loggingAndCheck("test", lui::error_info);
	loggingAndCheck("test", lui::critical_info);
	loggingAndCheck("test", lui::all_info);
}
TEST_F(exceptionTest,duplog)
{
	std::stringbuf buf;
	lui::setOutput( &buf );
	lui::setTraceLevel( lui::debug_info );
	
	lui::syslog("test");
	lui::syslog("test");
	lui::syslog(std::string("test"));
	lui::syslog(L"test");
	lui::syslog(std::wstring(L"test"));
	lui::syslog("test-end");
	
	EXPECT_TRUE(
		std::string::npos != buf.str().find("last message repeated 4 times")
	);
}
