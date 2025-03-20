#pragma once
#include <functional>


class Integrator
{
private:
	size_t cpuCount, maxCpuCount;
	double leftBound, rightBound, step;

public:
	Integrator(size_t cpuCount = 0);

	Integrator(double leftBound, double rightBound, double step, size_t cpuCount = 0);

public:
	size_t GetCpuCount() { return cpuCount; }

	size_t GetMaxCpuCount() { return maxCpuCount; }

	size_t SetCpuCount(size_t cpuCount);

	double GetLeftBound() { return leftBound; }

	double GetRightBound() { return rightBound; }

	double GetStep() { return step; }

	void SetLeftBound(double leftBound) { this->leftBound = leftBound; }

	void SetRightBound(double rightBound) { this->rightBound = rightBound; }

	void SetStep(double step) { this->step = step; }

private:
	static double CalculationThread(const std::function<double(double)>& lambda, double left, double right, double step);

public:
	double Calculate(const std::function<double(double)>& lambda) const;
};
