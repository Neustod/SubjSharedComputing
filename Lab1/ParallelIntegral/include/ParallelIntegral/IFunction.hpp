#pragma once


class _declspec(dllexport) IFunction
{
public:
	virtual ~IFunction() {};
	virtual double Calculate(double x) = 0;
};