#include "ParallelIntegral/Integrator.hpp"
#include "ParallelIntegral/ThreadManager.hpp"
#include "ParallelIntegral/CalculationThread.hpp"


Integrator::Integrator(size_t cpuCount)
{
	SYSTEM_INFO siSystemInfo;
	GetSystemInfo(&siSystemInfo);

	maxCpuCount = siSystemInfo.dwNumberOfProcessors;

	this->cpuCount = (cpuCount < maxCpuCount && cpuCount > 0 ? cpuCount : maxCpuCount);
}

size_t Integrator::SetCpuCount(size_t cpuCount)
{
	this->cpuCount = (cpuCount < maxCpuCount ? cpuCount : maxCpuCount);
	return this->cpuCount;
}


double Integrator::Calculate(const std::shared_ptr<IFunction>& ptrFunction, double leftBound, double rightBound, double step) const
{
	if (ptrFunction.get() == nullptr) throw std::exception("Integrator.Calculate: nullptr exception.");

	ThreadManager threads(cpuCount);
	LPTHREAD_ARGS args{new THREAD_ARGS[cpuCount]};

	double part = abs(rightBound - leftBound) / cpuCount;
	double result{ 0 };

	for (size_t i = 0; i < cpuCount; i++) 
	{
		args[i] = THREAD_ARGS{ ptrFunction, i * part + leftBound, (i + 1) * part + leftBound, step };
		threads[i] = ThreadManager::CreateThread((_beginthreadex_proc_type)CalculationThread, &args[i]);
	}

	threads.Wait(true);

	for (size_t i = 0; i < cpuCount; i++)
	{
		result += args[i].result;
	}

	delete[] args;

	return result;
}
