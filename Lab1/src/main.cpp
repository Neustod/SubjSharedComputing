#include <cstdio>
#include <iostream>
#include "Lab1/Integrator.hpp"
#include <chrono>
#include <cmath>


int main()
{
	std::chrono::high_resolution_clock::time_point start, end;
	double deltaTime{ 0 };

	double result{ 0 };
	auto lambda = [](double x) { return std::cos(x); };

	Integrator integrator{ 1, 2, 1.0e-7 };

	std::cout << "Left bound:  " << integrator.GetLeftBound() << std::endl;
	std::cout << "Right bound: " << integrator.GetRightBound() << std::endl;
	std::cout << "Step:        " << integrator.GetStep() << std::endl <<std::endl;

	for (size_t cores = 1; cores <= integrator.GetMaxCpuCount(); cores++)
	{
		integrator.SetCpuCount(cores);

		start = std::chrono::high_resolution_clock::now();
		result = integrator.Calculate(lambda);
		end = std::chrono::high_resolution_clock::now();

		deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 1.0e+6;

		std::cout << "Cores:    " << cores << std::endl;
		std::cout << "Time(ms): " << deltaTime << std::endl;
		std::cout << "Result:   " << result << std::endl << std::endl;
	}

	std::cout << "Press any key..." << std::endl;
	getchar();
	return 0;
}

