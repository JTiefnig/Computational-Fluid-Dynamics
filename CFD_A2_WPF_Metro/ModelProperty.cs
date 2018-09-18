﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using CFD_A1_OO;

namespace CFD_A2_WPF_Metro
{
    public class ModelProperty : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged(string name)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }

        public String Name { get; private set; }

        CfdA1Adapter cfdModel;

        public ModelProperty(CfdA1Adapter mod)
        {
            cfdModel = mod;
        }
        
        public double Value
        {

            get
            {
                return 0; // wird später das property aus dem model holen
            }

            set
            {

            }

        }


    }
}
