#include "Client/Integrator.hpp"
#include "Client/EndpointService.hpp"
#include <future>

#ifdef _DEBUG
#include <iostream>
#define DEBUG(x)
#else
#define DEBUG(x)
#endif // _DEBUG


Integrator::Integrator(function&& fx, size_t cpuCount) : _fx(std::move(fx))
{
	maxCpuCount = std::thread::hardware_concurrency();
	this->cpuCount = (cpuCount < maxCpuCount&& cpuCount > 0 ? cpuCount : maxCpuCount);
}

size_t Integrator::SetCpuCount(size_t cpuCount)
{
	this->cpuCount = (cpuCount < maxCpuCount ? cpuCount : maxCpuCount);
	return this->cpuCount;
}

double Integrator::CalculationThread(const function& fx, INTEGRAL_ARGS args)
{
	auto currentState = args.lborder;
	double result{ 0 };
	
	while (currentState < args.rborder)
	{
		result += (fx(currentState) + fx(currentState + args.step)) * args.step / 2;
		currentState += args.step;
	}

	return result;
}

double Integrator::Calculate(const INTEGRAL_ARGS& args) const
{
	DEBUG(std::cout << "[DEBUG] Integrator.Calculate: started" << std::endl;)

	std::vector<std::future<double>> threads;
	threads.reserve(cpuCount);

	double part = std::abs(args.rborder - args.lborder) / cpuCount;
	double left, right;
	double result{ 0 };

	for (size_t i = 0; i < cpuCount; i++) 
	{
		left = i* part + args.lborder;
		right = (i + 1) * part + args.lborder;
		auto f = std::async(std::launch::async, &Integrator::CalculationThread, _fx, INTEGRAL_ARGS{ left, right, args.step });
		threads.emplace_back(std::move(f));
	}

	for (size_t i = 0; i < cpuCount; i++)
	{
		result += threads[i].get();
	}

	return result;
}
