#pragma once
#include "Model1D.h"

class Solver1D
{
protected:
	Model1D* model;

	double cfl;
	double TimeStep();

public:
	Solver1D(Model1D* model);
	virtual ~Solver1D();

	virtual void DoStep()=0;
	virtual void DoSteps(int i);



};

