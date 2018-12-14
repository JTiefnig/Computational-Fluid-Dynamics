using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using CFD_A1_OO;

using System.IO;

namespace CFD_A2_WPF_Metro
{
    class CsvExporter
    {
        private string filePath;

        public CsvExporter(string path)
        {
            filePath = path;
        }



        public void Export(CfdA1Adapter mod)
        {
            using (StreamWriter writer = new StreamWriter(filePath))
            {
                
                String row = String.Empty;



                // Schreiben von Bezeichnern in die erste zeile
                row = "";
                row += "X;";
                row += "AREA;";
                row += "RHO;";
                row += "RHO_U;";
                row += "E;";
                row += "PRESSURE;";
                row += "U;";
                row += "MACH;";
                row += "T;";
                writer.WriteLine(row);


                

                for(int i =0; i<mod.GetGridSize(); i++)
                {
                    row = string.Empty;

                    double val = mod.GetData(i, DATASET.X);
                    row += val.ToString() + ";";
                    row += mod.GetData(i, DATASET.AREA).ToString() + ";";
                    row += mod.GetData(i, DATASET.RHO).ToString() + ";";
                    row += mod.GetData(i, DATASET.RHO_U).ToString() + ";";
                    row += mod.GetData(i, DATASET.E).ToString() + ";";
                    row += mod.GetData(i, DATASET.PRESSURE).ToString() + ";";
                    row += mod.GetData(i, DATASET.U).ToString() + ";";
                    row += mod.GetData(i, DATASET.MACH).ToString() + ";";
                    row += mod.GetData(i, DATASET.T).ToString() + ";";

                    writer.WriteLine(row);
                }

                writer.Close();
            }
        }
    }




}

