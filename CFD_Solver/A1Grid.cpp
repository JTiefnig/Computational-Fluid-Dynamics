#include "stdafx.h"
#include "A1Grid.h"
#include <vector>
#include <math.h>


using namespace std;


A1Grid::A1Grid(Model1D* mod)
	: GridGenerator(mod)
{

	x_min_A1 = mod->properties["x_min_A1"];
	
	x_max_A1 = mod->properties["x_max_A1"];
	
	y_min_A1 = mod->properties["y_min_A1"];
	
	y_max_A1 = mod->properties["y_max_A1"];

}


A1Grid::~A1Grid()
{
}


void A1Grid::Generate()
{

	// Generate Area
	vector<GridPoint1D> & u = model->u;

	

	std::size_t imax = u.size();

	double dx = (x_max_A1 - x_min_A1) / imax;
	model->dx = dx;
	double 	local_k = (y_max_A1 - y_min_A1) / (pow(x_max_A1,2));

	

	for (std::size_t i = 0; i < imax; i++)
	{
		u[i].x = (x_min_A1 + (i)* dx);

		u[i].area = (y_min_A1 + (y_max_A1 - y_min_A1)*pow(model->u[i].x / x_max_A1, 2));//(y_min + local_k * pow((*model)[i].x, 2));

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




	for (std::size_t i = 0; i < imax; i++)
	{
		u[i].rho = rho_start;
		u[i].rho_u = rhou_start;
		u[i].e = e_start;
	}


}
