#pragma once
#include "Solver1D.h"
#include "Model1D.h"


// very bad. but simple / will be improved
#define N 4

class Solver1D_ROE :
	public Solver1D
{
private:
	void calc_f();
	void calc_f_star_roe();

	std::vector<State1D> source;
	std::vector<State1D>  f;
	std::vector<State1D> f_star;


public:
	Solver1D_ROE(Model1D* m);
	~Solver1D_ROE();

	// Geerbt über Solver1D
	virtual void DoStep() override;




};

