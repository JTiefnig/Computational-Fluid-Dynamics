#pragma once

#include "State1D.h"
#include "GridPoint1D.h"

#include <vector>

using namespace std;



class Model1D
{
public:

	vector<GridPoint1D> u;
	vector<GridPoint1D> delta_u;

	double dx;


	// parameters 

	int stepcount;

	int gridsize;
	double R;
	double gamma; // kappa
	double p_tot;
	double T_tot;
	double p_exit;
	double sub_exit ;
	double rho_tot;
	


	Model1D(int gridSize);
	~Model1D();


	void Initialize();
	void Boundary();

	GridPoint1D& operator[](int i);


	double GetPressure(int i);

	float CalculateConvergence();

private:
	float convCompValue = 1;
};

