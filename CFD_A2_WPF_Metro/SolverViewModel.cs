using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Collections.ObjectModel;

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
    public class SolverViewModel: BaseViewModel
    {


        #region Properties
        public List<SolverType> SolverTypes => new List<SolverType>()
        {
            new SolverType(){Name= "Central", Selected=0},
            new SolverType(){Name= "LaxWendroff", Selected=1},
            new SolverType(){Name= "MC Cormack", Selected=2},
            new SolverType(){Name= "ROE", Selected=3}
        };

        public SolverType SelectedSolver { get; set; }

        private CfdA1Adapter solver;


        double _modConv;
        public double ModelConvergence
        {
            get => _modConv;
            set
            {
                _modConv = ModelConvergence;
                OnPropertyChanged(nameof(ModelConvergence));
            }
        }

        public int SelectedModel { get; set; } = 1; // todo


        public int Seps { set; get; } = 100;

        public int UpdatdateInterval { get; set; } = 10;

        public ObservableCollection<ModelProperty> Properties { get; set; }

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



        // some bridge functionality
        public double[] PressureSeries
        {
            get { return solver.GetPressureArray(); }
        }

        public double[] AreaSeries
        {
            get { return solver.GetDataArray(DATASET.AREA); }
        }


        #endregion


        public SolverViewModel()
        {
            // Create Solver C++ Instance
            try
            {
                solver = new CfdA1Adapter();
            }catch(Exception e)
            {
                MessageBox.Show(e.Message);
            }

            Properties = new ObservableCollection<ModelProperty>();


        }


        #region Commands

        RelayCommand RunSimulation => new RelayCommand(RunSimulationExecute);
        RelayCommand RunSteps => new RelayCommand(RunStepsExecute);
        RelayCommand ResetModel => new RelayCommand(ResetModelExecute);
        RelayCommand StopSimulation => new RelayCommand(StopSimulationExecute);

        #endregion

        #region Simulation

        private void RunSimulationExecute()
        {

        }

        private void RunStepsExecute()
        {

        }

        private void ResetModelExecute()
        {

        }

        private void StopSimulationExecute()
        {

        }


        #endregion


        #region Utils

        

        private void GenerateParameterlist()
        {
            List<String> props = solver.GetParameterList();

            foreach (var n in props)
            {
                Properties.Add(new ModelProperty(solver, n));
            }


        }


        #endregion

    }


    public class SolverType
    {

        public String Name { get; set; }

        public int Selected { get; set; }

        public override string ToString()
        {
            return Name;
        }
    }



}



