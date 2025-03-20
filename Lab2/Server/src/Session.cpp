#include "Server/Session.hpp"
#include <iostream>

namespace io = boost::asio;
using namespace std::placeholders;

#ifdef _DEBUG
#define DEBUG(x)
#else
#define DEBUG(x)
#endif // _DEBUG


Session::Session(tcp::socket&& socket) : _socket(std::move(socket))
{
	_coreAmount = 0;
	_partResult = 0;
}


void Session::start(task_handler&& onTask, result_handler&& onResult, error_handler&& onError)
{
	_onTask = std::move(onTask);
	_onResult = std::move(onResult);
	_onError = std::move(onError);

	io::async_read(_socket, io::buffer(&_coreAmount, sizeof(_coreAmount)), std::bind(&Session::onFirstRead, shared_from_this(), _1));
}

void Session::addTask(INTEGRAL_ARGS args) { _tasks.push(args); }

void Session::disconnect() { _socket.close(); }

void Session::asyncReadResults() 
{
	DEBUG(std::cout << "[DEBUG] Session.asyncRead: initiated" << std::endl;)
	io::async_read(_socket, io::buffer(&_partResult, sizeof(_partResult)), std::bind(&Session::onRead, shared_from_this(), _1));
}

void Session::asyncWriteArgs()
{
	DEBUG(std::cout << "[DEBUG] Session.asyncWrite: initiated" << std::endl;)

	if (_tasks.empty()) 
		_onTask();

	if (!_tasks.empty())
		io::async_write(_socket, io::buffer(&_tasks.front(), sizeof(INTEGRAL_ARGS)), std::bind(&Session::onWrite, shared_from_this(), _1));
}


void Session::onFirstRead(boost::system::error_code error)
{
	if (!error)
	{
		DEBUG(std::cout << "[DEBUG] Session.onFirstRead: initiated" << std::endl;)
		asyncWriteArgs();
		asyncReadResults();
	}
	else
	{
		_socket.shutdown(tcp::socket::shutdown_both);
		_socket.close();
		_onError();
	}
}

void Session::onRead(boost::system::error_code error)
{
	if (!error) 
	{
		DEBUG(std::cout << "[DEBUG] Session.onRead: initiated" << std::endl;)
		_tasks.pop();
		_onResult();

		asyncWriteArgs();
		asyncReadResults();
	}
	else 
	{
		_socket.shutdown(tcp::socket::shutdown_both);
		_socket.close();
		_onError();
	}
}

void Session::onWrite(boost::system::error_code error)
{
	if (!error)
	{
		DEBUG(std::cout << "[DEBUG] Session.onWrite: initiated" << std::endl;)
	}
	else
	{
		_socket.shutdown(tcp::socket::shutdown_both);
		_socket.close();
		_onError();
	}
}