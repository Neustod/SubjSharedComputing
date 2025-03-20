#include <iostream>
#include "Server/CoreService.hpp"


int main(int argc, char* argv[])
{
	size_t port;

	size_t fragmentation{ 1 };
	double lborder{ 1 }, rborder{ 2 }, step{ 1.0e-8 };
	double result;

	if (argc == 3) 
	{
		port = atoll(argv[1]);
		fragmentation = atoll(argv[2]);
	}
	else
	{
		std::cout << "Usage: " << argv[0] << " <port> <fragmentation>" << std::endl;
		return -1;
	}

	boost::asio::io_context io_context{};

	CoreService server(io_context, port);
	server.start(fragmentation, lborder, rborder, step);

	io_context.run();

	result = server.result();

	std::cout << "Integral cos(x) = " << result << std::endl;

	return 0;
}

