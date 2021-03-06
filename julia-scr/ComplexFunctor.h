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
		return "f(z) = z� + c";
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
		return "f(z) = z� + c";
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
		return "f(z) = 1/c * (z� + z� + z)";
	}
};

class QuadraticPolynomialSpecial : public ComplexFunctor
{
public:
	ComplexNumber operator()(ComplexNumber& z, ComplexNumber& c)
	{
		return z * z - z + c;
	}

	virtual std::string toString() const
	{
		return "f(z) = z� - z + c";
	}
};

class CubicPolynomialSpecial : public ComplexFunctor
{
public:
	ComplexNumber operator()(ComplexNumber& z, ComplexNumber& c)
	{
		return z * z * z - z * z + z + c;
	}

	virtual std::string toString() const
	{
		return "f(z) = z� - z� + z + c";
	}
};

class QuarticPolynomialSpecial : public ComplexFunctor
{
public:
	ComplexNumber operator()(ComplexNumber& z, ComplexNumber& c)
	{
		return z * z * z * z - z * z * z + z * z - z + c;
	}

	virtual std::string toString() const
	{
		return "f(z) = z^4 - z� + z� - z + c";
	}
};


class QuadraticPolynomialScaled : public ComplexFunctor
{
public:
	ComplexNumber operator()(ComplexNumber& z, ComplexNumber& c)
	{
		return c * c * z * z - c * z;
	}

	virtual std::string toString() const
	{
		return "f(z) = c�z� - c*z";
	}
};