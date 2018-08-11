#include "stdafx.h"
#include "Solver1D.h"

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif


Solver1D::Solver1D(Model1D * model)
{
	this->model = model;
	cfl = 0.09;
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
		// new

		/*double vel = model->u[i].rho_u / model->u[i].rho;
		double p = (model->u[i][3] - model->u[i][1] * pow(vel, 2)*0.5)*(model->gamma - 1);
		double c = sqrt(p / model->u[i][1] * model->gamma);
		double eigen = max(fabs(vel + c), fabs(vel - c));

		if (eigen > eigenmax)
		{
			eigenmax = eigen;
		}*/

	}

	double dt = cfl * model->dx / eigenmax;

	return dt;
}