using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace CFD_A2_WPF_Metro
{
    /// <summary>
    /// Interaktionslogik für "App.xaml"
    /// </summary>
    public partial class App : Application
    {

        /// <summary>
        /// Just getting more control over application startup sequence
        /// </summary>
        /// <Author>Johannes Tiefnig</Author>
        /// <param name="e"></param>
        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);

            var solverVM = new SolverViewModel();

            var mainWin = new MainWindow(solverVM);

            mainWin.Show();

        }

    }
}
