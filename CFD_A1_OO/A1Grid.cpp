#include "stdafx.h"
#include "A1Grid.h"
#include <vector>


using namespace std;


A1Grid::A1Grid(Model1D* mod)
	: GridGenerator(mod)
{
	x_min = -0.33333;
	x_max = 1;
	y_min = 0.1;
	y_max = 0.2;
}


A1Grid::~A1Grid()
{
}


void A1Grid::Generate()
{

	// Generate Area
	vector<GridPoint1D> & u = model->u;

	

	int imax = u.size();

	double dx = (x_max - x_min) / imax;
	model->dx = dx;
	double 	local_k = (y_max - y_min) / (pow(x_max, 2));

	for (int i = 0; i < imax; i++)
	{
		u[i].x = (x_min + (i)* dx);

		u[i].area = (y_min + (y_max - y_min)*pow(model->u[i].x / x_max, 2));//(y_min + local_k * pow((*model)[i].x, 2));

		u[i].da_dx = (local_k * (*model)[i].x * 2);
	}

	// Init model


	model->stepcount = 0;

	model->rho_tot = model->p_tot / (model->R*model->T_tot);

	double rho_start = model->rho_tot;
	double rhou_start = 0;
	double e_start = model->p_tot / (model->gamma - 1);


	// Initialisieren des Stroemungsfeldes (Zustandsvektor U) mit den Ruhezustandswerten
	// Initialisation of the flow field (state vector U) with the stagnation values (=total values)




	for (int i = 0; i < imax; i++)
	{
		u[i].rho = rho_start;
		u[i].rho_u = rhou_start;
		u[i].e = e_start;
	}


}
