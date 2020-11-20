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

	virtual std::string toString() const
	{
		return "f(z) = z";
	}
};

class QuadraticPolynomial : public ComplexFunctor
{
public:
	ComplexNumber operator()(ComplexNumber& z, ComplexNumber& c)
	{
		return z * z + c;
	}

	virtual std::string toString() const
	{
		return "f(z) = z² + c";
	}
};

class CubicPolynomial : public ComplexFunctor
{
public:
	ComplexNumber operator()(ComplexNumber& z, ComplexNumber& c)
	{
		return z * z * z + c;
	}

	virtual std::string toString() const
	{
		return "f(z) = z³ + c";
	}
};

class QuarticPolynomial : public ComplexFunctor
{
public:
	ComplexNumber operator()(ComplexNumber& z, ComplexNumber& c)
	{
		return z * z * z * z + c;
	}

	virtual std::string toString() const
	{
		return "f(z) = z^4 + c";
	}
};

class CubicInverseScaled : public ComplexFunctor
{
public:
	ComplexNumber operator()(ComplexNumber& z, ComplexNumber& c)
	{
		return c / (c*c) * (z * z * z + z * z + z);
	}

	virtual std::string toString() const
	{
		return "f(z) = 1/c * (z³ + z² + z)";
	}
};