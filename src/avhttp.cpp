#include <iostream>
#include <boost/thread.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>

#include "avhttp.hpp"



typedef boost::coroutines::coroutine< void() >   coro_t;
typedef boost::coroutines::coroutine< void() >   int_coro_t;

template <typename T>
void echo(coro_t::caller_type &ca, T i)
{
	std::cout << i; 
	ca();
}

template <typename T>
void runit(coro_t::caller_type & ca, T i)
{
	std::cout << "started! ";
	for (T i = 0; i < 10; ++i)
	{
		int_coro_t c(boost::bind(echo<int>, _1, i));
// 		while ( c)
// 			c();
		ca();
	}
}


void handle_open(avhttp::http_stream &h, const boost::system::error_code &ec)
{
	if (!ec)
	{
		std::cout << "open succeed!\n";

// 		boost::array<char, 1024> buf;
// 		boost::system::error_code ec;
// 		std::size_t file_size = 0;
// 		while (!ec)
// 		{
// 			std::size_t bytes_transferred = 0;
// 			bytes_transferred = boost::asio::read(h, boost::asio::buffer(buf), ec);
// 			// std::size_t bytes_transferred = h.read_some(boost::asio::buffer(buf), ec);
// 			file_size += bytes_transferred;
// 			std::cout << buf.data();
// 			std::cout << file_size << std::endl;
// 		}
// 
// 		h.close(ec);
	}
}

template <typename H>
void test_handler_0(coro_t::caller_type &ca, H h, int v)
{
	h(v);
	ca();
}

template <typename H>
void test_handler_1(H h)
{
	coro_t c(boost::bind(&test_handler_0<H>, _1, boost::ref(h), 1045));
}

void test(int a)
{
	printf("%d", a);
}

int main(int argc, char* argv[])
{
	if (0)
	{
		{
			boost::function<void(int)> fu = boost::bind(&test, _1);
			test_handler_1(boost::bind(&test, _1));

			int_coro_t c(boost::bind(&runit<int>, _1, 100));
			while (c) {
				std::cout << "-";
				c();
			}
		}
		std::cout << "\nDone" << std::endl;

		return -1;
	}

	boost::asio::io_service io;
	avhttp::http_stream h(io);
	avhttp::request_opts opt;

	opt.insert("Connection", "close");
	h.request_options(opt);

	// h.open("http://w.qq.com/cgi-bin/get_group_pic?pic={64A234EE-8657-DA63-B7F4-C7718460F461}.gif");

	h.async_open("http://www.boost.org/LICENSE_1_0.txt",
		boost::bind(&handle_open, boost::ref(h), boost::asio::placeholders::error));

	io.run();

	return 0;
}

