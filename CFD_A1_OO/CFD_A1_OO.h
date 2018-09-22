// CFD_A1_OO.h
// Autor Johannes Tiefnig
// CFD TU Graz final project 2018


#pragma once


#include "Model1D.h"
#include "Solver1D.h"
#include "Solver1D_LaxWendroff.h"
#include "Solver1D_Central.h"
#include "Solver1D_MacCormack.h"
#include "Solver1D_ROE.h"
#include "GridGenerator.h"
#include "A1Grid.h"
#include "A2Grid.h"


using namespace System;
using namespace System::Collections::Generic;



namespace CFD_A1_OO {

	public enum class DATASET {
		X,
		AREA,
		RHO,
		RHO_U,
		E,
		PRESSURE, 
		U,
		MACH,
		T
	};


	public enum class MODEL
	{
		A1, 
		A2
	};

	public ref class CfdA1Adapter
	{
	private:
		Model1D * mod;

		Solver1D_Central* solv_C;
		Solver1D_LaxWendroff* solv_LW;
		Solver1D_MacCormack* solv_MCC;
		Solver1D_ROE* solv_ROE;


		A1Grid* grid1;
		A2Grid* grid2;

	public:
		

		MODEL selectedModel;
		

		CfdA1Adapter(MODEL m);
		~CfdA1Adapter();


		void Reset();

		void DoSteps(int i, int solverID);

		int GetGridSize();

		double GetData(int i, DATASET set);

		array<double>^ GetPressureArray();

		array<double>^ GetDataArray(DATASET set);

		List<String^>^ GetParameterList();

		double GetParameter(String^ name);
		void SetParameter(String^ name, double value);

		void SavePropertiesToXml();

		float Convergence();

		int StepCount();


		// Just a test:
		//property double Parameter {double get() { return 4; }}
	};
}
