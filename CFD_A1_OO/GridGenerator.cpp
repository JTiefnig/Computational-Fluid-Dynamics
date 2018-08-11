#include "stdafx.h"
#include "GridGenerator.h"
#include "GridPoint1D.h"

#include <vector>

using namespace std;

GridGenerator::GridGenerator(Model1D* mod)
{

	this->model = mod;
	
	// Some parameters
	x_min = -0.33333;
	x_max = 1;
	y_min = 0.1;
	y_max = 0.2;
}


GridGenerator::~GridGenerator()
{
}

void GridGenerator::Generate()
{

	int imax = model->u.size();

	double dx = (x_max - x_min) / imax;
	model->dx = dx;
	double 	local_k = (y_max - y_min) / (pow(x_max, 2));

	for (int i = 0; i < imax; i++)
	{
		(*model)[i].x = (x_min + (i)* dx);

		(*model)[i].area = (y_min + (y_max - y_min)*pow(model->u[i].x / x_max, 2));//(y_min + local_k * pow((*model)[i].x, 2));

		(*model)[i].da_dx = (local_k * (*model)[i].x * 2);
	}


}
