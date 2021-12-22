#include "complex.h"
#include "cmath"

Complex::Complex()
{
	Re = 0;
	Im = 0;
}

Complex::Complex(const double &t_num)
{
	Re = t_num;
	Im = 0;
}

Complex::Complex(const double &t_Re, const double &t_Im)
{
	Re = t_Re;
	Im = t_Im;
}

Complex::Complex(const Complex &t_other)
{
	Re = t_other.Re;
	Im = t_other.Im;
}

Complex Complex::exp(const double &t_index)
{
	return Complex(std::cos(t_index), std::sin(t_index));
}

double Complex::getRe() const
{
	return Re;
}

double Complex::getIm() const
{
	return Im;
}

Complex Complex::getConjugate() const
{
	return Complex(Re, -Im);
}

double Complex::getModule() const
{
	return std::sqrt(Re * Re + Im * Im);
}

Complex Complex::operator*(const Complex &t_other) const
{
	return Complex(Re * t_other.Re - Im * t_other.Im,
				   Re * t_other.Im + Im * t_other.Re);
}

Complex Complex::operator*(const double &t_num) const
{
	return Complex(Re * t_num, Im * t_num);
}

Complex Complex::operator/(const Complex &t_other) const
{
	Complex numerator = *(this) * t_other.getConjugate();
	Complex denominator = t_other * t_other.getConjugate();

	return numerator / denominator.Re;
}

Complex Complex::operator/(const double &t_num) const
{
	return Complex(Re / t_num, Im / t_num);
}

Complex Complex::operator+(const Complex &t_other) const
{
	return Complex(Re + t_other.Re, Im + t_other.Im);
}

Complex Complex::operator+(const double &t_num) const
{
	return Complex(Re + t_num, Im);
}

Complex Complex::operator-(const Complex &t_other) const
{
	return Complex(Re - t_other.Re, Im - t_other.Im);
}

Complex Complex::operator-(const double &t_num) const
{
	return Complex(Re - t_num, Im);
}

Complex& Complex::operator=(const Complex &t_other)
{
	Re = t_other.Re;
	Im = t_other.Im;

	return *this;
}

Complex& Complex::operator=(const double &t_num)
{
	Re = t_num;
	Im = 0;

	return *this;
}
