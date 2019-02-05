#pragma once

#include "State1D.h"
#include "GridPoint1D.h"

#include "propertyContainer.h"
#include "smartProperty.h"

#include "propertyContainer.h"



#include <vector>

class GridGenerator;



// Autor Johannes Tiefnig
// CFD TU Graz final project 2018
class Model1D 
{
public:
	std::vector<State1D> delta_u;
	std::vector<GridPoint1D> u;

	PropertyContainer properties;


	double dx;

	// parameters 

	int stepcount;
	double time;

	int gridsize;

	smartProperty R;
	smartProperty gamma; // kappa
	smartProperty p_tot;
	smartProperty T_tot;
	smartProperty p_exit;
	smartProperty sub_exit;

	int nPrint = 9; // wir beginnen bei 0

	double rho_tot;

	

	Model1D(int gridSize);
	~Model1D();



	void Boundary();

	GridPoint1D& operator[](int i);


	double GetPressure(int i);
	double GetMach(int i);
	double GetTemperatur(int i);
	double GetVelocity(int i);
	double GetTotalPressure(int i);
	double GetContinuity(int i);


	double CalculateConvergence();


	std::vector<State1D> ToStateVector();

private:
	double convCompValue = 1;
};

