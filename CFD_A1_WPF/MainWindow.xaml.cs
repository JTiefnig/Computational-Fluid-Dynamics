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


namespace CFD_A1_WPF
{
    /// <summary>
    /// Interaktionslogik für MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        CfdBackgroundSolver backgroundWorker;

        public SeriesCollection CfdCollection { get; private set; }

        int stepsToCalculate = 100;
     


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
            this.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                        (ThreadStart)delegate ()
                        {
                            this.UpdateChart();
                        }
                          );
        }


        public double CfdSolvConv
        {
            get { return 33.664d; }
            set
            {
                 
            }
        }

        private void UpdateChart()
        {


            LineSeries ls;

            if (CfdCollection.Count == 0)
            {
                ls = new LineSeries
                {
                    Title = "Series 1",
                    Values = new ChartValues<ObservablePoint>(),
                    PointGeometry = DefaultGeometries.None,

                };
            }
            else
            {
                ls = CfdCollection[0] as LineSeries;
            }



            List<double> list = backgroundWorker.GetData();

            int i = 0;


            lock (list)
            {
                foreach (double v in list)
                {

                    if (i < ls.Values.Count)
                    {
                        ObservablePoint point = ls.Values[i] as ObservablePoint;
                        point.Y = v;
                    }
                    else
                    {
                        ls.Values.Add(new ObservablePoint(i, v));
                    }


                    i++;

                }
            }


            CfdCollection.Clear();

            CfdCollection.Add(ls);

            DataContext = this;

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
