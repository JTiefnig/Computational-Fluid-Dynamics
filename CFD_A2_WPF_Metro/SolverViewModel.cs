﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Threading;
using System.Collections.ObjectModel;
using System.IO;
using Microsoft.Win32;

using LiveCharts;
using LiveCharts.Wpf;
using LiveCharts.Defaults;

using CFD_A1_OO;



/// <summary>
/// Clear View Model Seperation
/// This class functions as a bridge between the c++ solver and the ui
/// </summary>
/// /// <author> Johanes Tiefnig</author>
namespace CFD_A2_WPF_Metro
{
    public class SolverViewModel : BaseViewModel
    {


        #region Properties
        public List<SolverType> SolverTypes => new List<SolverType>()
        {
            new SolverType(){Name= "Central", Selected=0, PrevCFL=0.1},
            new SolverType(){Name= "LaxWendroff", Selected=1, PrevCFL=0.4},
            new SolverType(){Name= "MacCormack", Selected=2, PrevCFL=0.9},
            new SolverType(){Name= "ROE", Selected=3, PrevCFL=0.9}
        };


        private SolverType _solverType;
        public SolverType SelectedSolver
        {
            get => _solverType;
            set
            {
                _solverType = value;

                // change to prevered cfl

                this.CFL= _solverType.PrevCFL;

                OnPropertyChanged(nameof(SelectedSolver));
            }

        }


        // no on property changed !
        public double CFL
        {
            get
            {
                var l = Properties.Where((x) => x.Name == "cfl").ToList();
                double ret =0;
                try
                {
                    ret = l[0].Value;
                }catch
                {
                    // some error
                }

                return ret;
            }
            set
            {
                var l = Properties.Where((x) => x.Name == "cfl").ToList();
                
                try
                {
                    l[0].Value = value;
                }
                catch 
                {
                    // some error
                }
            }
        }

        private CfdA1Adapter solver;


        double _modConv;
        public double ModelConvergence
        {
            get => _modConv;
            set
            {
                _modConv = value;
                OnPropertyChanged(nameof(ModelConvergence));
            }
        }


        private int _selMod;
        public int SelectedModel
        {
            get => _selMod;
            set
            {
                _selMod = value;

                ResetModelExecute();
            }
        }

        public int Steps { set; get; } = 100;

        public int UpdatdateInterval { get; set; } = 10;


        private ObservableCollection<ModelProperty> generateProperties()
        {
            List<String> props = solver.GetParameterList();
            var ret = new ObservableCollection<ModelProperty>();
            foreach (var n in props)
            {
                ret.Add(new ModelProperty(solver, n));
            }

            return ret;
        }


        private ObservableCollection<ModelProperty> _properties;

        public ObservableCollection<ModelProperty> Properties 
        {
            get
            {
                if (_properties == null)
                    _properties = generateProperties();

                return _properties;
            }

        }


        private bool run4ever;
        private bool endWorkerFlag;


        public int StepCount => solver.StepCount();

        public double ModelTime => solver.ModelTime();

        // some bridge functionality
        public double[] PressureSeries
        {
            get { return solver.GetPressureArray(); }
        }

        public double[] AreaSeries
        {
            get { return solver.GetDataArray(DATASET.AREA); }
        }
      

        private Thread worker;

        #endregion


        public SolverViewModel()
        {
            // Create Solver C++ Instance
            try
            {
                solver = new CfdA1Adapter(SelectedModel);
            }catch(Exception e)
            {
                MessageBox.Show(e.Message);
            }

            // initial solver: Central--
            SelectedSolver = SolverTypes[0];

            worker = new Thread(() => this.SimulateStepsFunc());

            InitSeries();

        }


        #region Commands

        public RelayCommand RunSimulation => new RelayCommand(RunSimulationExecute);
        public RelayCommand RunSteps => new RelayCommand(RunStepsExecute);
        public RelayCommand ResetModel => new RelayCommand(ResetModelExecute);
        public RelayCommand StopSimulation => new RelayCommand(StopSimulationExecute);
        public RelayCommand SaveParameter => new RelayCommand(()=> solver.SavePropertiesToXml());

        public RelayCommand ExportCommand => new RelayCommand(ExportRoutine);


        #endregion

        #region Simulation

        private void RunSimulationExecute()
        {
            if (!worker.IsAlive)
            {
                worker = new Thread(() => this.SimulateStepsFunc());
                worker.IsBackground = true;
                run4ever = true;
                worker.Start();
            }
        }

        private void RunStepsExecute()
        {
            if (!worker.IsAlive)
            {
                worker = new Thread(() => this.SimulateStepsFunc());
                worker.IsBackground = true;
                run4ever = false;
                worker.Start();
            }
        }

        private void ResetModelExecute()
        {
            solver.Reset(SelectedModel);
            InitSeries();
            UpdateData();
           
        }

        private async void StopSimulationExecute()
        {
            endWorkerFlag = true;

            await Task.Run(() => { Thread.Sleep(1000); });

            if (worker.IsAlive)
            {
                worker.Abort();
            }

        }


        protected void SimulateStepsFunc()
        {
            endWorkerFlag = false;
            for (int i = 0; i < Steps || run4ever; i += UpdatdateInterval)
            {

                solver.DoSteps(UpdatdateInterval, SelectedSolver.Selected);

                UpdateData();

                if (endWorkerFlag)
                    break;
            }
        }


        protected void UpdateData()
        {

            lock (SimulationData)
            {
                try
                {

                    int i = 0;
                    foreach (ObservablePoint p in pressurels.Values)
                    {
                        p.Y = solver.GetData(i, DATASET.PRESSURE);
                        i++;
                    }


                    i = 0;
                    foreach (ObservablePoint p in machls.Values)
                    {
                        p.Y = solver.GetData(i, DATASET.MACH);
                        i++;
                    }

                    
                    ModelConvergence = solver.Convergence();
                }catch(Exception e)
                {
                    MessageBox.Show(e.Message);
                    StopSimulationExecute();
                }

            }

            
            OnPropertyChanged(nameof(PressureSeries));
            OnPropertyChanged(nameof(StepCount));
            OnPropertyChanged(nameof(ModelTime));
            OnPropertyChanged(nameof(AreaSeries));
        }





        #endregion



        #region DiagramProperties


        SeriesCollection _simdata;
        public SeriesCollection SimulationData
        {
            get { return _simdata; }
            set
            {
                _simdata = value;
                OnPropertyChanged(nameof(SimulationData));
            }
        }

        public AxesCollection YAxesCollection
        {
            get; set;
        }

        LineSeries pressurels;
        LineSeries machls;

        #endregion

        #region Utils


        void InitSeries()
        {

            Axis paAxis;
            Axis maAxis;


            YAxesCollection = new AxesCollection();
            paAxis = new Axis();
            paAxis.Title = "Pa";
            paAxis.Visibility = System.Windows.Visibility.Visible;


            maAxis = new Axis();
            maAxis.Title = "Ma";
            maAxis.Visibility = System.Windows.Visibility.Visible;


            pressurels = new LineSeries
            {
                Title = "Pressure",
                Values = new ChartValues<ObservablePoint>(),
                PointGeometry = DefaultGeometries.None,

            };
            for (int i = 0; i < solver.GetGridSize(); i++)
                pressurels.Values.Add(new ObservablePoint(solver.GetData(i, CFD_A1_OO.DATASET.X), solver.GetData(i, DATASET.PRESSURE)));

            machls = new LineSeries
            {
                Title = "Mach",
                Values = new ChartValues<ObservablePoint>(),
                PointGeometry = DefaultGeometries.None,

            };
            for (int i = 0; i < solver.GetGridSize(); i++)
                machls.Values.Add(new ObservablePoint(solver.GetData(i, CFD_A1_OO.DATASET.X), solver.GetData(i, DATASET.MACH)));

            

            SimulationData = new SeriesCollection();
            YAxesCollection = new AxesCollection();
         
            YAxesCollection.Add(paAxis);
            pressurels.ScalesYAt = YAxesCollection.IndexOf(paAxis);
            SimulationData.Add(pressurels);
          
            YAxesCollection.Add(maAxis);
            machls.ScalesYAt = YAxesCollection.IndexOf(maAxis);
            SimulationData.Add(machls);

            OnPropertyChanged(nameof(YAxesCollection));
            OnPropertyChanged(nameof(SimulationData));
        }



        private void ExportRoutine()
        {
            SaveFileDialog sfd = new SaveFileDialog
            {
                Filter = "CSV file (*.csv)|*.csv"
            };

            if (sfd.ShowDialog() == true)
            {
                try
                {
                    var exp = new CsvExporter(sfd.FileName.ToString());
                    exp.Export(solver);
                }
                catch (Exception exception)
                {
                    MessageBox.Show("A handled exception just occurred: " + exception.Message, "ERROR", MessageBoxButton.OK, MessageBoxImage.Warning);
                }
            }
        }






        #endregion

    }


    public class SolverType
    {

        public String Name { get; set; }

        public int Selected { get; set; }

        public double PrevCFL { get; set; } = 0.1;

        public override string ToString()
        {
            return Name;
        }
    }



}



