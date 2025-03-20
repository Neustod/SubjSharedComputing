#pragma once
#include <boost/asio.hpp>
#include <functional>
#include <queue>


typedef struct _integral_args
{
	double lBound, rBound, step;
} INTEGRAL_ARGS;

using task_handler = std::function<void()>;
using result_handler = std::function<void()>;
using error_handler = std::function<void()>;
using boost::asio::ip::tcp;


class Session : public std::enable_shared_from_this<Session>
{
private:
	tcp::socket _socket;

	task_handler _onTask;
	result_handler _onResult;
	error_handler _onError;

	size_t _coreAmount;
	double _partResult;
	std::queue<INTEGRAL_ARGS> _tasks;

public:
	Session(tcp::socket&& socket);

	void start(task_handler&& onTask, result_handler&& onResult, error_handler&& onError);

	void addTask(INTEGRAL_ARGS args);

	void disconnect();

public:
	size_t coreAmount() const { return _coreAmount; }

	double result() const { return _partResult; }

	const std::queue<INTEGRAL_ARGS>& tasks() const { return _tasks; }

private:
	void asyncReadResults();

	void asyncWriteArgs();

	void onFirstRead(boost::system::error_code error);

	void onRead(boost::system::error_code error);

	void onWrite(boost::system::error_code error);
};
