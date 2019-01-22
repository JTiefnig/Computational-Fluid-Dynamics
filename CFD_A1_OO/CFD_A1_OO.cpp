// Dies ist die Haupt-DLL.

#include "stdafx.h"

#include "CFD_A1_OO.h"
#include <string>


//#include <msclr\marshal_cppstd.h>
//#include <msclr\marshal.h>

using namespace System;
using namespace System::Runtime::InteropServices;

CFD_A1_OO::CfdA1Adapter::CfdA1Adapter(int m)
{
	

	try
	{
		mod = new Model1D(100);
	}
	catch (const char* c)
	{
		throw gcnew Exception(gcnew String(c));
	}
	

	grid1 = new A1Grid(mod);
	grid2 = new A2Grid(mod);
	
	Reset(m);

	
	
	solv_C = new Solver1D_Central(mod);
	solv_LW = new Solver1D_LaxWendroff(mod);
	solv_MCC = new Solver1D_MacCormack(mod);
	solv_ROE = new Solver1D_ROE(mod);
}

CFD_A1_OO::CfdA1Adapter::~CfdA1Adapter()
{
	delete mod;
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

	double retval = 0;
	switch (set)
	{
	case CFD_A1_OO::DATASET::X:
		retval = mod->u[i].x;
		break;
	case CFD_A1_OO::DATASET::AREA:
		retval = mod->u[i].area;
		break;
	case CFD_A1_OO::DATASET::RHO:
		retval = mod->u[i].rho;
		break;
	case CFD_A1_OO::DATASET::RHO_U:
		retval = mod->u[i].rho_u;
		break;
	case CFD_A1_OO::DATASET::E:
		retval = mod->u[i].e;
		break;
	case CFD_A1_OO::DATASET::PRESSURE:
		retval = mod->GetPressure(i);
		break;
	case CFD_A1_OO::DATASET::U:
		retval = mod->GetVelocity(i);
		break;
	case CFD_A1_OO::DATASET::MACH:
		retval = mod->GetMach(i);
		break;
	case CFD_A1_OO::DATASET::T:
		retval = mod->GetTemperatur(i);
		break;
	default:
		break;
	}



	if (isnan(retval) || isinf(retval))
		throw gcnew Exception(gcnew String("Not Stable"));


	return retval;
}


void CFD_A1_OO::CfdA1Adapter::SavePropertiesToXml()
{
	mod->properties.SaveToFile("PARAMETER.xml");
}

float CFD_A1_OO::CfdA1Adapter::Convergence()
{
	return mod->CalculateConvergence();
}

int CFD_A1_OO::CfdA1Adapter::StepCount()
{
	return mod->stepcount;
}


void CFD_A1_OO::CfdA1Adapter::Reset(int m)
{
	switch (m) 
	{
	case 0:
		grid1->Generate();
		break;
	case 1:
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
	List<String^>^ ret = gcnew List<String^>();

	for (int i = 0; i < mod->properties.size(); i++)
	{
		ret->Add(gcnew String(mod->properties[i].getName().c_str()));
	}

	return ret;

}

double CFD_A1_OO::CfdA1Adapter::GetParameter(String^ name)
{

	double ret =0;
	IntPtr pString = Marshal::StringToHGlobalAnsi(name);
	try
	{
		char* pchString = static_cast<char *>(pString.ToPointer());
		ret= mod->properties[pchString].getValue();
	}
	finally
	{
		Marshal::FreeHGlobal(pString);
	}
	return ret;
	
}

void CFD_A1_OO::CfdA1Adapter::SetParameter(String^ name, double value)
{

	IntPtr pString = Marshal::StringToHGlobalAnsi(name);
	try
	{
		char* pchString = static_cast<char *>(pString.ToPointer());
		mod->properties[pchString] = value;
	}
	finally
	{
		Marshal::FreeHGlobal(pString);
	}
}