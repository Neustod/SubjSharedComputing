#pragma once
#include <boost/asio.hpp>
#include "Client/Integrator.hpp"

using function = std::function<double(double)>;


class EndpointService : public std::enable_shared_from_this<EndpointService>
{
private:
	boost::asio::io_context& _io_context;

	boost::asio::ip::tcp::endpoint _endpoint;
	boost::asio::ip::tcp::socket _socket;

	Integrator _integrator;
	Integrator::INTEGRAL_ARGS _args;

	double _lf_buffer = 0.0;
	size_t _ll_buffer = 0;

public:
	EndpointService(boost::asio::io_context& io_context, const boost::asio::ip::tcp::endpoint& endpoint, size_t coresUsage = 0);

	void asyncWrite(double result);

private:
	void start();

	void asyncFirstWrite();

	void asyncRead();

	void asyncCalculate();

	void onConnect(boost::system::error_code error);

	void onWrite(boost::system::error_code error);

	void onRead(boost::system::error_code error);
};
