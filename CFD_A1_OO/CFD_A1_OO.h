// CFD_A1_OO.h

#pragma once


#include "Model1D.h"
#include "Solver1D.h"
#include "Solver1D_LaxWendroff.h"
#include "Solver1D_Central.h"
#include "Solver1D_MacCormack.h"
#include "Solver1D_ROE.h"
#include "GridGenerator.h"


using namespace System;
using namespace System::Collections::Generic;

///
/// CFD Final Project
/// Autor Johannes Tiefnig
///

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

	public ref class CfdA1Adapter
	{
	private:
		Model1D * mod;

		Solver1D_Central* solv_C;
		Solver1D_LaxWendroff* solv_LW;
		Solver1D_MacCormack* solv_MCC;
		Solver1D_ROE* solv_ROE;


		
	public:

		

		CfdA1Adapter();

		void Reset();

		void DoSteps(int i, int solverID);


		int GetGridSize();

		double GetData(int i, DATASET set);

		array<double>^ GetPressureArray();

		array<double>^ GetDataArray(DATASET set);

		float Convergence();

		

		property double Parameter {double get() { return 4; }}

		// TODO: Die Methoden für diese Klasse hier hinzufügen.
	};
}
