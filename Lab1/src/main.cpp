#include <iostream>
#include "ParallelIntegral/Integrator.hpp"
#include <chrono>


class MyFunction : public IFunction { double Calculate(double x) { return cos(x); } };


int main()
{
	std::chrono::high_resolution_clock::time_point start, end;
	double deltaTime{ 0 };

	double leftBound{ 1 }, rightBound{ 2 }, step{ 1.0e-8 };
	double result{ 0 };

	auto& ptrFunction = std::make_shared<MyFunction>();
	Integrator cosIntegral;
	
	std::cout << "Left bound:  " << leftBound << std::endl;
	std::cout << "Right bound: " << rightBound << std::endl;
	std::cout << "Step:        " << step << std::endl <<std::endl;

	for (size_t cores = 1; cores <= cosIntegral.GetMaxCpuCount(); cores++)
	{
		cosIntegral.SetCpuCount(cores);

		start = std::chrono::high_resolution_clock::now();
		result = cosIntegral.Calculate(ptrFunction, leftBound, rightBound, step);
		end = std::chrono::high_resolution_clock::now();

		deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 1.0e+6;

		std::cout << "Cores:    " << cores << std::endl;
		std::cout << "Time(ms): " << deltaTime << std::endl;
		std::cout << "Result:   " << result << std::endl << std::endl;
	}

	system("pause");
	return 0;
}

