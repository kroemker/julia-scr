#pragma once

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


	double real;
	double imag;
private:
};
