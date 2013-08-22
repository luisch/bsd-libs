/*! @file udp_service.cpp

   @brief UDP通信のヘルパークラス

   Implementation of the administrative class for sdclib.
   This program has been written in C and C++.
   Distributed under the BSD License. (See accompanying file LICENSE.TXT.)
   Copyright (C) 2011 ゑ, All Rights Reserved.

   @author ゑ.
*/
#include "misc.h"
#include "exception.h"
#include "udp_service.h"

using namespace lui;

// client mode
udp_service::udp_service(
	const std::string& url,
	const std::string& port
) : socket( *this, boost::asio::ip::udp::v4() ),
    resolver( *this ),
    in_thr ( boost::bind(&udp_service::input_loop, this) ),
    out_thr( boost::bind(&udp_service::output_loop,this) ),
    buf_state(invalid)
{
	using boost::asio::ip::udp;
	udp::resolver::query query( udp::v4(), url, port );
	remote_ptr = *resolver.resolve(query);
}

// server mode
udp_service::udp_service( int port )
: socket(
	*this,
	boost::asio::ip::udp::endpoint(
		boost::asio::ip::udp::v4(), port
	)
  ),
  resolver( *this ),
  in_thr ( boost::bind(&udp_service::input_loop, this) ),
  out_thr( boost::bind(&udp_service::output_loop,this) ),
  buf_state(invalid)
{
}

udp_service::~udp_service()
{
	socket.shutdown( boost::asio::ip::udp::socket::shutdown_both );
	socket.close();

	in_thr.interrupt();
	out_thr.interrupt();
	in_thr.join();
	out_thr.join();
}

std::string udp_service::getInput()
{
	boost::recursive_mutex::scoped_lock l(lock);
	if( buf_state != input_data ){
		return "";
	}
	buf_state = invalid;
	return buffer;
}

void udp_service::putOutput( const std::string& out )
{
	boost::recursive_mutex::scoped_lock l(lock);
	buffer = out;
	buf_state = output_data;

	output_ready.notify_one();
}

std::string udp_service::sendRequest( const std::string& req, int wait )
{
	putOutput(req);

	boost::recursive_mutex::scoped_lock l(lock);
	while( buf_state != input_data )
	{
		if( !input_ready.timed_wait(l, boost::posix_time::milliseconds(wait) ) )
		{
			// timed out
			return std::string("");
		}
	}
	buf_state = invalid;
	return buffer;
}

void udp_service::input_loop()
{
	using boost::asio::ip::udp;

	const int max_size = 8192;
	boost::array<char,max_size> buf;

	try{for(;;)
	{
		std::memset( buf.data(), 0, max_size );

		boost::system::error_code error;
		std::size_t length = socket.receive_from(
			boost::asio::buffer(buf,max_size-1),
			remote_ptr,
			0,
			error
		);
		boost::this_thread::interruption_point();
		
		if( error /*&& error != boost::asio::error::would_block*/ ) {
			syslog( boost::system::system_error(error).what(), error_info );
		}

		boost::recursive_mutex::scoped_lock l(lock);
		buffer = buf.data();
		buf_state = input_data;
		input_ready.notify_one();
	}}
	catch(boost::thread_interrupted const&)
	{
		syslog( "input loop is interrupted.", debug_info );
	} 
}

void udp_service::output_loop()
{
	using boost::asio::ip::udp;
	boost::recursive_mutex::scoped_lock l(lock);

	try { for(;;)
	{
		while( buf_state != output_data ) {
			output_ready.wait(l);
		}

		boost::asio::streambuf sbuf;
		std::ostream os(&sbuf);
		os << buffer;

		buffer.resize(0);
		buf_state = invalid;

		boost::system::error_code error;
		std::size_t length = socket.send_to(
			boost::asio::buffer(sbuf.data()),
			remote_ptr,
			0,
			error
		);
		boost::this_thread::interruption_point();

		if( error ) {
			syslog( boost::system::system_error(error).what(), error_info );
		}
	}}
	catch(boost::thread_interrupted const&)
	{
		syslog( "output loop is interrupted.", debug_info );
	} 
}
