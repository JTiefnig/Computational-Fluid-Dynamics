#pragma once
#include "Solver1D.h"
#include "Model1D.h"
#include "State1D.h"
#include <vector>




class Solver1D_MacCormack :
	public Solver1D
{
private: 
	double eps_s;

	double time; // könnte ein Attribut des models sein !!

	void Calc_uq();

	void Boundary_q();

	void Calc_uqq();

	State1D Calc_f(int i);
	State1D Calc_f_q(int i);

	State1D Calc_source(int i);
	State1D Calc_source_q(int i);

	std::vector<State1D> u_q;
	std::vector<State1D> u_qq;

	double dt;

public:
	Solver1D_MacCormack(Model1D* mod);
	~Solver1D_MacCormack();

	// Geerbt über Solver1D
	virtual void DoStep() override;

};

