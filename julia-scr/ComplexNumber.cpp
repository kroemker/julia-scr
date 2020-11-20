#include "ComplexNumber.h"

#include <cstdio>

ComplexNumber::ComplexNumber(double real, double imag) : real(real), imag(imag)
{
}

ComplexNumber::~ComplexNumber()
{
}

void ComplexNumber::print() const
{
	printf("%.2f + %.2f i\n", real, imag);
}

ComplexNumber ComplexNumber::operator+ (const ComplexNumber& c)
{
	ComplexNumber complexNumber(this->real + c.real, this->imag + c.imag);
	return complexNumber;
}

ComplexNumber ComplexNumber::operator- (const ComplexNumber& c)
{
	ComplexNumber complexNumber(this->real - c.real, this->imag - c.imag);
	return complexNumber;
}

ComplexNumber ComplexNumber::operator* (const ComplexNumber& c)
{
	double r = this->real * c.real - this->imag * c.imag;
	double i = this->real * c.imag + this->imag * c.real;
	ComplexNumber complexNumber(r, i);
	return complexNumber;
}

ComplexNumber ComplexNumber::operator/ (const ComplexNumber& c)
{
	double d = c.real * c.real + c.imag * c.imag;
	double r = this->real * c.real + this->imag * c.imag;
	double i = this->imag * c.real - this->real * c.imag;
	ComplexNumber complexNumber(r / d, i / d);
	return complexNumber;
}

ComplexNumber ComplexNumber::operator+ (double n)
{
	ComplexNumber complexNumber(this->real + n, this->imag);
	return complexNumber;
}

ComplexNumber ComplexNumber::operator- (double n)
{
	ComplexNumber complexNumber(this->real - n, this->imag);
	return complexNumber;
}

ComplexNumber ComplexNumber::operator* (double n)
{
	double r = this->real * n;
	double i = this->imag * n;
	ComplexNumber complexNumber(r, i);
	return complexNumber;
}

ComplexNumber ComplexNumber::operator/ (double n)
{
	double r = this->real / n;
	double i = this->imag / n;
	ComplexNumber complexNumber(r, i);
	return complexNumber;
}

ComplexNumber ComplexNumber::operator- ()
{
	ComplexNumber complexNumber(-this->real, -this->imag);
	return complexNumber;
}

std::string ComplexNumber::toString() 
{
	char n[20];
	sprintf_s(n, "%.2f + %.2f i", real, imag);
	return std::string(n);
}