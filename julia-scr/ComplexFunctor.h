#pragma once

#include "ComplexNumber.h"

class ComplexFunctor
{
public:
	ComplexFunctor() = default;
	~ComplexFunctor() = default;

	virtual ComplexNumber operator()(ComplexNumber& z, ComplexNumber& c)
	{
		return z;
	}
};

class QuadraticPolynomial : public ComplexFunctor
{
	ComplexNumber operator()(ComplexNumber& z, ComplexNumber& c)
	{
		return z * z + c;
	}
};

class CubicPolynomial : public ComplexFunctor
{
	ComplexNumber operator()(ComplexNumber& z, ComplexNumber& c)
	{
		return z * z * z + c;
	}
};

class QuarticPolynomial : public ComplexFunctor
{
	ComplexNumber operator()(ComplexNumber& z, ComplexNumber& c)
	{
		return z * z * z * z + c;
	}
};

class CubicInverseScaled : public ComplexFunctor
{
	ComplexNumber operator()(ComplexNumber& z, ComplexNumber& c)
	{
		return c / (c*c) * (z * z * z + z * z + z);
	}
};