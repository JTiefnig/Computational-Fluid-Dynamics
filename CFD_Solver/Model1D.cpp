#include "stdafx.h"
#include "Model1D.h"

#include "GridGenerator.h"

using namespace std;

Model1D::Model1D(int gridSize)
{
	this->gridsize = gridSize;

	u = vector<GridPoint1D>(gridSize);
	delta_u = vector<State1D>(gridSize);

	this->properties.LoadFromFile("PARAMETER.xml");
	// jetzt noch nicht da
	//properties.loadPropertiesFromFile("PARAMERTER.xml");
	
	R = properties["R"];
	//properties.createProperty("R", R);
	gamma = properties["gamma"]; // 
	//properties.createProperty("gamma", gamma);
	p_tot = properties["p_tot"];
	//properties.createProperty("p_tot", p_tot);
	T_tot = properties["T_tot"];
	//properties.createProperty("T_tot", T_tot);
	p_exit = properties["p_exit"];
	//properties.createProperty("p_exit", p_exit);
	sub_exit = properties["sub_exit"];
	//properties.createProperty("sub_exit", sub_exit);


	//nPrint = properties["nPrint"];
	// Rho tot wird errechnet...
	rho_tot = p_tot / (R*T_tot);

	time = 0;

}


Model1D::~Model1D()
{
	// wird derzeit nicht erreicht ?
}




void Model1D::Boundary()
{
	

	int imax = u.size();

	double rho = 2*u[1].rho- u[2].rho;

	if (rho > rho_tot)
	{
		rho = rho_tot;
	}
	
	u[0].rho = rho;

	//double cp = (R* gamma / (gamma + 1));

	double p = p_tot * pow(rho / rho_tot, gamma); //p_tot* pow(((gamma - 1) / gamma*cp*T_tot*rho / (p_tot)), gamma);

	double vel = sqrt(2 * R*gamma / (gamma - 1)*(T_tot - p / (rho*R)));//u[1].rho_u / u[1].rho;
	
	u[0].rho_u = vel * rho;
	u[0].e = p / (gamma - 1) + rho * pow(vel, 2)*0.5;


	p = p_exit; //p_tot* pow(((gamma - 1) / gamma*cp*T_tot*rho / (p_tot)), gamma);

	rho = 2*u[imax - 2].rho -  u[imax - 3].rho;

	u[imax - 1].rho = rho;
	u[imax - 1].rho_u = 2 * u[imax - 2].rho_u - u[imax - 3].rho_u;// vel * rho;

	vel = u[imax - 1].rho_u / u[imax - 1].rho;
	u[imax - 1].e = p / (gamma - 1) + rho * pow(vel, 2)*0.5;


	if (stepcount == nPrint)
		CalculateConvergence();

	// Steps are counted here
	stepcount++;
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
	return p; 
}

double Model1D::GetMach(int i)
{
	double rho = u[i].rho;
	double vel = u[i].rho_u / rho;
	double p = (gamma - 1)*(u[i].e - rho * vel*vel / 2);
	double mach = vel / pow((gamma*p / rho), 0.5);
	return mach;
}
double Model1D::GetTemperatur(int i)
{
	double rho = u[i].rho;
	double vel = u[i].rho_u / rho;
	double p = (gamma - 1)*(u[i].e - rho * vel*vel / 2);
	double mach = vel / pow((gamma*p / rho), 0.5);
	double temp = 1 + (gamma - 1) / 2 * mach * mach;
	return temp;
}
double Model1D::GetVelocity(int i)
{
	double rho = u[i].rho;
	double vel = u[i].rho_u / rho;
	return vel;
}




double Model1D::CalculateConvergence()
{
	
	State1D resid;
	
	
	// Extrapolierte Randbereiche nicht in die Berechnung nehmen.
	for(std::size_t i =1; i<delta_u.size()-1; i++)
	{  
		resid = resid + delta_u[i].absComponents();
	}

	if (this->stepcount == nPrint)
		convCompValue = resid.e;


	return resid.e/convCompValue;
}
