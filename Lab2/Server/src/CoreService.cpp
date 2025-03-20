#include "Server/CoreService.hpp"
#include <iostream>

using boost::system::error_code;

#ifdef _DEBUG
#define DEBUG(x)
#else
#define DEBUG(x)
#endif // _DEBUG



CoreService::CoreService(boost::asio::io_context& io_context, std::uint32_t port) : 
	_io_context(io_context),
	_acceptor(io_context, tcp::endpoint(tcp::v4(), port)) 
{
	_performedTasks = 0;
	_fragmentation = 0;
	_part = 0;
	_partIndex = 0;
	_result_index = 0;
	_result = 0;
	_args = { 0.0, 0.0, 0.0 };
}


void CoreService::asyncAccept() 
{
	DEBUG(std::cout << "[DEBUG] Core.asyncAccept: initiated" << std::endl;)

	_socket.emplace(_io_context);
	
	_acceptor.async_accept
	(
		*_socket, 
		[&](error_code error) 
		{
			auto client = std::make_shared<Session>(std::move(*_socket));

			client->start(
				[&, client]() {
					DEBUG(std::cout << "[DEBUG] Core.onTask: initiated" << std::endl;)
					if (!_lateQueue.empty())
					{
						client->addTask(_lateQueue.front());
						_lateQueue.pop();
					}
					else
					{
						if (_args.lBound + _part * _partIndex >= _args.rBound)
						{
							return;
						}
						else
						{
							client->addTask(INTEGRAL_ARGS{ _args.lBound + _part * _partIndex, _args.lBound + _part * (_partIndex + 1), _args.step });
							++_partIndex;
						}
					}
				},
				[&, client]() {
					std::cout << "[DEBUG] Core.onResult: " << client->result() << std::endl;
					_result += client->result();
					
					if (++_result_index == _fragmentation)
						_io_context.stop();
				},
				[&, client]() {
					DEBUG(std::cout << "[DEBUG] Core.onError: initiated" << std::endl;)
					size_t queue_size = client->tasks().size();
					
					for (size_t i = 0; i < queue_size; i++) 
					{
						_lateQueue.push(client->tasks().front());
					}
				}
			);

			asyncAccept();
		}
	);
}

void CoreService::start(uint32_t fragmentation, double leftBound, double rightBound, double step)
{
	_result = 0;
	_performedTasks = 0;
	_fragmentation = fragmentation;

	_args = { leftBound, rightBound, step };
	_part = std::abs(rightBound - leftBound) / fragmentation;

	asyncAccept();
}
