#include "stdafx.h"
#include "Solver1D_ROE.h"
#include "Model1D.h"




void Solver1D_ROE::calc_f()
{
	double rho, vel, p, temp;
	

	int imax = model->u.size();

	vector<GridPoint1D> & u = model->u;

	for (int i = 0; i < imax; i++)
	{
		p = (u[i].e - u[i].rho * pow(u[i].rho_u / u[i].rho, 2)*0.5)*(model->gamma - 1);
		rho = u[i].rho;
		vel = u[i].rho_u / u[i].rho;
		temp = p / (rho*model->R);

		f[i].rho = u[i].rho_u;
		f[i].rho_u = rho * pow(vel, 2) + p;
		f[i].e = (u[i].e + p)*vel;

		source[i].rho = -u[i].da_dx / u[i].area * u[i].rho_u;
		source[i].rho_u = -u[i].da_dx / u[i].area * rho*pow(vel, 2);
		source[i].e = -u[i].da_dx / u[i].area * (u[i].e + p)*vel;

	}
}

void Solver1D_ROE::calc_f_star_roe()
{
	

	int     i, ii, jj;

	double  R_av;
	double  rho_av;
	double  u_av;
	double  c_av;
	double  H_i;
	double  H_ip1;
	double  H_av;
	double  lambda_av1;
	double  lambda_av2;
	double  lambda_av3;
	double  epsilon;
	double  R[N][N] = { 0 };
	double  L[N][N] = { 0 };
	double  lambda_mat[N][N] = { 0 };
	double  lambda_abs[N][N] = { 0 };
	double  lambda_abs_x_L[N][N] = { 0 };
	double  A_abs[N][N] = { 0 };
	double  u_diff[N] = { 0 };
	double  u_trans[N] = { 0 };

	vector<GridPoint1D> &u = model->u;
	double gamma = model->gamma;

	int imax = model->u.size();

	for (i = 0; i < imax - 1; i++)
	{
		// (A1) calculation of averages
		R_av = sqrt(u[i + 1].rho / u[i].rho);
		rho_av = R_av * u[i].rho;
		u_av = (R_av * u[i + 1].rho_u / u[i + 1].rho + u[i].rho_u / u[i].rho) / (R_av + 1);

		//Enthalpy
		H_i = pow(u[i].rho_u / u[i].rho, 2) * (1 - gamma) + gamma * u[i].e / u[i].rho;
		H_ip1 = pow(u[i + 1].rho_u / u[i + 1].rho, 2) * (1 - gamma) + gamma * u[i + 1].e / u[i + 1].rho;


		H_av = (R_av * H_ip1 + H_i) / (R_av + 1);

		//speed of sound
		c_av = sqrt((gamma - 1) * (H_av - 0.5 * pow(u_av, 2)));

		// (A2) Eigenvalues and Eigenvectors
		lambda_av1 = u_av;
		lambda_av2 = u_av + c_av;
		lambda_av3 = u_av - c_av;

		// (A3) Matrices of the left and right eigenvectors
		lambda_mat[1][1] = lambda_av1;
		lambda_mat[2][2] = lambda_av2;
		lambda_mat[3][3] = lambda_av3;

		L[1][1] = 1 - (gamma - 1) * pow(u_av, 2) / (2 * pow(c_av, 2));
		L[1][2] = (gamma - 1) * (u_av) / pow(c_av, 2);
		L[1][3] = -(gamma - 1) / pow(c_av, 2);
		L[2][1] = (0.5*(gamma - 1)*pow(u_av, 2) - u_av * c_av) / (rho_av*c_av);
		L[2][2] = (c_av - (gamma - 1)*u_av) / (rho_av*c_av);
		L[2][3] = (gamma - 1) / (rho_av*c_av);
		L[3][1] = (0.5*(gamma - 1)*pow(u_av, 2) + u_av * c_av) / (rho_av*c_av);
		L[3][2] = -1 * (c_av + (gamma - 1)*u_av) / (rho_av*c_av);
		L[3][3] = (gamma - 1) / (rho_av*c_av);

		R[1][1] = 1;
		R[1][2] = rho_av / (2 * c_av);
		R[1][3] = rho_av / (2 * c_av);
		R[2][1] = u_av;
		R[2][2] = (rho_av*(u_av + c_av)) / (2 * c_av);
		R[2][3] = (rho_av*(u_av - c_av)) / (2 * c_av);
		R[3][1] = pow(u_av, 2) / 2;
		R[3][2] = rho_av / (2 * c_av) * (pow(c_av, 2) / (gamma - 1) +
			pow(u_av, 2) / 2 + u_av * c_av);
		R[3][3] = rho_av / (2 * c_av) * (pow(c_av, 2) / (gamma - 1) +
			pow(u_av, 2) / 2 - u_av * c_av);

		//Entropy fix

		double c_i;
		double c_ip1;
		double lambda_i;
		double lambda_ip1;

		int k;

		c_i = sqrt((u[i].e / u[i].rho - pow(u[i].rho_u / u[i].rho, 2.) / 2.) *
			gamma * (gamma - 1.));
		c_ip1 = sqrt((u[i + 1].e / u[i + 1].rho - pow(u[i + 1].rho_u / u[i + 1].rho, 2.) / 2.) *
			gamma * (gamma - 1.));

		for (k = 1; k <= 3; k++) //cycle through all three eigenvalues
		{
			switch (k)
			{
			case 1:
				lambda_i = u[i].rho_u / u[i].rho;
				lambda_ip1 = u[i + 1].rho_u / u[i + 1].rho;
				break;
			case 2:
			{
				lambda_i = u[i].rho_u / u[i].rho + c_i;
				lambda_ip1 = u[i + 1].rho_u / u[i + 1].rho + c_ip1;
				break;
			}
			case 3:
			{
				lambda_i = u[i].rho_u / u[i].rho - c_i;
				lambda_ip1 = u[i + 1].rho_u / u[i + 1].rho - c_ip1;
				break;
			}
			}
			//----------------------------------------------------------------------
			//
			epsilon = fmax(0.,
				fmax(lambda_mat[k][k] - lambda_i,
					lambda_ip1 - lambda_mat[k][k])); //Eq. 13-92
			if (fabs(lambda_mat[k][k]) < epsilon) //Eq. 13-91
			{
				lambda_mat[k][k] = epsilon;
			}
		}
		int k2;
		for (k2 = 1; k2 <= 3; k2++) //Use absolute value of Eigenvalues
		{
			lambda_abs[k2][k2] = fabs(lambda_mat[k2][k2]);
		}

		// Build Matrix A_abs = R.lambda_abs.L
		matrix_mult(lambda_abs, L, lambda_abs_x_L);
		matrix_mult(R, lambda_abs_x_L, A_abs);

		for (ii = 1; ii <= 3; ii++)
		{
			u_diff[ii] = u[i + 1][ii] - u[i][ii];
		}
		matrix_vector_mult(A_abs, u_diff, u_trans);

		for (jj = 1; jj <= 3; jj++)
		{
			f_star[i][jj] = 0.5 * (f[i][jj] + f[i + 1][jj]) -
				0.5 * u_trans[jj];
		}
	}

}



void Solver1D_ROE::matrix_mult(double mat1[][N], double mat2[][N], double res[][N])
{
	int i, j, k;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			res[i][j] = 0;
			for (k = 0; k < N; k++)
				res[i][j] += mat1[i][k] * mat2[k][j];
		}
	}
}

void Solver1D_ROE::matrix_vector_mult(double mat1[][N], double vector[N], double res[N])
{
	int i, k;
	for (i = 0; i < N; i++)
	{
		res[i] = 0;
		for (k = 0; k < N; k++)
			res[i] += mat1[i][k] * vector[k];
	}
}




Solver1D_ROE::Solver1D_ROE(Model1D * m)
	:Solver1D(m)
{
	int imax =  model->u.size();

	source = vector<State1D>(imax);
	f = vector<State1D>(imax);
	f_star = vector<State1D>(imax);
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
	
	for (int i = 1; i < imax - 1; i++)
	{

		model->delta_u[i] =  (f_star[i] - f_star[i - 1])* (-dt) / model->dx + source[i] * dt;
		u[i] = u[i] + model->delta_u[i];
	}

	

	model->Boundary();
}


void Solver1D_ROE::DoSteps(int i)
{
	for (; i >= 0; i--)
	{
		this->DoStep();
	}
}
