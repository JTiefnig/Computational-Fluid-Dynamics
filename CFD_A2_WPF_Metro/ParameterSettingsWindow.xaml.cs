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
using System.Windows.Shapes;
using System.Collections.ObjectModel;

namespace CFD_A2_WPF_Metro
{
    /// <summary>
    /// Interaktionslogik für ParameterSettingsWindow.xaml
    /// </summary>
    public partial class ParameterSettingsWindow : Window
    {

        public ObservableCollection<ModelProperty> Properties { get; set; }

        public ParameterSettingsWindow(ObservableCollection<ModelProperty> modProps)
        {
           
            InitializeComponent();
            Properties = modProps;
            DataContext = this; 
        }

        private void ExitButton_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
