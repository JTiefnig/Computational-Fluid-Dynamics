using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

using CFD_A1_OO;

namespace CFD_A1_WPF
{
    class CfdBackgroundSolver
    {
        List<double> values;

        CfdA1Adapter cfd;

        int selectedSover = 0;
        int stepsToCalcuate = 1;


        public CfdBackgroundSolver()
        {
            values = new List<double>();

            cfd = new CfdA1Adapter();

        }


        public void StartSolver(int solverID, int steps)
        {

            selectedSover = solverID;
            stepsToCalcuate = steps;

            Thread myThread = new Thread(this.DoCalculation);
            myThread.SetApartmentState(ApartmentState.STA);
            myThread.Start();

        }


        public void DoCalculation()
        {
            for (int i = 0; i < 10; i++)
            {
                cfd.DoSteps(stepsToCalcuate,selectedSover);

                ReceiveDatafromSolver();
            }

        }


        public delegate void DataUpdatedevent();
        public event DataUpdatedevent dataRefreshed;

        public void ResetModel()
        {
            cfd.Reset();

        }


        public List<double> GetData()
        {
            lock (values)
            {
                return values;
            }

        }


        void ReceiveDatafromSolver()
        {
            lock (values)
            {
                values.Clear();
                for (int i = 0; i < 100; i++)
                {
                    values.Add(cfd.GetData(i));
                }

            }

            if (dataRefreshed != null)
                dataRefreshed();

        }



    }
}
