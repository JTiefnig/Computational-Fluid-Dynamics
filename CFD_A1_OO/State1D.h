#pragma once
class State1D
{
public:
	double rho;
	double rho_u;
	double e;

	State1D(double Rho, double Rho_U, double E);
	State1D(const State1D &mod);
	State1D();
	~State1D();

	State1D operator+(const State1D &b);
	State1D operator-(const State1D &b);
	State1D operator*(double c);
	State1D operator/(double c);

	State1D absComponents();

	virtual void operator=(const State1D &b);

};

