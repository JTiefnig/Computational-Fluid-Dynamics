#pragma once
#include "State1D.h"

class GridPoint1D :
	public State1D
{
public:
	GridPoint1D();
	GridPoint1D(const GridPoint1D & gridPoint);
	GridPoint1D(double Rho, double Rho_U, double E, double Area, double X, double Da_dx);
	~GridPoint1D();

	double area;
	double x;
	double da_dx;


	void operator=(const State1D & state);


	


};

