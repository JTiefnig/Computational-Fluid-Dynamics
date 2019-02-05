#pragma once
#include "Solver1D.h"
#include "Model1D.h"
#include "State1D.h"
#include <vector>



// Autor Johannes Tiefnig
// CFD TU Graz final project 2018

// source 1 : https://www.degruyter.com/downloadpdf/j/heem.2014.60.issue-1-4/heem-2013-0008/heem-2013-0008.pdf
// soucce 2: https://projects.skill-lync.com/projects/Simulation-of-a-1D-Quasi-Super-sonic-nozzle-flow-simulation-using-Maccormack-Method-76683

class Solver1D_MacCormack :
	public Solver1D
{
private: 
	double eps_s;


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

