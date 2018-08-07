using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;


using System.Windows.Threading;
using System.Threading;
using LiveCharts;
using LiveCharts.Wpf;
using LiveCharts.Defaults;
using System.ComponentModel;

using MahApps.Metro.Controls;


namespace CFD_A1_WPF
{
    /// <summary>
    /// Interaktionslogik für MainWindow.xaml
    /// </summary>
    public partial class MainWindow : MetroWindow, INotifyPropertyChanged
    {
        CfdBackgroundSolver backgroundWorker;


        private SeriesCollection cfdcol;
        

        public SeriesCollection CfdCollection {
            get
            {
                return cfdcol;
            }

            private set
            {
                cfdcol = value;
                OnPropertyChanged("CfdCollection");
            }
        }

        int stepsToCalculate = 100;

        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged(string name)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }

        public MainWindow()
        {
            InitializeComponent();

            backgroundWorker = new CfdBackgroundSolver();

            CfdCollection = new SeriesCollection();
            DataContext = this;

            backgroundWorker.dataRefreshed += GetRefreshedData;

        }

        private void Run_Button_Click(object sender, RoutedEventArgs e)
        { 

            int solver = SolverSelector_ComboBox.SelectedIndex;
            backgroundWorker.StartSolver(solver, stepsToCalculate);
        }

        void GetRefreshedData()
        {
            this.Dispatcher.Invoke(() =>  this.UpdateChart() );
        }


        double cfdConv;


        public double CfdSolvConv
        {
            get { return cfdConv; }
            set
            {
                cfdConv = value;
                this.OnPropertyChanged("CfdSolvConv");
            }
        }

        private void UpdateChart()
        {


            LineSeries ls;

            if (CfdCollection.Count == 0)
            {
                ls = new LineSeries
                {
                    Title = "Pressure",
                    Values = new ChartValues<ObservablePoint>(),
                    PointGeometry = DefaultGeometries.None,

                };
                
                for(int iA = 0; iA<100; iA++)
                {
                    ls.Values.Add(new ObservablePoint());
                }
                CfdCollection.Add(ls);
            }


            List<double> list = new List<double>(backgroundWorker.GetData());

            int i = 0;

            foreach(ObservablePoint dp in CfdCollection[0].Values)
            {
                dp.X = i;
                dp.Y = list[i];
                i++;
            }

            //OnPropertyChanged("CfdCollection");

            

        }

        private void Button_Reset_Click(object sender, RoutedEventArgs e)
        {
            backgroundWorker.ResetModel();
        }

        private void TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            



        }

        private void StepsInputBox_TextInput(object sender, TextCompositionEventArgs e)
        {
            string textinput = StepsInputBox.Text;
            try
            {
                stepsToCalculate = Int32.Parse(textinput);
            }
            catch (Exception)
            {
                MessageBox.Show("Only int input!");
            }
        }
    }
}
