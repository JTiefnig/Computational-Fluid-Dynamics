#pragma once





// Autor Johannes Tiefnig
// CFD TU Graz final project 2018
class MatrixXd
{
	double* v;
	

public:
	const int m, n;

	MatrixXd(int i, int j);
	~MatrixXd();

	MatrixXd(const MatrixXd& temp);

	MatrixXd operator * (MatrixXd& b);

	double& operator() (int i, int j);

	double& operator [] (int i) const;

	MatrixXd operator * (double s);

	MatrixXd operator + (MatrixXd& b);
	MatrixXd operator - (MatrixXd& b);


	void operator = (MatrixXd& temp);

	MatrixXd transpose();

	MatrixXd Abs();
};

