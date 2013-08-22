
#ifndef MISC_LIB_ZSTREAM_H
#define MISC_LIB_ZSTREAM_H

#include <iostream>
#include <cstring>
#include <zlib.h>
#include "type.hpp"

namespace lui {

template <class CH,class TR,int BSIZE>
class basic_zstreambuf : public std::basic_streambuf<CH,TR> {
	
public:
	// 出力コンストラクタ
	basic_zstreambuf(std::basic_ostream<CH,TR>& os)
	{
		in=NULL;
		out=&os;
		
		std::memset(buffer, 0, BSIZE*sizeof(CH));
		this->setp(buffer,buffer+BSIZE);
		this->setg(buffer,buffer+BSIZE,buffer+BSIZE);
	}
	// 入力コンストラクタ
	basic_zstreambuf(std::basic_istream<CH,TR>& is )
	{
		out=NULL; 
		in=&is;
		
		std::memset(buffer, 0, BSIZE*sizeof(CH));
		this->setp(buffer,buffer+BSIZE);
		this->setg(buffer,buffer+BSIZE,buffer+BSIZE);
	}
	// デストラクタ
	~basic_zstreambuf(void)
	{
	}

protected:
	// overflow
    typename std::basic_streambuf<CH,TR>::int_type overflow(typename std::basic_streambuf<CH,TR>::int_type c=TR::eof())
	{
		if( out )
		{
			assert( this->pptr()-buffer == BSIZE );
			compress();
			
			if( c != TR::eof() )
			{
				std::memset( buffer, 0, BSIZE*sizeof(CH) );
				this->setp(buffer,buffer+BSIZE);
				this->setg(buffer,buffer+BSIZE,buffer+BSIZE);
				
				*this->pptr() = TR::to_char_type(c);
				this->pbump(1);
				
				return TR::not_eof(c);  
			}
		}
		return TR::eof();
	}

	// underflow
	typename std::basic_streambuf<CH,TR>::int_type underflow(void)
	{
		if( in )
		{
			const int s = decompress();
			if( s <= 0 ) {
				return TR::eof();
			}
			// no convert with codecvt.
			this->setg( buffer, buffer, buffer+s );
			return TR::to_int_type(*this->gptr());
		}
		return TR::eof();
	}

	// sync
	int sync(void)
	{
		if(out){
			compress();
		}
		return 0;
	}

protected:
	// compress
	void compress()
	{
		CH dst[BSIZE];
		uLongf dst_size = BSIZE;
		uLongf src_size = static_cast<uLongf>( sizeof(CH)*(this->pptr()-buffer) );
		compress2(
			reinterpret_cast<unsigned char*>(dst), &dst_size,
			reinterpret_cast<unsigned char*>(buffer), src_size,
			Z_BEST_SPEED
		);
		lui::type::ui32 dsize = static_cast<lui::type::ui32>(dst_size);
		out->write(reinterpret_cast<char*>(&dsize),sizeof(dsize));
		out->write(dst,dsize);
	}

	// decompress
	uLongf decompress(void)
	{
		CH src[BSIZE];
		lui::type::ui32 src_size = 0;
		uLongf dst_size = BSIZE;
		if( 
			!in->read(reinterpret_cast<char*>(&src_size),sizeof(src_size)).eof() &&
			!in->read(reinterpret_cast<char*>(src), src_size).eof()
		){
			uncompress(
				reinterpret_cast<unsigned char*>(buffer), &dst_size,
				reinterpret_cast<unsigned char*>(src),     src_size
			);
			return dst_size;
		}
		return 0;
	}

private:
	std::basic_ostream<CH,TR>* out;
	std::basic_istream<CH,TR>* in;

	CH buffer[BSIZE];
};

// 非圧縮→圧縮
template <class CH,class TR=std::char_traits<CH>,int BSIZE=4096 >
class basic_ozstream : public std::basic_ostream<CH,TR> {
public:
	basic_ozstream(std::basic_ostream<CH,TR>& os)
		: std::basic_ostream<CH,TR>(new basic_zstreambuf<CH,TR,BSIZE>(os))
	{
	}

	~basic_ozstream(void)
	{
		this->flush();
		delete this->rdbuf();
	}
};

// 圧縮→非圧縮
template <class CH,class TR=std::char_traits<CH>,int BSIZE=4096 >
class basic_izstream : public std::basic_istream<CH,TR> {
public:
	basic_izstream(std::basic_istream<CH,TR>& is)
		: std::basic_istream<CH,TR>(new basic_zstreambuf<CH,TR,BSIZE>(is))
	{
	}

	~basic_izstream(void)
	{
		delete this->rdbuf();
	}
};

typedef basic_ozstream<char> ozstream;
typedef basic_izstream<char> izstream;
}
#endif//MISC_LIB_ZSTREAM_H
