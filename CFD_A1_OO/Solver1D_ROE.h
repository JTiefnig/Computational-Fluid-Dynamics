#pragma once
#include "Solver1D.h"
#include "Model1D.h"


class Solver1D_ROE :
	public Solver1D
{
private:
	void calc_f();
	void calc_f_star_roe();

public:
	Solver1D_ROE(Model1D* m);
	~Solver1D_ROE();

	// Geerbt über Solver1D
	virtual void DoStep() override;
	virtual void DoSteps(int i) override;



};

