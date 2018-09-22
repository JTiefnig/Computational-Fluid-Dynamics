using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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



        public void Export()
        {
            using (StreamWriter writer = new StreamWriter(filePath))
            {
                
                String row = String.Empty;

               

                //row = nameof(VehiclePackage.PROJECT_CODE_ID) + ";;";
                //foreach (VehiclePackage vp in list)
                //{
                //    row += vp.PROJECT_CODE_ID + ";";
                //}
                //writer.WriteLine(row);

                //row = nameof(VehiclePackage.PROJECT_NAME) + ";;";
                //foreach (VehiclePackage vp in list)
                //{
                //    row += vp.PROJECT_NAME + ";";
                //}
                //writer.WriteLine(row);

                //row = nameof(VehiclePackage.OEM) + ";;";
                //foreach (VehiclePackage vp in list)
                //{
                //    row += vp.OEM + ";";
                //}
                //writer.WriteLine(row);

                //row = nameof(VehiclePackage.PROJECT_TYP) + ";;";
                //foreach (VehiclePackage vp in list)
                //{
                //    row += vp.PROJECT_TYP + ";";
                //}
                //writer.WriteLine(row);

                //row = nameof(VehiclePackage.PROJECT_SEG) + ";;";
                //foreach (VehiclePackage vp in list)
                //{
                //    row += vp.PROJECT_SEG + ";";
                //}
                //writer.WriteLine(row);

                //row = "GCIE_DEF;GCIE_DIM;";
                //writer.WriteLine(row);

                //Dictionary<string, List<string>> dimRows = new Dictionary<string, List<string>>();

                
                //int packageID = 0;
                //foreach (VehiclePackage vp in list)
                //{
                //    foreach (Dimension pair in vp)
                //    {
                //        if (dimRows.ContainsKey(pair.Ident))
                //        {
                //            dimRows[pair.Ident][packageID] = pair.Value;
                //        }
                //        else
                //        {
                //            String[] l = new String[list.Count];

                //            l[packageID] = pair.Value;
                //            dimRows.Add(pair.Ident, new List<string>(l));
                //        }
                //    }
                //    packageID++;
                //}

                //foreach (KeyValuePair<string, List<string>> pair in dimRows)
                //{
                //    row = string.Empty;
                //    row += pair.Key + ";";
                //    row += "[mm];";

                //    foreach (string sval in pair.Value)
                //    {
                //        if (!string.IsNullOrEmpty(sval))
                //        { row += sval; }
                //        row += ";";
                //    }
                //    writer.WriteLine(row);
                //}

                writer.Close();
            }
        }
    }




}

