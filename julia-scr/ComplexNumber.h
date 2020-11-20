#pragma once

#include <string>

class ComplexNumber
{
public:
	ComplexNumber(double real, double imag);
	~ComplexNumber();

	void   print() const;

	ComplexNumber operator+(const ComplexNumber& c);
	ComplexNumber operator-(const ComplexNumber& c);
	ComplexNumber operator*(const ComplexNumber& c);
	ComplexNumber operator/(const ComplexNumber& c);

	ComplexNumber operator+(double n);
	ComplexNumber operator-(double n);
	ComplexNumber operator*(double n);
	ComplexNumber operator/(double n);

	ComplexNumber operator-();

	std::string toString();

	double real;
	double imag;
private:
};
