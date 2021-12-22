#ifndef COMPLEX_H
#define COMPLEX_H


class Complex
{
public:
	Complex();
	Complex(const double &t_num);
	Complex(const double &t_Re, const double &t_Im);
	Complex(const Complex &t_other);

	static Complex exp(const double &t_index);

	double getRe() const;
	double getIm() const;
	Complex getConjugate() const;
	double getModule() const;

	Complex operator*(const Complex &t_other) const;
	Complex operator*(const double &t_num) const;

	Complex operator/(const Complex &t_other) const;
	Complex operator/(const double &t_num) const;

	Complex operator+(const Complex &t_other) const;
	Complex operator+(const double &t_num) const;

	Complex operator-(const Complex &t_other) const;
	Complex operator-(const double &t_num) const;

	Complex& operator=(const Complex &t_other);
	Complex& operator=(const double &t_num);

private:
	double Re;
	double Im;
};

#endif // COMPLEX_H
