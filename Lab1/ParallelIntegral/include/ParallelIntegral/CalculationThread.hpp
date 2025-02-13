#pragma once
#include "ParallelIntegral/IFunction.hpp"
#include <memory>


typedef struct _thread_args
{
	std::shared_ptr<IFunction> function;
	double leftBound, rightBound, step;
	double result;

	_thread_args() : function(nullptr), result(0)
	{
		leftBound = 0;
		rightBound = 0;
		step = 0;
	}

	explicit _thread_args(const std::shared_ptr<IFunction>& ptrFunction, double leftBound, double rightBound, double step)
	{
		function = ptrFunction;

		this->leftBound = leftBound;
		this->rightBound = rightBound;
		this->step = step;

		result = 0;
	}
} *LPTHREAD_ARGS, THREAD_ARGS;


void __stdcall CalculationThread(void* args);