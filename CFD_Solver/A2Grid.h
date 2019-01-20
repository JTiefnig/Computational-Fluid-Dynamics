#pragma once
#include "GridGenerator.h"
#include "Model1D.h"


// Autor Johannes Tiefnig
// CFD TU Graz final project 2018
class A2Grid :
	public GridGenerator
{
private:
	

	smartProperty y_gridA2;
	smartProperty x_min_gridA2;
	smartProperty x_max_gridA2;


public:
	A2Grid(Model1D* mod);
	~A2Grid();

	// Geerbt über GridGenerator
	virtual void Generate() override;
};

