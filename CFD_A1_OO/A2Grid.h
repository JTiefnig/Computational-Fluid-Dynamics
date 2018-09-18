#pragma once
#include "GridGenerator.h"
#include "Model1D.h"


// Autor Johannes Tiefnig
// CFD TU Graz final project 2018
class A2Grid :
	public GridGenerator
{
private:
	double y;
	double x_min;
	double x_max;


public:
	A2Grid(Model1D* m);
	~A2Grid();

	// Geerbt über GridGenerator
	virtual void Generate() override;
};

