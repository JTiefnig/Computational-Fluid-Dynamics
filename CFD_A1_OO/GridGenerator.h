#pragma once
#include <vector>

#include "Model1D.h"


using namespace std;



class GridGenerator
{
private: 

	Model1D * model;



	// some Parameters
	double x_min;
	double x_max;
	double y_min;
	double y_max;

public:
	GridGenerator(Model1D* mod);
	~GridGenerator();
	void Generate();
};

