#include "ParallelIntegral/CalculationThread.hpp"
#include <cmath>


void __stdcall CalculationThread(void* args)
{
	auto data = (LPTHREAD_ARGS)args;

	auto currentState = data->leftBound;
	double result = 0;

	if (data->function.get() == nullptr) 
		return;
	
	auto& function = *data->function.get();

	while (currentState < data->rightBound)
	{
		result += (function.Calculate(currentState) + function.Calculate(currentState + data->step)) * data->step / 2;
		currentState += data->step;
	}

	data->result = result;
}
