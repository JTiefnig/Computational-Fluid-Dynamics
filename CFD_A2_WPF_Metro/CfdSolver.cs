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
        bool showPa = true;
        public bool ShowPressure
        {
            set
            {
                showPa = value;
                UpdateSeriesConfig();
            }
            get { return showPa; }
        }
        bool showMa = true;
        public bool ShowMach
        {
            set
            {
                showMa = value;
                UpdateSeriesConfig();
            }
            get { return showMa; }
        }
        bool showVel = true;
        public bool ShowVelocity
        {
            set
            {
                showVel = value;
                UpdateSeriesConfig();
            }
            get { return showVel; }
        }


        public int SelectedModel
        {
            set
            {
                switch(value)
                {
                    case 0:
                        cfd.selectedModel = MODEL.A1;
                        break;
                    case 1:
                        cfd.selectedModel = MODEL.A2;
                        break;
                }
                ResetModel();

            }

        }



        public AxesCollection YAxesCollection
        {
            get; set;
        }


        LineSeries pressurels;
        LineSeries machls;
        LineSeries vells;

        Axis paAxis;
        Axis maAxis;
        Axis velAxis;


        public CfdSolver()
        {
            simData = new SeriesCollection();

            YAxesCollection = new AxesCollection();

            cfd = new CfdA1Adapter(MODEL.A1);

            

            //cfd.SavePropertiesToXml();

            Properties = new ObservableCollection<ModelProperty>();
            GenerateParameterlist();

            InitSeries();

            worker = new Thread(() => this.SimulateSteps());

        }


        public CfdA1Adapter GetModel()
        {
            return cfd;
        }

        public ObservableCollection<ModelProperty> Properties { get; set; }

        private void GenerateParameterlist()
        {
            List<String> props = cfd.GetParameterList();

            foreach(var n in props)
            {
                Properties.Add(new ModelProperty(cfd, n));
            }
      

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
            get { return simData; }
            set
            {
                
                    simData = value;
                
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
            get { return cfd.GetPressureArray(); }
        }


        public double[] AreaSeries
        {
            get { return cfd.GetDataArray(DATASET.AREA); }
        }


        void InitSeries()
        {



            // Init graph Axis

            YAxesCollection = new AxesCollection();
            paAxis = new Axis();
            paAxis.Title = "Pa";
            paAxis.Visibility = System.Windows.Visibility.Visible;
            //YAxesCollection.Add(paAxis);

            maAxis = new Axis();
            maAxis.Title = "Ma";
            maAxis.Visibility = System.Windows.Visibility.Visible;
            //YAxesCollection.Add(maAxis);

            velAxis = new Axis();
            velAxis.Title = "m/s";
            velAxis.Visibility = System.Windows.Visibility.Visible;
            //YAxesCollection.Add(velAxis);




            pressurels = new LineSeries
            {
                Title = "Pressure",
                Values = new ChartValues<ObservablePoint>(),
                PointGeometry = DefaultGeometries.None,

            };
            for (int i = 0; i < cfd.GetGridSize(); i++)
                pressurels.Values.Add(new ObservablePoint(cfd.GetData(i, CFD_A1_OO.DATASET.X), cfd.GetData(i, DATASET.PRESSURE)));

            machls = new LineSeries
            {
                Title = "Mach",
                Values = new ChartValues<ObservablePoint>(),
                PointGeometry = DefaultGeometries.None,

            };
            for (int i = 0; i < cfd.GetGridSize(); i++)
                machls.Values.Add(new ObservablePoint(cfd.GetData(i, CFD_A1_OO.DATASET.X), cfd.GetData(i, DATASET.MACH)));

            //vells = new LineSeries
            //{
            //    Title = "Velocity",
            //    Values = new ChartValues<ObservablePoint>(),
            //    PointGeometry = DefaultGeometries.None

            //};
            //for (int i = 0; i < cfd.GetGridSize(); i++)
            //    vells.Values.Add(new ObservablePoint(cfd.GetData(i, CFD_A1_OO.DATASET.X), cfd.GetData(i, DATASET.U)));

            UpdateSeriesConfig();

        }


        void UpdateSeriesConfig()
        {

            simData = new SeriesCollection();
            YAxesCollection = new AxesCollection();

            //if (SimulationData.Count != 0)
            //    SimulationData.Clear();
            //if (YAxesCollection.Count != 0)
            //    YAxesCollection.Clear();


            

            if (ShowPressure)
            {
                YAxesCollection.Add(paAxis);
                pressurels.ScalesYAt = YAxesCollection.IndexOf(paAxis);
                SimulationData.Add(pressurels);
            }

            if(ShowMach)
            {
                YAxesCollection.Add(maAxis);
                machls.ScalesYAt = YAxesCollection.IndexOf(maAxis);
                SimulationData.Add(machls);

            }

            //if(ShowVelocity)
            //{
            //    YAxesCollection.Add(velAxis);
            //    vells.ScalesYAt = YAxesCollection.IndexOf(velAxis);
            //    SimulationData.Add(vells);
            //}



            OnPropertyChanged(nameof(YAxesCollection));
            OnPropertyChanged(nameof(SimulationData));


        }

        public int SelectedSolverIndex { set; get; } = 3;

        Thread worker;

        public void RunSteps()
        {
            if (!worker.IsAlive)
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
            for (int i = 0; i < steps || run4ever; i += updateInterval)
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
            OnPropertyChanged(nameof(AreaSeries));
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
            return 1 / (1 + Math.Exp(-x)); // sigmoid "vereinfacht" ^^
        }


        public int StepCount
        {
            get { return cfd.StepCount(); }
        }

        protected void UpdateData()
        {

            lock (simData)
            {


                if(showPa)
                {
                    int i = 0;
                    foreach (ObservablePoint p in pressurels.Values)
                    {
                        p.Y = cfd.GetData(i, DATASET.PRESSURE);
                        i++;
                    }
                }

                if (showMa)
                {
                    int i = 0;
                    foreach (ObservablePoint p in machls.Values)
                    {
                        p.Y = cfd.GetData(i, DATASET.MACH);
                        i++;
                    }
                }

                //if (showVel)
                //{
                //    int i = 0;
                //    foreach (ObservablePoint p in vells.Values)
                //    {
                //        p.Y = cfd.GetData(i, DATASET.U);
                //        i++;
                //    }
                //}


                OnPropertyChanged(nameof(PressureSeries));
                ModelConvergence = cfd.Convergence();
                OnPropertyChanged(nameof(StepCount));
                
            }
            
        }

    }
}
