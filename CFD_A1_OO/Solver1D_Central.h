#pragma once
#include "Solver1D.h"
#include "Model1D.h"
#include "State1D.h"
#include <vector>

using namespace std;


class Solver1D_Central :
	public Solver1D
{
public:
	Solver1D_Central(Model1D* mod);
	virtual ~Solver1D_Central();


	// Geerbt �ber Solver1D
	virtual void DoStep() override;

	virtual void DoSteps(int i) override;

private:

	double eps_s;


	double time;


	void CalcF();

	
	State1D CalcF_star_central(int i);

	void CalcDissipSimple();


	
	vector<State1D> source;
	vector<State1D> f;
	vector<State1D> dissip;

};
