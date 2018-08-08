#include "stdafx.h"
#include "Solver1D_ROE.h"
#include "Model1D.h"




Solver1D_ROE::Solver1D_ROE(Model1D * m)
	:Solver1D(m)
{
}

Solver1D_ROE::~Solver1D_ROE()
{
}

void Solver1D_ROE::DoStep()
{
	double dt = TimeStep();

	//calc_f();
	//calc_f_star_roe();

	//for (i = 2; i <= imax - 1; i++)
	//{
	//	for (k = 1; k <= 3; k++)
	//	{
	//		delta_u[i][k] = -dt * (f_star[i][k] - f_star[i - 1][k]) / model->dx + dt * source[i][k];
	//		u[i][k] = u[i][k] + delta_u[i][k];
	//	}

	//}

	//model->Boundary(); //Extrapolation

	//if ((itr%nprint == 0) || (itr == max_iter))		end = conv(itr); //convergence test

	//if (end == 1)
	//{
	//	printf("\n Convergence limit of %lf achieved! \n", convergence);
	//	printf("\n Number of iterations: %d \n", itr);
	//	break;
	//}
}


void Solver1D_ROE::DoSteps(int i)
{
	for (; i <= 0; i--)
	{
		this->DoStep();
	}
}
