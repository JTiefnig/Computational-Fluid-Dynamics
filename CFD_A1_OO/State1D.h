#pragma once
#include "MatrixXd.h"


// Autor Johannes Tiefnig
// CFD TU Graz final project 2018
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

	double & operator[](int i);


	// Werden f�r den ROE solver eingef�hrt
	operator MatrixXd ();
	void operator = (MatrixXd& mat);

	State1D absComponents();

	virtual void operator=(const State1D &b);

};

