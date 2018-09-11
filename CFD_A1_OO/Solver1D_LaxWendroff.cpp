#include "stdafx.h"
#include "Solver1D_LaxWendroff.h"
#include <math.h>

using namespace std;

Solver1D_LaxWendroff::Solver1D_LaxWendroff(Model1D* mod)
	: Solver1D(mod)
{
	int imax = mod->u.size();

	source = vector<State1D>(imax);
	f = vector<State1D>(imax);
	
	cfl = 0.05;

	dt = 0;
	time = 0;

}


Solver1D_LaxWendroff::~Solver1D_LaxWendroff()
{
}

void Solver1D_LaxWendroff::DoStep()
{
	int imax = model->u.size();

	dt = TimeStep();
	time += dt;

	vector<GridPoint1D> & u = model->u;

	CalcF();

	State1D prev_f_star = CalcF_star_LW(0);
	State1D f_star;

	for (int i= 1; i < imax-1; i++)
	{
		f_star = CalcF_star_LW(i);
		
		u[i] = u[i] + (f_star - prev_f_star) / model->dx *(-dt) + source[i] * dt;
		prev_f_star = f_star;

	}

	model->Boundary();

}




void Solver1D_LaxWendroff::CalcF()
{
	int imax = model->u.size();

	vector<GridPoint1D> & u = model->u;

	for (int i = 0; i < imax; i++)
	{
		double local_u = u[i].rho_u / u[i].rho;
		double local_p = (model->gamma - 1)  * (u[i].e - u[i].rho * pow(local_u, 2) * 0.5);

		f[i].rho = u[i].rho_u;
		f[i].rho_u = u[i].rho_u * local_u + local_p;
		f[i].e = (u[i].e + local_p) * local_u;

		double local_k = -(model->u[i].da_dx) / (model->u[i].area);

		/*source[i].rho = u[i].rho_u * local_k;
		source[i].rho_u = u[i].rho_u * local_u*local_k;
		source[i].e = f[i].e * local_k;*/

		double p = (u[i].e - u[i].rho * pow(u[i].rho_u / u[i].rho, 2)*0.5)*(model->gamma - 1);
		double rho = u[i].rho;
		double vel = u[i].rho_u / u[i].rho;
		source[i].rho = -u[i].da_dx / u[i].area * u[i].rho_u;
		source[i].rho_u = -u[i].da_dx / u[i].area * rho*pow(vel, 2);
		source[i].e = -u[i].da_dx / u[i].area * (u[i].e + p)*vel;
	}
}


State1D Solver1D_LaxWendroff::CalcF_star_LW(int i)
{
	State1D fStarCentral;

	double loc_area = ((*model)[i].area + (*model)[i + 1].area)*0.5;


	fStarCentral = (f[i + 1] + f[i])*0.5 - (f[i + 1] - f[i]) *0.5*dt/(model->dx)*loc_area;

	return fStarCentral;

}