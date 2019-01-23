#include "stdafx.h"
#include "Solver1D_ROE.h"
#include "Model1D.h"
#include "MatrixXd.h"
#include <algorithm>
#include <functional>



using namespace std;

void Solver1D_ROE::calc_f()
{
	double rho, p, v;
	

	int imax = model->u.size();

	vector<GridPoint1D> & u = model->u;

	for (int i = 0; i < imax; i++)
	{
		p = (u[i].e - u[i].rho * pow(u[i].rho_u / u[i].rho, 2)*0.5)*(model->gamma - 1);
		rho = u[i].rho;
		v = u[i].rho_u / u[i].rho;
		

		f[i].rho = u[i].rho_u;
		f[i].rho_u = rho * pow(v, 2) + p;
		f[i].e = (u[i].e + p)*v;

		source[i].rho = -u[i].da_dx / u[i].area * u[i].rho_u;
		source[i].rho_u = -u[i].da_dx / u[i].area * rho*pow(v, 2);
		source[i].e = -u[i].da_dx / u[i].area * (u[i].e + p)*v;

	}
}

void Solver1D_ROE::calc_f_star_roe()
{

	double  H_i;
	double  H_ip1;
	double  H_av;;
	double  R_av;
	double  c_av;
	double  rho_av;
	double  u_av;

	vector<GridPoint1D> &u = model->u;
	double gamma = model->gamma;

	int imax = model->u.size();

	
	MatrixXd L(3, 3);
	MatrixXd R(3, 3);
	MatrixXd lambdaMat(3, 3);

	for (std::size_t i = 0; i < imax - 1; i++)
	{
		
		R_av = sqrt(u[i + 1].rho / u[i].rho); // Eq 13-75
		rho_av = R_av * u[i].rho; // Eq 13-76
		u_av = (R_av * u[i + 1].rho_u / u[i + 1].rho + u[i].rho_u / u[i].rho) / (R_av + 1); // Eq 13-77

		// ...enthalpy
		H_i = pow(u[i].rho_u / u[i].rho, 2) * (1 - gamma) + gamma * u[i].e / u[i].rho;
		H_ip1 = pow(u[i + 1].rho_u / u[i + 1].rho, 2) * (1 - gamma) + gamma * u[i + 1].e / u[i + 1].rho;


		H_av = (R_av * H_ip1 + H_i) / (R_av + 1); // Eq 17-78

		//c... speed of sound
		c_av = sqrt((gamma - 1) * (H_av - 0.5 * pow(u_av, 2))); //Eq 13-80
		
		lambdaMat(0, 0) = u_av;
		lambdaMat(1, 1) = u_av + c_av;
		lambdaMat(2, 2) = u_av - c_av;

		
		// Eq 7-30
		L(0, 0) = 1 - (gamma - 1) * pow(u_av, 2) / (2 * pow(c_av, 2));
		L(0, 1) = (gamma - 1) * (u_av) / pow(c_av, 2);
		L(0, 2) = -(gamma - 1) / pow(c_av, 2);
		L(1, 0) = (0.5*(gamma - 1)*pow(u_av, 2) - u_av * c_av) / (rho_av*c_av);
		L(1, 1) = (c_av - (gamma - 1)*u_av) / (rho_av*c_av);
		L(1, 2) = (gamma - 1) / (rho_av*c_av);
		L(2, 0) = (0.5*(gamma - 1)*pow(u_av, 2) + u_av * c_av) / (rho_av*c_av);
		L(2, 1) = -1 * (c_av + (gamma - 1)*u_av) / (rho_av*c_av);
		L(2, 2) = (gamma - 1) / (rho_av*c_av);


		// Eq 13-90
		R(0, 0) = 1;
		R(0, 1) = rho_av / (2 * c_av);
		R(0, 2)= rho_av / (2 * c_av);
		R(1, 0) = u_av;
		R(1, 1) = (rho_av*(u_av + c_av)) / (2 * c_av);
		R(1, 2) = (rho_av*(u_av - c_av)) / (2 * c_av);
		R(2, 0) = pow(u_av, 2) / 2;
		R(2, 1) = rho_av / (2 * c_av) * (pow(c_av, 2) / (gamma - 1) +
			pow(u_av, 2) / 2 + u_av * c_av);
		R(2, 2) = rho_av / (2 * c_av) * (pow(c_av, 2) / (gamma - 1) +
			pow(u_av, 2) / 2 - u_av * c_av);

		//Entropy fix

		double lambda_i;
		double lambda_ip1;
		double c_i;
		double c_ip1;
		

	

		c_i = sqrt((u[i].e / u[i].rho - pow(u[i].rho_u / u[i].rho, 2.) / 2.) *
			gamma * (gamma - 1.));
		c_ip1 = sqrt((u[i + 1].e / u[i + 1].rho - pow(u[i + 1].rho_u / u[i + 1].rho, 2.) / 2.) *
			gamma * (gamma - 1.));


		// Using a c++ lambda here .. Just for fun
		auto local_func = [&](int k)
			{ 
			double epsilon = fmax(0.,
				fmax(lambdaMat(k, k) - lambda_i,
					lambda_ip1 - lambdaMat(k, k))); //Eq. 13-92
			
			return fabs(lambdaMat(k, k)) < epsilon ? epsilon : lambdaMat(k, k);
		};

		lambda_i = u[i].rho_u / u[i].rho;
		lambda_ip1 = u[i + 1].rho_u / u[i + 1].rho;

		lambdaMat(0, 0) = local_func(0);

		lambda_i = u[i].rho_u / u[i].rho + c_i;
		lambda_ip1 = u[i + 1].rho_u / u[i + 1].rho + c_ip1;

		lambdaMat(1, 1) = local_func(1);

		lambda_i = u[i].rho_u / u[i].rho - c_i;
		lambda_ip1 = u[i + 1].rho_u / u[i + 1].rho - c_ip1;

		lambdaMat(2, 2) = local_func(2);


		MatrixXd lambdaAbs = lambdaMat.Abs();

		MatrixXd A_Abs = R * lambdaAbs* L;

		State1D udiff = u[i + 1] - u[i];


		f_star[i] = (MatrixXd)(f[i] + f[i + 1])*0.5 - A_Abs * ((MatrixXd)(udiff)*0.5);

	}

}


Solver1D_ROE::Solver1D_ROE(Model1D * m)
	:Solver1D(m)
{
	int imax =  model->u.size();

	source = vector<State1D>(imax);
	f = vector<State1D>(imax);
	f_star = vector<State1D>(imax);

	cfl = 0.9;
}

Solver1D_ROE::~Solver1D_ROE()
{
}

void Solver1D_ROE::DoStep()
{
	double dt = TimeStep();
	vector<GridPoint1D> & u = model->u;

	calc_f();
	calc_f_star_roe();

	int imax = u.size();
	
	for (int i = 1; i < imax; i++)	
	{
		model->delta_u[i] =  (f_star[i] - f_star[i - 1])* (-dt) / model->dx + source[i] * dt;
		u[i] = u[i] + model->delta_u[i];
	}


	model->Boundary();
}


