#pragma once
#include <functional>


using result_handler = std::function<void(double)>;
using function = std::function<double(double)>;

class EndpointService;


class Integrator
{
private:
	static Integrator* _instance;

	size_t cpuCount, maxCpuCount;
	function _fx;

public:
	typedef struct _integral_args
	{
		double lborder, rborder, step;
	} INTEGRAL_ARGS;

public:
	Integrator(function&& fx, size_t cpuCount = 0);

public:
	static Integrator& initialize(function&& fx, size_t cpuCount = 0)
	{
		if (_instance == nullptr)
		{
			_instance = new Integrator(std::move(fx), cpuCount);
		}

		return *_instance;
	}

	static Integrator& getInstance()
	{
		return *_instance;
	}

	static void deinitialize()
	{
		if (_instance != nullptr)
		{
			delete _instance;
			_instance = nullptr;
		}
	}

public:
	size_t GetCpuCount() const { return cpuCount; }

	size_t GetMaxCpuCount() const { return maxCpuCount; }

	size_t SetCpuCount(size_t cpuCount);

private:
	static double CalculationThread(const function& fx, INTEGRAL_ARGS args);

public:
	double Calculate(const INTEGRAL_ARGS& args) const;
};
