#pragma once
#include "Solver1D.h"
#include "Model1D.h"
#include "State1D.h"

#include <vector>




// Autor Johannes Tiefnig
// CFD TU Graz final project 2018
class Solver1D_LaxWendroff :
	public Solver1D
{
private: 
	double eps_s;


	double time; // könnte ein Attribut des modells sein !!

	void CalcF();

	State1D CalcF_star_LW(int i);

	std::vector<State1D> source;
	std::vector<State1D> f;
	std::vector<State1D> dissip;

	double dt;

	void CalcDissipSimple();

public:
	Solver1D_LaxWendroff(Model1D* mod);
	~Solver1D_LaxWendroff();

	// Geerbt über Solver1D
	virtual void DoStep() override;

};

