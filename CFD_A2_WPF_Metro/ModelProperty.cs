using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using CFD_A1_OO;

namespace CFD_A2_WPF_Metro
{
    public class ModelProperty : BaseViewModel
    {

        public String Name { get; private set; }

        CfdA1Adapter cfdModel;

        public ModelProperty(CfdA1Adapter mod, string name)
        {
            Name = name;
            cfdModel = mod;
        }
        
        public double Value
        {

            get
            {
                return cfdModel.GetParameter(Name); 
            }

            set
            {
                cfdModel.SetParameter(Name, value);
            }

        }


    }
}
