using System.ComponentModel;

namespace CFD_A2_WPF_Metro
{

    /// <summary>
    /// Just basic On Property Changed functionality
    /// </summary>
    /// <author>Johannes Tiefnig</author>
    class BaseViewModel
    {
        public event PropertyChangedEventHandler PropertyChanged = (sender, e) => { };

        protected void OnPropertyChanged(string name) => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
    }
}
