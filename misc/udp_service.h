/*! @file udp_service.h

   @brief UDP通信のヘルパークラス

   Implementation of the administrative class for sdclib.
   This program has been written in C and C++.
   Distributed under the BSD License. (See accompanying file LICENSE.TXT.)
   Copyright (C) 2011 ゑ, All Rights Reserved.

   @author ゑ.
*/
#ifndef ML_MISC_UDP_SERVICE_H
#define ML_MISC_UDP_SERVICE_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace lui
{
	class udp_service : public boost::asio::io_service
	{
	private:
		boost::asio::ip::udp::socket   socket;
		boost::asio::ip::udp::resolver resolver;
		boost::asio::ip::udp::endpoint remote_ptr;
		
		boost::recursive_mutex lock;
		boost::condition_variable_any output_ready, input_ready;
		
		volatile enum buffer_state {
			invalid,
			output_data,
			input_data,
		} buf_state;

		std::string buffer;
		
		boost::thread in_thr, out_thr;

	public:
		// client mode
		udp_service( const std::string& url, const std::string& port );
		
		// server mode
		udp_service( int port );

		// destruct
		~udp_service();

	public:
		std::string getInput();
		void putOutput( const std::string& out );

		std::string sendRequest( const std::string& req, int wait );
		
	private:
		
		int send_to();
		int receive_from();
		
		void input_loop();
		void output_loop();
	};
}
#endif
