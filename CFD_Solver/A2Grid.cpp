#include "stdafx.h"
#include "A2Grid.h"
#include <vector>
using namespace std;


A2Grid::A2Grid(Model1D* mod)
	: GridGenerator(mod)
{
	// set default values
	y_gridA2 = mod->properties["y_gridA2"];
	
	x_min_gridA2 = mod->properties["x_min_gridA2"];
	
	x_max_gridA2 = mod->properties["x_max_gridA2"];
	
}


A2Grid::~A2Grid()
{
}

void A2Grid::Generate()
{

	vector<GridPoint1D> & u = model->u;

	int imax = u.size();

	double dx = (x_max_gridA2 - x_min_gridA2) / (imax);

	for (int i = 0; i < imax; i++)
	{
		u[i].x = x_min_gridA2 + dx * (i);
		u[i].area = y_gridA2;
		u[i].da_dx = 0;
	}

	model->stepcount = 0;
	model->time = 0;


	model->rho_tot = model->p_tot / (model->R*model->T_tot);


	for (int i = 0; i < imax; i++)
	{
		if (i < imax / 2)
		{
			u[i].rho = model->rho_tot;
			u[i].rho_u = 0;
			u[i].e = model->p_tot / (model->gamma - 1);
		}
		else
		{
			u[i].rho = model->p_exit / (model->R*model->T_tot);
			u[i].rho_u = 0;
			u[i].e = model->p_exit / (model->gamma - 1);
		}
	}
}
