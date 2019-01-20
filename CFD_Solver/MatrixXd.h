#pragma once





// Autor Johannes Tiefnig
// CFD TU Graz final project 2018
// ....could be a template class later... but than i rather would use Eigen lib
class MatrixXd
{

	// dont know if i should put mutable here? 
	double* v;
	

public:
	const int m, n;

	MatrixXd(int i, int j);
	
	~MatrixXd();

	MatrixXd(const MatrixXd& temp);

	MatrixXd operator * (const MatrixXd& b);

	double& operator() (int i, int j) const;

	double& operator [] (int i) const;

	MatrixXd operator * (double s);

	MatrixXd operator + (const MatrixXd& b);
	MatrixXd operator - (const MatrixXd& b);


	void operator = (const MatrixXd& temp);

	MatrixXd transpose();

	MatrixXd Abs();
};

