#pragma once
#include "IFunction.hpp"
#include <memory>


class _declspec(dllexport) Integrator
{
private:
	size_t cpuCount, maxCpuCount;

public:
	Integrator(size_t cpuCount = 0);

public:
	size_t GetCpuCount() { return cpuCount; }

	size_t GetMaxCpuCount() { return maxCpuCount; }

	size_t SetCpuCount(size_t cpuCount);

public:
	double Calculate(const std::shared_ptr<IFunction>& ptrFunction, double leftBound, double rightBound, double step) const;
};