#include "stdafx.h"
#include "Solver1D.h"



Solver1D::Solver1D(Model1D * model)
{
	this->model = model;
	cfl = 0.9;
}

Solver1D::~Solver1D()
{
}


double Solver1D::TimeStep()
{

	int imax = model->u.size();

	double eigenmax = 0;

	for (int i = 0; i < imax; i++)
	{
		double local_u = model->u[i].rho_u / model->u[i].rho;
		double local_p = (model->gamma - 1)  * (model->u[i].e - model->u[i].rho * pow(local_u, 2) * 0.5);
		double local_c = sqrt(model->gamma*local_p / model->u[i].rho);

		if (eigenmax < fabs(local_u + local_c))
		{
			eigenmax = fabs(local_u + local_c);
		}
		if (eigenmax < fabs(local_u - local_c))
		{
			eigenmax = fabs(local_u - local_c);
		}
	}

	double dt = cfl * model->dx / eigenmax;

	return dt;
}