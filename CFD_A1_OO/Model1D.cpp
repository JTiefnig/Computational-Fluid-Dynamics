#include "stdafx.h"
#include "Model1D.h"


Model1D::Model1D(int gridSize)
{
	this->gridsize = gridSize;
	u = vector<GridPoint1D>(gridSize);


	R = 287.0;
	gamma = 1.4; // entspricht kappa

	p_tot = 1;
	T_tot = 273.15;
	p_exit = 0.9;
	sub_exit = 1;
	rho_tot = 0;
}


Model1D::~Model1D()
{
}

void Model1D::Initialize()
{
	rho_tot = p_tot / (R*T_tot);

	double rho_start = rho_tot;
	double rhou_start = 0;
	double e_start = p_tot / (gamma - 1);

	// Initialisieren des Stroemungsfeldes (Zustandsvektor U) mit den Ruhezustandswerten
	// Initialisation of the flow field (state vector U) with the stagnation values (=total values)

	int imax = u.size();
	for (int i = 0; i < imax; i++)
	{
		u[i].rho = rho_start;
		u[i].rho_u = rhou_start;
		u[i].e = e_start;
	}
	return;
}


void Model1D::Boundary()
{

	int imax = u.size();

	double rho = u[1].rho;

	double cp = (R* gamma / (gamma + 1));

	double p = p_tot * pow(rho / rho_tot, gamma); //p_tot* pow(((gamma - 1) / gamma*cp*T_tot*rho / (p_tot)), gamma);

	double vel = u[1].rho_u / u[1].rho;


	if (rho > rho_tot)
	{
		rho = rho_tot;
	}


	u[0].rho = rho;
	u[0].rho_u = vel * rho;
	u[0].e = p / (gamma - 1) + rho * pow(vel, 2)*0.5;


	rho = u[imax - 2].rho;


	p = p_exit; //p_tot* pow(((gamma - 1) / gamma*cp*T_tot*rho / (p_tot)), gamma);

	vel = u[imax - 2].rho_u / u[imax - 2].rho;


	if (rho > rho_tot)	rho = rho_tot;

	u[imax - 1].rho = rho;
	u[imax - 1].rho_u = vel * rho;
	u[imax - 1].e = p / (gamma - 1) + rho * pow(vel, 2)*0.5;

}

GridPoint1D & Model1D::operator[](int i)
{
	return u[i];
}

double Model1D::GetPressure(int i)
{
	double rho = u[i].rho;
	double vel = u[i].rho_u / rho;
	double p = (gamma - 1)*(u[i].e - rho * vel*vel / 2);
	double mach = vel / pow((gamma*p / rho), 0.5);
	double temp = 1 + (gamma - 1) / 2 * mach * mach;



	return p; // Whatever
	
}
