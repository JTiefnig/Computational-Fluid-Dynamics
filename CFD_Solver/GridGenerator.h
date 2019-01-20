#pragma once
#include <vector>

#include "Model1D.h"




//
// Autor Johannes Tienfig
// CFD Tu Graz Final Project
//
// This Class enables the generic generation of an inital model condition
// For now there are just two different Startconditions implemented
// 
//

class GridGenerator
{
protected:
	Model1D * model;

private: 
	// some Parameters // SmartPropteries later

public:
	GridGenerator(Model1D* mod);
	~GridGenerator();
	virtual void Generate()=0;
};

