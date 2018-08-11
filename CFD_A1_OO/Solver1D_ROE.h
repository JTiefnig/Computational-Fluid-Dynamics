#pragma once
#include "Solver1D.h"
#include "Model1D.h"


// very bad but simple
#define N 3

class Solver1D_ROE :
	public Solver1D
{
private:
	void calc_f();
	void calc_f_star_roe();

	vector<State1D> source;
	vector<State1D>  f;
	vector<State1D> f_star;

	static void matrix_mult(double mat1[][N], double mat2[][N], double res[][N]);
	static void matrix_vector_mult(double mat1[][N], double vector[N], double res[N]);


public:
	Solver1D_ROE(Model1D* m);
	~Solver1D_ROE();

	// Geerbt über Solver1D
	virtual void DoStep() override;
	virtual void DoSteps(int i) override;



};

