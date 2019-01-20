#include "stdafx.h"
#include "GridPoint1D.h"


GridPoint1D::GridPoint1D()
{
	this->area = 0;
	this->da_dx = 0;
	this->x = 0;
}

GridPoint1D::GridPoint1D(const GridPoint1D & gridPoint)
	:State1D(gridPoint.rho, gridPoint.rho_u, gridPoint.e)
{
	
	this->area = gridPoint.area;
	this->da_dx = gridPoint.da_dx;
	this->x = gridPoint.x;
}


GridPoint1D::GridPoint1D(double Rho, double Rho_U, double E, double Area, double X, double Da_dx)
	: State1D(Rho, Rho_U, E)
{
	this->area = Area;
	this->da_dx = Da_dx;
	this->x = X;
}

GridPoint1D::~GridPoint1D()
{
}

void GridPoint1D::operator=(const State1D & state)
{
	this->rho = state.rho;
	this->rho_u = state.rho_u;
	this->e = state.e;
}
