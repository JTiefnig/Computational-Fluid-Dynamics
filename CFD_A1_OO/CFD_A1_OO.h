// CFD_A1_OO.h

#pragma once


#include "Model1D.h"
#include "Solver1D.h"
#include "Solver1D_LaxWendroff.h"
#include "Solver1D_Central.h"
#include "Solver1D_MacCormack.h"
#include "GridGenerator.h"


using namespace System;





namespace CFD_A1_OO {

	public ref class CfdA1Adapter
	{
	private:
		Model1D* mod;
		Solver1D_Central* solv_C;
		Solver1D_LaxWendroff* solv_LW;
		Solver1D_MacCormack* solv_MCC;

	public:

		CfdA1Adapter();

		void Reset();

		void DoSteps(int i, int solverID);

		double GetData(int i);


		// TODO: Die Methoden für diese Klasse hier hinzufügen.
	};
}
