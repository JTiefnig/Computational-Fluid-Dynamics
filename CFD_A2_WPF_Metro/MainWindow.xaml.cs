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


namespace CFD_A2_WPF_Metro
{
    /// <summary>
    /// Interaktionslogik für MainWindow.xaml
    /// </summary>
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

        }

        private void RunStepsButton_Click(object sender, RoutedEventArgs e)
        {
            //solverAdapt.RunSteps();
        }
    }
}
