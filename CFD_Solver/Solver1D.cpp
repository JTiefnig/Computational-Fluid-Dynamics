#include "stdafx.h"
#include "Solver1D.h"

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif


Solver1D::Solver1D(Model1D * model)
{
	this->model = model;
	cfl = model->properties["cfl"];
	
	//model->properties.createProperty("cfl", cfl);
}

Solver1D::~Solver1D()
{
}

void Solver1D::DoSteps(int i)
{
	for (; i > 0; i--)
	{
		this->DoStep();
	}
}


double Solver1D::TimeStep()
{

	int imax = model->u.size();

	double eigenmax = 0.01;

	for (int i = 0; i < imax; i++)
	{

		// old
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
	model->time += dt;
	return dt;
}