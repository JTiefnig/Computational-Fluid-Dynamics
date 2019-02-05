#include <iostream>
#include <fstream>
#include <vector>

// Includes needed for solver
#include "Model1D.h"
#include "A2Grid.h"
#include "Solver1D_ROE.h"
#include "Solver1D_Central.h"
#include "Solver1D_MacCormack.h"
#include "Solver1D_LaxWendroff.h"
#include "GridPoint1D.h"




// CFD Homework
// author Johannes Tiefnig
// Simple implementation of "The shock tube problem" solved with the ROE solver
//

int main()
{
	// using streams from the standard library for IO
	std::cout << "CFD Solver Start\n";

	// generate new model with girdsize 100
	auto mod = new Model1D(100);


	// define out Pressure // its diffrent than in the XML
	mod->properties["p_exit"] = 10000;


	// Grid of exercise 2 (Tube)
	auto girdShock = new A2Grid(mod);

	// generate grid according to spesifications
	girdShock->Generate();

	// just generate solver in Stack // type ROE
	Solver1D_ROE solver(mod);
	mod->properties["cfl"] = 0.9;


	// do 100 Steps
	for (int i = 100; i > 0; i--)
	{
		// do one timestep
		solver.DoStep();

		
		//if (mod->time > 0.008)
		//	break;
	}
		

	// open filestream
	std::fstream fileStream;
	fileStream.open("output.csv", std::fstream::out | std::fstream::trunc);

	// first line in file is the legend
	fileStream << "velocity; pressure; mach; temperatur" << std::endl;


	// loop trough all gridpoints and write data to file 
	for (std::size_t i = 0; i < mod->u.size(); i++)
	{
		fileStream << mod->GetVelocity(i) << ";"
			<< mod->GetPressure(i) << ";"
			<< mod->GetMach(i) << ";"
			<< mod->GetTemperatur(i) << ";"
			<< std::endl;
	}

	fileStream.close();


	std::cout << "100 Steps Done; Data exported \n\n";

	std::cout << "Time: " << mod->time << std::endl;


	// Free memory in heap
	delete girdShock;
	delete mod;

	//
	// Hold Console window open
	system("PAUSE");

	// end of program
	return 0;
}


