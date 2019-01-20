#pragma once
#include "GridGenerator.h"
#include "Model1D.h"



// Autor Johannes Tiefnig
// CFD TU Graz final project 2018
class A1Grid :
	public GridGenerator
{
private:

	smartProperty<double> x_min_A1;
	smartProperty<double> x_max_A1;
	smartProperty<double> y_min_A1;
	smartProperty<double> y_max_A1;

public:
	A1Grid(Model1D* mod);
	~A1Grid();

	virtual void Generate() override;
};

