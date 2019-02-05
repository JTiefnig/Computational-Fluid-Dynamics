#include "stdafx.h"
#include "Solver1D_LaxWendroff.h"
#include <math.h>
#include <algorithm>
#include <functional>


using namespace std;

Solver1D_LaxWendroff::Solver1D_LaxWendroff(Model1D* mod)
	: Solver1D(mod)
{
	int imax = mod->u.size();

	source = vector<State1D>(imax);
	f = vector<State1D>(imax);
	dissip = vector<State1D>(imax);

	dt = 0;


}


Solver1D_LaxWendroff::~Solver1D_LaxWendroff()
{
}

void Solver1D_LaxWendroff::DoStep()
{
	int imax = model->u.size();

	dt = TimeStep();


	vector<GridPoint1D> & u = model->u;

	CalcDissipSimple();
	CalcF();

	State1D prev_f_star = CalcF_star_LW(0);
	State1D f_star;

	for (int i= 1; i < imax-1; i++)
	{
		f_star = CalcF_star_LW(i);
		
		model->delta_u[i] = (f_star - prev_f_star) / model->dx *(-dt) + source[i] * dt;
		u[i] = u[i] + model->delta_u[i];
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
		//double local_u = u[i].rho_u / u[i].rho;
		//double local_p = (model->gamma - 1)  * (u[i].e - u[i].rho * pow(local_u, 2) * 0.5);

		//f[i].rho = u[i].rho_u;
		//f[i].rho_u = u[i].rho_u * local_u + local_p;
		//f[i].e = (u[i].e + local_p) * local_u;

		//double local_k = -(model->u[i].da_dx) / (model->u[i].area);

		///*source[i].rho = u[i].rho_u * local_k;
		//source[i].rho_u = u[i].rho_u * local_u*local_k;
		//source[i].e = f[i].e * local_k;*/

		//double p = (u[i].e - u[i].rho * pow(u[i].rho_u / u[i].rho, 2)*0.5)*(model->gamma - 1);
		//double rho = u[i].rho;
		//double vel = u[i].rho_u / u[i].rho;
		//source[i].rho = -u[i].da_dx / u[i].area * u[i].rho_u;
		//source[i].rho_u = -u[i].da_dx / u[i].area * rho*pow(vel, 2);
		//source[i].e = -u[i].da_dx / u[i].area * (u[i].e + p)*vel;

		double p = (u[i].e - u[i].rho * pow(u[i].rho_u / u[i].rho, 2)*0.5)*(model->gamma - 1);
		double rho = u[i].rho;
		double vel = u[i].rho_u / u[i].rho;
		double temp = p / (rho*model->R);

		f[i].rho = u[i].rho_u;
		f[i].rho_u = rho * pow(vel, 2) + p;
		f[i].e = (u[i].e + p)*vel;

		source[i].rho = -u[i].da_dx / u[i].area * u[i].rho_u;
		source[i].rho_u = -u[i].da_dx / u[i].area * rho*pow(vel, 2);
		source[i].e = -u[i].da_dx / u[i].area * (u[i].e + p)*vel;



	}
}


State1D Solver1D_LaxWendroff::CalcF_star_LW(int i)
{
	State1D fStarCentral;

	double loc_area = ((*model)[i].area + (*model)[i + 1].area)*0.5;

	fStarCentral = (f[i + 1] + f[i])*0.5 - (f[i + 1] - f[i]) *0.5*dt / (model->dx)*loc_area + dissip[i];

	return fStarCentral;

}

void Solver1D_LaxWendroff::CalcDissipSimple()
{
	int imax = model->u.size();

	double dx = model->dx;

	vector<GridPoint1D> & u = model->u;


	for (int i = 1; i < (imax - 2); i++)
	{
		dissip[i] = (u[i + 2] - u[i + 1] * 3 + u[i] * 3 - u[i - 1]) *10000.0 * dx;
	}

	dissip[0] = (u[1] - u[2] * 2 + u[3])* 10000.0 * dx;

	dissip[imax - 1] = (u[imax - 2] - u[imax - 3] * 2 + u[imax - 4]) *10000.0 * dx;
}




void Solver1D_LaxWendroff::CalcDissipComplex()
{
	int imax = model->u.size();
	double dx = model->dx;
	vector<GridPoint1D> & u = model->u;
	vector<State1D> & du = model->delta_u;

	



	// just a little Helper lambda function
	function<State1D (State1D, State1D)> maxState = [](State1D a, State1D b) 
		{ 
			State1D ret; 
			ret.rho = a.rho > b.rho ? a.rho : b.rho;
			ret.rho_u = a.rho_u > b.rho_u ? a.rho_u : b.rho_u;
			ret.e = a.e > b.e ? a.e : b.e;
			return ret;
		};

	function<double (int)> pres = [this](int i)
	{
		return model->GetPressure(i);
	};

	function<vector<double> (vector<double>, double)> dDifRW = [](vector<double> in, double h)
	{
		vector<double> ret(in.size());
		for (int i = 1; i < in.size(); i++)
		{
			ret[i] = (in[i]-in[i-1]) / h;
		}
		// extrapolation
		ret[0] = 2 * ret[1] - ret[2];

		return ret;
	};

	function<vector<State1D> (vector<State1D>, double)> DifRW = [](vector<State1D> in, double h)
	{
		vector<State1D> ret(in.size());
		for (int i = 1; i < in.size(); i++)
		{
			ret[i] = (in[i] - in[i - 1]) / h;
		}
		// extrapolation
		ret[0] = ret[1] *2 - ret[2];

		return ret;
	};

	function<vector<State1D> (vector<State1D>, double)> DifFW = [](vector<State1D> in, double h)
	{
		vector<State1D> ret(in.size());
		int imax = in.size();
		for (int i = 0; i < in.size()-1; i++)
		{
			ret[i] = (in[i+1] - in[i]) / h;
		}
		// extrapolation
		ret[imax-1] = ret[imax-2]*2 - ret[imax-3];

		return ret;
	};


	// between 1/2 and 1/4
	double kapp2 = 1 / 3;
	// 1/64  -  1/32
	double kapp4 = 1 / 40;


	vector<double> ny(imax);
	vector<double> sigma(imax);
	vector<double> V(imax);


	
	for (int i = 0; i < imax; i++)
	{

		// just calc sigma
		double local_u = u[i].rho_u / u[i].rho;
		double local_p = (model->gamma - 1)  * (u[i].e - u[i].rho * pow(local_u, 2) * 0.5);

		// eq ... ideal gas speed of sound 
		double local_c = sqrt(model->gamma*local_p / u[i].rho);
		sigma[i] = abs(local_u) + local_c;

		// calc v

		V[i] = u[i].area * dx;
	}
	

	for (int i = 1; i < imax-1; i++)
	{
		ny[i] = abs(pres(i-1)-2*pres(i)+pres(i+1)) / abs(pres(i-1)+2*pres(i)+pres(i+1));
	}

	vector<double> term1(imax);


	// calc term1
	for(int i = 0; i < imax-1; i++)
	{
		term1[i] = (V[i+1]*sigma[i+1] + V[i]*sigma[i]);
	}

	auto termDif = dDifRW(term1, dx);
	


	auto ud_f = DifFW(model->ToStateVector(), dx);

	auto ud_frf = DifFW(DifRW(ud_f, dx), dx);

	for (int i = 1; i < imax - 1; i++)
	{
		

		// max function 
		double eps2 = ny[i - 1];
		eps2 = ny[i] > eps2 ? ny[i] : eps2;
		eps2 = ny[i+1] > eps2 ? ny[i+1] : eps2;
		eps2 *= kapp2 * dt;


		// max func

		double eps4 = kapp4*dt-eps2;
		eps4 = eps4 < 0 ? 0 : eps4;

		
		dissip[i] = (ud_f[i] *eps2 - ud_frf[i]*eps4) * termDif[i];

	}


	// not good extrapolation...
	dissip[0] = dissip[1]*2 - dissip[2];
	
	dissip[imax - 1] = dissip[imax - 2] * 2 - dissip[imax - 3]; 
}