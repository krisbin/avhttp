
#include <iostream>
#include <boost/array.hpp>
#include <avhttp.hpp>

void do_download(boost::asio::io_service& service,
	boost::asio::yield_context yield)

{
	avhttp::http_stream stream(service);

	boost::system::error_code ec;
	stream.yield_open("http://avplayer.org/", yield[ec]);
	if (ec)
	{
		std::cout << "Open url failed:" << ec.message() << std::endl;
		return;
	}

	for (;;)
	{
		boost::array<char, 1024> buffer;
		std::size_t read_size = stream.yield_read_some(boost::asio::buffer(buffer,buffer.size()), yield[ec]);

		std::cout.write(buffer.data(), read_size);
		if (ec)
		{
			if (ec == boost::asio::error::eof)
			{
				std::cout << "\n\nover!" << std::endl;
			}
			else
			{
				std::cout << "Error occured while reading:" << ec.message() << std::endl;
			}

			return;
		}
	}
}
int main(int argc,char* argv[])
{
	boost::asio::io_service service;

	boost::asio::spawn(service, boost::bind(&do_download, boost::ref(service), _1));

	service.run();
	return 0;
}