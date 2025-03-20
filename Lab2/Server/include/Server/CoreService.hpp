#pragma once
#include <queue>
#include "Server/Session.hpp"
#include <boost/optional.hpp>


class CoreService
{
private:
	boost::asio::io_context& _io_context;
	tcp::acceptor _acceptor;
	boost::optional<tcp::socket> _socket;

	std::queue<INTEGRAL_ARGS> _lateQueue;
	uint32_t _fragmentation;
	uint32_t _performedTasks;

	double _part;
	size_t _partIndex;
	size_t _result_index;
	double _result;

	INTEGRAL_ARGS _args;

public:
	CoreService(boost::asio::io_context& io_context, std::uint32_t port);

	void start(uint32_t fragmentation, double leftBound, double rightBound, double step);

public:
	double result() const { return _result; }

private:
	void asyncAccept();

	void taskHandler(Session& client);

	void resultHandler(const Session& client);
};
