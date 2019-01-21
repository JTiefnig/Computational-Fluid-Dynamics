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
    /// UI logic 
    /// <autor> Johannes Tiefnig</autor>
    /// CFD TU Graz 2018/19
    /// </summary>
    /// 
    public partial class MainWindow : MetroWindow
    {


        // need this later
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

    }
}
