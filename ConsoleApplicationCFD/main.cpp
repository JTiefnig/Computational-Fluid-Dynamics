                                                                                                                                 #include <iostream>
#include <fstream>
#include <vector>

// Includes needed for solver
#include "Model1D.h"
#include "A1Grid.h"
#include "Solver1D_ROE.h"
#include "GridPoint1D.h"




// CFD Homework
// author Johannes Tiefnig
// Have fun with my program
//

int main()
{
	// using streams from the standard library for IO
    std::cout << "CFD Solver Start\n"; 

	// generate new model with girdsize 100
	auto mod = new Model1D(100);
	// Grid of exercise 1 (Laval)
	auto girdLaval = new A1Grid(mod);
	
	// generate grid according to spesifications
	girdLaval->Generate();

	// just generate solver in Stack // type rohesovler
	Solver1D_ROE solver(mod);


	// define limit when system is converged
	double convLimit = 1e-2;

	while (true)
	{

		// do one timestep
		solver.DoStep();

		// output every 10th iteration
		if(!(mod->stepcount%10))
		{
			double conv = mod->CalculateConvergence();

			std::cout << "iter: " << mod->stepcount << " - conv: " << conv << std::endl;
			
			// end loop if convergence is reached
			if (conv < convLimit)
				break;
		}
	}


	// open filestream
	std::fstream fileStream;
	fileStream.open("output.csv", std::fstream::out | std::fstream::trunc);

	// first line in file is the legend
	fileStream << "velocity; pressure; mach; temperatur" << std::endl;

	
	

	// loop trough all gridpoints and write data to file 
	for(std::size_t i =0; i< mod->u.size(); i++)
	{
		fileStream << mod->GetVelocity(i) << ";"
			<< mod->GetPressure(i) << ";"
			<< mod->GetMach(i) << ";"
			<< mod->GetTemperatur(i) << ";"
			<< std::endl;
	}

	fileStream.close();


	std::cout << "System converged; Data exported \n\n";

	delete girdLaval;
	delete mod;

	//
	// Hold Console window open
	system("PAUSE");

	// end of program
	return 0;
}


