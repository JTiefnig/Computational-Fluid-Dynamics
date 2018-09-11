#include "stdafx.h"
#include "MatrixXd.h"
#include <cmath>



MatrixXd::MatrixXd(int i, int j)
	:m(i), n(j)
{
	v = new double[m*n]{0.0};
}


MatrixXd::~MatrixXd()
{
	delete[] v;
}

MatrixXd::MatrixXd(const MatrixXd & temp)
	:m(temp.m), n(temp.n)
{
	v = new double[m*n]{ 0.0 };

	for (int c = 0; c < m*n; c++)
	{
		v[c] = temp[c];
	}
}



double& MatrixXd::operator() (int i, int j)
{
	return v[i*n+j];
}
double& MatrixXd::operator[] (int i) const
{
	return v[i];
}

MatrixXd MatrixXd::operator*(double s)
{
	MatrixXd ret(this->m, this->n);


	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j<n; j++)
		{	
			ret(i, j) = v[i*n + j] * s;
		}
	}

	return ret;
}

MatrixXd MatrixXd::operator+(MatrixXd & b)
{
	if (n != b.n|| m != b.m)
		throw "Dim ERROR";

	MatrixXd ret(this->m, this->n);


	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j<n; j++)
		{
			ret(i, j) = v[i*n + j] + b(i, j);
		}
	}

	return ret;
}

MatrixXd MatrixXd::operator-(MatrixXd & b)
{
	if (n != b.n || m != b.m)
		throw "Dim ERROR";

	MatrixXd ret(this->m, this->n);


	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j<n; j++)
		{
			ret(i, j) = v[i*n + j] - b(i, j);
		}
	}

	return ret;
}

void MatrixXd::operator=(MatrixXd & temp)
{

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j<n; j++)
		{
			 v[i*n + j] = temp(i, j);
		}
	}
}

MatrixXd MatrixXd::transpose()
{
	MatrixXd ret(this->n, this->m);

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j<n; j++)
		{
			ret(j, i) = v[i*n + j];
		}
	}

	return ret;
}

MatrixXd MatrixXd::Abs()
{
	MatrixXd ret(*this);

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j<n; j++)
		{
			ret(i, j) = abs(ret(i, j));
		}
	}

	return ret;
}

MatrixXd MatrixXd::operator * (MatrixXd& b)
{
	if (this->n != b.m)
		throw "Dim ERROR";

	int nm = this->m;
	int nn = b.n;
	MatrixXd ret(nm, nn);

	for (int i = 0; i < nm; i++)
	{
		for (int j = 0; j<nn; j++)
		{
			for (int c = 0; c < b.m; c++)
			{
				ret(i, j) += v[i*n + c]*b(c,j);
			}
		}
	}

	return ret;

}