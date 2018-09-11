#pragma once
#include "Solver1D.h"
#include "Model1D.h"
#include "State1D.h"
#include <vector>




class Solver1D_Central :
	public Solver1D
{
public:
	Solver1D_Central(Model1D* mod);
	virtual ~Solver1D_Central();


	// Geerbt über Solver1D
	virtual void DoStep() override;



private:

	double eps_s;


	double time;


	void CalcF();

	
	State1D CalcF_star_central(int i);

	void CalcDissipSimple();


	
	std::vector<State1D> source;
	std::vector<State1D> f;
	std::vector<State1D> dissip;

};

