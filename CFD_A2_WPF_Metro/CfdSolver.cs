using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CFD_A1_OO;
using System.ComponentModel;
using System.Threading;
using System.Collections.ObjectModel;


using LiveCharts;
using LiveCharts.Wpf;
using LiveCharts.Defaults;


namespace CFD_A2_WPF_Metro
{

    public class CfdSolver : INotifyPropertyChanged
    {
        bool pressure = true;

        public bool ShowPressure { set { pressure = value; OnPropertyChanged(nameof(ShowPressure)); } get { return pressure; } }
        public bool ShowMach { set; get; } = false;
        public bool ShowVelocity { set; get; } = false;




        public CfdSolver()
        {
            simData = new SeriesCollection();

            cfd = new CfdA1Adapter();

            var ax = new AxesCollection();
       
            // Just for testning now
            AddSeriesCollection("Pressure");

            worker = new Thread(() => this.SimulateSteps());

        }

        private SeriesCollection simData;

        private CfdA1Adapter cfd;

        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged(string name)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }

        public SeriesCollection SimulationData
        {
            get { lock (simData) { return simData; } }
            set
            {
                lock (simData)
                {
                    simData = value;
                }
                OnPropertyChanged("SimulationData");
            }
        }

        private int steps = 500;
        public int Steps
        {
            get { return steps; }
            set
            {
                steps = value;
                OnPropertyChanged("Steps");
            }
        }

        private int updateInterval = 20;
        public int UpdatdateInterval
        {
            get { return updateInterval; }
            set
            {
                updateInterval = value;
                OnPropertyChanged("UpdatdateInterval");
            }
        }


        public double[] PressureSeries
        {
            get {return cfd.GetPressureArray(); }
        }


        public double[] AreaSeries
        {
            get {  return cfd.GetDataArray(DATASET.AREA);  }
        }


        public void AddSeriesCollection(string name)
        {

            var ls = new LineSeries
            {
                Title = name,
                Values = new ChartValues<ObservablePoint>(),
                PointGeometry = DefaultGeometries.None,

            };

            for (int i = 0; i < cfd.GetGridSize(); i++)
                ls.Values.Add(new ObservablePoint(cfd.GetData(i, CFD_A1_OO.DATASET.X), 0));



            SimulationData.Add(ls);
            
        }

    
        

        public int SelectedSolverIndex { set; get; } = 3;

        Thread worker;

        public void RunSteps()
        {
            if(!worker.IsAlive)
            {
                worker = new Thread(() => this.SimulateSteps());
                run4ever = false;
                worker.Start();
            }
            //await Task.Run(() => this.SimulateSteps());
        }

        public void Run()
        {
            if (!worker.IsAlive)
            {
                worker = new Thread(() => this.SimulateSteps());
                run4ever = true;
                worker.Start();
            }
        }

        bool run4ever;

        bool endWorkerFlag;

        public async void stopSimulation()
        {
            endWorkerFlag = true;

            await Task.Run(() => { Thread.Sleep(1000); });

            if (worker.IsAlive)
            {
                worker.Abort();
            }

        }


        protected void SimulateSteps()
        {
            endWorkerFlag = false;
            for (int i = 0; i < steps ||  run4ever ; i += updateInterval)
            {
                
                cfd.DoSteps(updateInterval, SelectedSolverIndex);
                
                this.UpdateData();

                if (endWorkerFlag)
                    break;
            }
        }


        public void ResetModel()
        {
            cfd.Reset();
            UpdateData();
        }

        private double convergence;
        public double ModelConvergence
        {
            get { return convergence; /*(Sigmoid(convergence)-0.5)*2;*/ }
            set
            {
                convergence = value;
                OnPropertyChanged("ModelConvergence");
            }
        }


        static private double Sigmoid(double x)
        {
            return 1/(1 + Math.Exp(-x)); // sigmoid "vereinfacht" ^^
        }




        protected void UpdateData()
        {

            lock (simData)
            {
                int i = 0;                
                double[] pr = cfd.GetPressureArray();
                foreach (ObservablePoint p in simData[0].Values)
                {
                    p.Y = pr[i];
                    i++;
                }
                OnPropertyChanged(nameof(PressureSeries));
                ModelConvergence = cfd.Convergence();
            }
            
        }

    }
}
