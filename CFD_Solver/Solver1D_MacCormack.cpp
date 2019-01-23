#include "stdafx.h"
#include "Solver1D_MacCormack.h"


using namespace std;

Solver1D_MacCormack::Solver1D_MacCormack(Model1D* mod)
	: Solver1D(mod)
{
	int imax = mod->u.size();

	u_q = vector<State1D>(imax);
	u_qq = vector<State1D>(imax);

	dt = 0;
	
}


Solver1D_MacCormack::~Solver1D_MacCormack()
{
}

void Solver1D_MacCormack::DoStep()
{
	int imax = model->u.size();

	dt = TimeStep();

	Calc_uq();
	Boundary_q();

	Calc_uqq();

	for (int i = 1; i < imax - 1; i++)
	{	  
		(*model)[i] = (u_q[i] + u_qq[i]) * 0.5;	
	}

	model->Boundary();


}




void Solver1D_MacCormack::Calc_uq()
{
	vector<GridPoint1D> & u = model->u;
	int imax = u.size();


	for (int i = 1; i < imax; i++)
	{
		u_q[i] = u[i] - (Calc_f(i) - Calc_f(i - 1)) *dt / model->dx + Calc_source(i)*dt;

	}
}

void Solver1D_MacCormack::Boundary_q()
{
	vector<GridPoint1D> & u = model->u;
	int imax = u.size();

	u_q[0] = u_q[1]*2 - u_q[2];

	u_q[imax-1] = u_q[imax-2]*2 - u_q[imax - 3];



}

void Solver1D_MacCormack::Calc_uqq()
{
	vector<GridPoint1D> & u = model->u;
	int imax = u.size();


	for (int i = 0; i < imax-1; i++)
	{
		u_qq[i] = u[i] - (Calc_f_q(i + 1) - Calc_f_q(i)) *dt / model->dx + Calc_source_q(i)*dt;
	}


}


State1D Solver1D_MacCormack::Calc_f(int i)
{
	State1D f;

	vector<GridPoint1D> & u = model->u;
	int imax = u.size();



	double local_u = u[i].rho_u / u[i].rho;
	double local_p = (model->gamma - 1)  * (u[i].e - u[i].rho * pow(local_u, 2) * 0.5);


	f.rho = u[i].rho_u;
	f.rho_u = u[i].rho_u * local_u + local_p;
	f.e = (u[i].e + local_p) * local_u;

	return f;
}


State1D Solver1D_MacCormack::Calc_f_q(int i)
{

	State1D f;

	double local_u = u_q[i].rho_u / u_q[i].rho;
	double local_p = (model->gamma - 1)  * (u_q[i].e - u_q[i].rho * pow(local_u, 2) * 0.5);


	f.rho = u_q[i].rho_u;
	f.rho_u = u_q[i].rho_u * local_u + local_p;
	f.e = (u_q[i].e + local_p) * local_u;

	return f;

}

State1D Solver1D_MacCormack::Calc_source(int i)
{
	State1D s;


	vector<GridPoint1D> & u = model->u;

	double local_u = u[i].rho_u / u[i].rho;
	double local_p = (model->gamma - 1)  * (u[i].e - u[i].rho * pow(local_u, 2) * 0.5);

	
	double local_k = -(model->u[i].da_dx) / (model->u[i].area);

	s.rho = u[i].rho_u * local_k;
	s.rho_u = u[i].rho_u * local_u*local_k;
	s.e = Calc_f(i).e * local_k;

	return s;
}

State1D Solver1D_MacCormack::Calc_source_q(int i)
{
	State1D s;


	vector<State1D> & u = u_q;

	double local_u = u[i].rho_u / u[i].rho;
	double local_p = (model->gamma - 1)  * (u[i].e - u[i].rho * pow(local_u, 2) * 0.5);


	double local_k = -(model->u[i].da_dx) / (model->u[i].area);

	s.rho = u[i].rho_u * local_k;
	s.rho_u = u[i].rho_u * local_u*local_k;
	s.e = Calc_f_q(i).e * local_k;

	return s;
}
