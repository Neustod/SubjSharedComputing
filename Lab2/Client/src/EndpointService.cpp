#include "Client/EndpointService.hpp"
#include <iostream>


#ifdef _DEBUG
#define DEBUG(x)
#else
#define DEBUG(x)
#endif // _DEBUG


namespace io = boost::asio;
using boost::asio::ip::tcp;
using namespace std::placeholders;
using boost::system::error_code;


EndpointService::EndpointService(boost::asio::io_context& io_context, const tcp::endpoint& endpoint, size_t coresUsage) : 
	_io_context(io_context), _endpoint(endpoint), _socket(io_context), _integrator([](double x) { return std::cos(x); }, coresUsage)
{
	_args = { 0.0, 0.0, 0.0 };
	_ll_buffer = _integrator.GetCpuCount();

	start();
}

void EndpointService::start()
{
	_socket.async_connect(_endpoint, 
		[&](const error_code& error)
		{
			if (!error) 
			{
				DEBUG(std::cout << "[DEBUG] async_connect: SUCCESS" << std::endl;)
				asyncFirstWrite();
			}
			else 
			{
				//std::cerr << "EndpointService.async_connect.callback: can't connect to the remote endpoint." << std::endl;
				_socket.close();

				start();
			}
		}
	);
}

void EndpointService::asyncFirstWrite() 
{
	io::async_write(_socket, io::buffer(&_ll_buffer, sizeof(_ll_buffer)),
		[&](const error_code& error, size_t bytes_transferred)
		{
			if (!error) 
			{
				DEBUG(std::cout << "[DEBUG] async_first_write: SUCCESS" << std::endl;)
				asyncRead();
			}
			else 
			{
				std::cerr << "EndpointService.asyncFirstWrite.async_write.callback: can't send data to the remote endpoint." << std::endl;
				_socket.close();

				start();
			}
		}
	);
}

void EndpointService::asyncRead() 
{
	io::async_read(_socket, io::buffer(&_args, sizeof(_args)),
		[&](const error_code& error, size_t bytes_transferred)
		{
			if (!error)
			{
				DEBUG(std::cout << "[DEBUG] async_read: SUCCESS" << std::endl;)
				asyncCalculate();
			}
			else
			{
				//std::cerr << "EndpointService.asyncRead.async_read.callback: can't receive data from the remote endpoint." << std::endl;
				_socket.close();

				start();
			}
		}
	);
}

void EndpointService::asyncWrite(double result) 
{
	_lf_buffer = result;
	std::cout << "Result = " << result << std::endl;

	io::async_write(_socket, io::buffer(&_lf_buffer, sizeof(_lf_buffer)),
		[&](const error_code& error, size_t bytes_transferred)
		{
			if (!error)
			{
				asyncRead();
			}
			else
			{
				//std::cerr << "EndpointService.async_write.callback: can't send data to the remote endpoint." << std::endl;
				_socket.close();

				start();
			}
		}
	);
}

void EndpointService::asyncCalculate()
{
	auto f = std::async(std::launch::async,
						[&, args = this->_args]() mutable
						{
							DEBUG(std::cout << "[DEBUG] Integrator.asyncCalculate: started" << std::endl;)

							auto result = _integrator.Calculate(_args);

							DEBUG(std::cout << "[DEBUG] Integrator.asyncCalculate: result calculated" << std::endl;)

							asyncWrite(result);
						}
	);
}
