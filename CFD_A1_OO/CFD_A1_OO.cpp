// Dies ist die Haupt-DLL.

#include "stdafx.h"

#include "CFD_A1_OO.h"




CFD_A1_OO::CfdA1Adapter::CfdA1Adapter(MODEL m)
{
	selectedModel = m;

	mod = new Model1D(100);

	grid1 = new A1Grid(mod);
	grid2 = new A2Grid(mod);
	
	Reset();

	
	
	solv_C = new Solver1D_Central(mod);
	solv_LW = new Solver1D_LaxWendroff(mod);
	solv_MCC = new Solver1D_MacCormack(mod);
	solv_ROE = new Solver1D_ROE(mod);

	


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
	case 3:
		solv_ROE->DoSteps(i);
		break;
	}
}


double CFD_A1_OO::CfdA1Adapter::GetData(int i, DATASET set)
{
	if (i >= mod->gridsize)
		throw gcnew Exception("Index out of range");

	switch (set)
	{
	case CFD_A1_OO::DATASET::X:
		return mod->u[i].x;
		break;
	case CFD_A1_OO::DATASET::AREA:
		return mod->u[i].area;
		break;
	case CFD_A1_OO::DATASET::RHO:
		return mod->u[i].rho;
		break;
	case CFD_A1_OO::DATASET::RHO_U:
		return mod->u[i].rho_u;
		break;
	case CFD_A1_OO::DATASET::E:
		return mod->u[i].e;
		break;
	case CFD_A1_OO::DATASET::PRESSURE:
		return mod->GetPressure(i);
		break;
	case CFD_A1_OO::DATASET::U:
		return mod->GetVelocity(i);
		break;
	case CFD_A1_OO::DATASET::MACH:
		return mod->GetMach(i);
		break;
	case CFD_A1_OO::DATASET::T:
		return mod->GetTemperatur(i);
		break;
	default:
		break;
	}
}

float CFD_A1_OO::CfdA1Adapter::Convergence()
{
	return mod->CalculateConvergence();
}

int CFD_A1_OO::CfdA1Adapter::StepCount()
{
	return mod->stepcount;
}


void CFD_A1_OO::CfdA1Adapter::Reset()
{
	switch (selectedModel) 
	{
	case MODEL::A1:
		grid1->Generate();
		break;
	case MODEL::A2:
		grid2->Generate();
		break;
	}
}


array<double>^ CFD_A1_OO::CfdA1Adapter::GetPressureArray()
{
	array<double>^ ret = gcnew array<double>(mod->gridsize);

	for (int i = 0; i < mod->gridsize; i++)
	{
		ret[i] = mod->GetPressure(i);
	}

	return ret;
}

array<double>^ CFD_A1_OO::CfdA1Adapter::GetDataArray(DATASET set)
{
	array<double>^ ret = gcnew array<double>(mod->gridsize);

	for (int i = 0; i < mod->gridsize; i++)
	{
		ret[i] = this->GetData(i, set);
	}

	return ret;

}



int CFD_A1_OO::CfdA1Adapter::GetGridSize()
{
	return mod->gridsize;
}


List<String^>^ CFD_A1_OO::CfdA1Adapter::GetParameterList()
{

}

double CFD_A1_OO::CfdA1Adapter::GetParameter(String^ name)
{
	
}

void CFD_A1_OO::CfdA1Adapter::SetParameter(String^ name, double value)
{

}