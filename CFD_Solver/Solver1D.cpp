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

	double eigenmax = 0;

	for (int i = 0; i < imax; i++)
	{

		// eq ...
		double local_u = model->u[i].rho_u / model->u[i].rho;

		// transformed eq. 2-5
		double local_p = (model->gamma - 1)  * (model->u[i].e - model->u[i].rho * pow(local_u, 2) * 0.5);

		// eq ... ideal gas speed of sound 
		double local_c = sqrt(model->gamma*local_p / model->u[i].rho);


		// eq 7-28
		// eu 7-29
		if (eigenmax < fabs(local_u + local_c))
		{
			eigenmax = fabs(local_u + local_c);
		}
		if (eigenmax < fabs(local_u - local_c))
		{
			eigenmax = fabs(local_u - local_c);
		}
	}
	

	if (eigenmax == 0)
		return 0; // schould throw an error... 

	// eq 15-1...
	double dt = cfl * model->dx / eigenmax;
	model->time += dt;
	return dt;
}