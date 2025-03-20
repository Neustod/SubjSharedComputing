#include <iostream>
#include "Client/EndpointService.hpp"

namespace io = boost::asio;
using io::ip::tcp;


int main(int argc, char* argv[])
{
	std::string ipAddress;
	size_t port{ 0 };
	size_t core_count;

	if (argc == 4) 
	{
		ipAddress = argv[1];
		port = atoll(argv[2]);
		core_count = atoll(argv[3]);
	}
	else 
	{
		std::cout << "Usage: " << argv[0] << " <ip_address> <port> <cores_amount>" << std::endl;
		return -1;
	}

	io::io_context io_context;
	tcp::endpoint ep(io::ip::make_address_v4(ipAddress), port);

	EndpointService eps(io_context, ep, 4);

	io_context.run();

	system("pause");
	return 0;
}

