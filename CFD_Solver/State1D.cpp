#include "stdafx.h"
#include "State1D.h"
#include <cmath>


State1D::State1D(double Rho, double Rho_U, double E)
{
	this->rho = Rho;
	this->rho_u = Rho_U;
	this->e = E;
}

State1D::State1D(const State1D & mod)
{
	rho = mod.rho;
	rho_u = mod.rho_u;
	e = mod.e;
}

State1D::State1D()
{
	this->rho = 0;
	this->rho_u = 0;
	this->e = 0;
}


State1D::~State1D()
{
}

State1D State1D::operator+(const State1D & b)
{
	State1D ret;
	ret.rho = this->rho + b.rho;
	ret.rho_u = this->rho_u + b.rho_u;
	ret.e = this->e + b.e;
	return ret;
}

State1D State1D::operator-(const State1D & b)
{
	State1D ret;
	ret.rho = this->rho - b.rho;
	ret.rho_u = this->rho_u - b.rho_u;
	ret.e = this->e - b.e;
	return ret;
}

State1D State1D::operator*(double c)
{
	State1D ret;
	ret.rho = this->rho * c;
	ret.rho_u = this->rho_u * c;
	ret.e = this->e*c;
	return ret;
}

State1D State1D::operator/(double c)
{
	State1D ret;
	ret.rho = this->rho / c;
	ret.rho_u = this->rho_u / c;
	ret.e = this->e / c;
	return ret;
}

double & State1D::operator[](int i)
{
	switch (i)
	{
	case 0:
		return rho;
		break;
	case 1:
		return rho_u;
		break;
	case 2:
		return e;
		break;
	default:
		break;
	}
	throw "Dim ERROR";
	// TODO: Exception 
}

State1D::operator MatrixXd()
{
	MatrixXd ret(3, 1);
	ret(0, 0) = rho;
	ret(1, 0) = rho_u;
	ret(2, 0) = e;
	return ret;
}

void State1D::operator=(const MatrixXd & mat)
{
	if (mat.m != 3 || mat.n != 1)
		throw "Dim ERROR";

	rho = mat(0, 0);
	rho_u = mat(1, 0);
	e = mat(2, 0);
}

State1D State1D::absComponents()
{
	State1D ret = State1D();
	
	ret.rho = std::abs(this->rho);
	ret.rho_u = std::abs(this->rho_u);
	ret.e =  std::abs(this->e);

	return ret;
}

void State1D::operator=(const State1D & b)
{
	this->rho = b.rho;
	this->rho_u = b.rho_u;
	this -> e = b.e;
}
