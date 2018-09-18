#pragma once
#include "GridGenerator.h"
#include "Model1D.h"



// Autor Johannes Tiefnig
// CFD TU Graz final project 2018
class A1Grid :
	public GridGenerator
{
private:


	double x_min;
	double x_max;
	double y_min;
	double y_max;

public:
	A1Grid(Model1D* mod);
	~A1Grid();

	virtual void Generate() override;
};

