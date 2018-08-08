// Dies ist die Haupt-DLL.

#include "stdafx.h"

#include "CFD_A1_OO.h"



CFD_A1_OO::CfdA1Adapter::CfdA1Adapter()
{
	mod = new Model1D(100);
	GridGenerator* gridGen = new GridGenerator(mod);
	gridGen->Generate();
	gridGen->~GridGenerator();
	mod->Initialize();
	solv_C = new Solver1D_Central(mod);
	solv_LW = new Solver1D_LaxWendroff(mod);
	solv_MCC = new Solver1D_MacCormack(mod);

}

void CFD_A1_OO::CfdA1Adapter::DoSteps(int i, int solverID)
{
	switch (solverID)
	{
	case 0:
		solv_C->DoSteps(i);
		break;
	case 1:
		solv_LW->DoSteps(i);
		break;
	case 2:
		solv_MCC->DoSteps(i);
		break;
	}
}

double CFD_A1_OO::CfdA1Adapter::GetData(int i)
{
	return mod->GetPressure(i);
}

float CFD_A1_OO::CfdA1Adapter::Convergence()
{
	return mod->CalculateConvergence();
}


void CFD_A1_OO::CfdA1Adapter::Reset()
{
	mod->Initialize();
}