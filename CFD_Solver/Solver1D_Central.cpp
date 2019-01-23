#include "stdafx.h"
#include "Solver1D_Central.h"
#include <vector>
#include <math.h>

using namespace std;


Solver1D_Central::Solver1D_Central(Model1D* mod)
	:Solver1D(mod)
{
	int imax = mod->u.size();

	source = vector<State1D>(imax);
	f = vector<State1D>(imax);
	dissip = vector<State1D>(imax);

	

	eps_s = 10000.0;

}


Solver1D_Central::~Solver1D_Central()
{
}

void Solver1D_Central::DoStep()
{
	int imax = model->u.size();

	vector<GridPoint1D> & u = model->u;


	double dt = TimeStep();
	model->time += dt;

	CalcF();
	CalcDissipSimple();


	State1D f_star;
	State1D prv_fstar = CalcF_star_central(0);

	

	for (int i = 1; i < imax -1 ; i++)
	{

		f_star = CalcF_star_central(i);

		

		model->delta_u[i] = (f_star - prv_fstar) *(-dt) / model->dx + source[i] * dt;
 
		u[i] = u[i] + model->delta_u[i];

		prv_fstar = f_star;

	}

	model->Boundary();


}




void Solver1D_Central::CalcF()
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

		source[i].rho = u[i].rho_u * local_k;
		source[i].rho_u = u[i].rho_u * local_u*local_k;
		source[i].e = f[i].e * local_k;
	}
}

State1D Solver1D_Central::CalcF_star_central(int i)
{
	State1D fStarCentral;

	fStarCentral = (f[i + 1] + f[i])*0.5 + dissip[i];

	return fStarCentral;
}

void Solver1D_Central::CalcDissipSimple()
{
	int imax = model->u.size();

	double dx = model->dx;

	vector<GridPoint1D> & u = model->u;


	for (int i = 1; i < (imax - 2); i++)
	{
		dissip[i] =  (u[i + 2] -  u[i + 1] *3 +  u[i] *3 - u[i - 1]) *eps_s * dx;
	}

	dissip[0] = (u[1] - u[2] *2 + u[3])* eps_s * dx;

	dissip[imax - 2] = (u[imax - 2] -  u[imax - 3] *2 + u[imax - 4]) *eps_s * dx;
}

