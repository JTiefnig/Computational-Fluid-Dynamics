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
using MahApps.Metro.Controls;
using System.IO;
using Microsoft.Win32;

namespace CFD_A2_WPF_Metro
{
    /// <summary>
    /// Interaktionslogik für MainWindow.xaml
    /// <autor> Johannes Tiefnig</autor>
    /// CFD TU Graz 2018
    /// </summary>
    /// 
    public partial class MainWindow : MetroWindow
    {
        private CfdSolver solverAdapt;

        public CfdSolver SolverAdapter
        {
            get { return solverAdapt; }
            set { solverAdapt = value; }
        }

        public MainWindow()
        {
            InitializeComponent();
            SolverAdapter = new CfdSolver();

            DataContext = this;
        }

        private void RunButton_Click(object sender, RoutedEventArgs e)
        {
            solverAdapt.Run();
        }

        private void RunStepsButton_Click(object sender, RoutedEventArgs e)
        {
            solverAdapt.RunSteps();
        }

        private void ResetButton_Click(object sender, RoutedEventArgs e)
        {
            solverAdapt.ResetModel();
        }

        private void StopButton_Click(object sender, RoutedEventArgs e)
        {
            solverAdapt.stopSimulation();
        }

        private void SettingsButton_Click(object sender, RoutedEventArgs e)
        {
            var diag = new ParameterSettingsWindow(solverAdapt.Properties);

            diag.Show();



        }

        private void AddSeriesButton_Click(object sender, RoutedEventArgs e)
        {
           
        }

        private void RemoveSeries_Click(object sender, RoutedEventArgs e)
        {

        }


        private void ExportButton_Click(object sender, RoutedEventArgs e)
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
                    exp.Export(solverAdapt.GetModel());
                }
                catch (Exception exception)
                {
                    MessageBox.Show("A handled exception just occurred: " + exception.Message, "ERROR", MessageBoxButton.OK, MessageBoxImage.Warning);
                }
            }
        }

        private void SaveParams_Click(object sender, RoutedEventArgs e)
        {
            solverAdapt.SaveParametersToFile();
        }




    }
}
