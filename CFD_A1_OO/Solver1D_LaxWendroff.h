#pragma once
#include "Solver1D.h"
#include "Model1D.h"
#include "State1D.h"

#include <vector>



using namespace std;


class Solver1D_LaxWendroff :
	public Solver1D
{
private: 
	double eps_s;


	double time; // könnte ein Attribut des modells sein !!

	void CalcF();

	State1D CalcF_star_LW(int i);

	vector<State1D> source;
	vector<State1D> f;

	double dt;

public:
	Solver1D_LaxWendroff(Model1D* mod);
	~Solver1D_LaxWendroff();

	// Geerbt über Solver1D
	virtual void DoStep() override;
	virtual void DoSteps(int i) override;
};

