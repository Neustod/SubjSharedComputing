#include "Lab1/Integrator.hpp"
#include <cmath>
#include <vector>
#include <future>


Integrator::Integrator(size_t cpuCount) : Integrator(0, 0, 0, 0) {}

Integrator::Integrator(double leftBound, double rightBound, double step, size_t cpuCount)
{
	maxCpuCount = std::thread::hardware_concurrency();
	this->cpuCount = (cpuCount < maxCpuCount&& cpuCount > 0 ? cpuCount : maxCpuCount);

	this->leftBound = leftBound;
	this->rightBound = rightBound;
	this->step = step;
}

size_t Integrator::SetCpuCount(size_t cpuCount)
{
	this->cpuCount = (cpuCount < maxCpuCount ? cpuCount : maxCpuCount);
	return this->cpuCount;
}

double Integrator::CalculationThread(const std::function<double(double)>& lambda, double left, double right, double step)
{
	auto currentState = left;
	double result{ 0 };
	
	while (currentState < right)
	{
		result += (lambda(currentState) + lambda(currentState + step)) * step / 2;
		currentState += step;
	}

	return result;
}

double Integrator::Calculate(const std::function<double(double)>& lambda) const
{
	std::vector<std::future<double>> threads(cpuCount);

	double part = std::abs(rightBound - leftBound) / cpuCount;
	double left, right;
	double result{ 0 };

	auto function = [](double x) { return cos(x); };

	for (size_t i = 0; i < cpuCount; i++) 
	{
		left = i* part + leftBound;
		right = (i + 1) * part + leftBound;
		threads[i] = std::async(std::launch::async, &Integrator::CalculationThread, lambda, left, right, step);
	}

	for (size_t i = 0; i < cpuCount; i++)
	{
		result += threads[i].get();
	}

	return result;
}
