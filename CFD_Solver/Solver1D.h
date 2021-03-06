#pragma once
#include "Model1D.h"
#include "propertyContainer.h"


// Autor Johannes Tiefnig
// CFD TU Graz final project 2018

class Solver1D
{
protected:
	Model1D* model;

	smartProperty cfl;

	double TimeStep();

public:
	Solver1D(Model1D* model);
	virtual ~Solver1D();

	virtual void DoStep()=0;
	virtual void DoSteps(int i);





};

