﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CFD_A1_OO;
using System.ComponentModel;
using System.Threading;
using System.Collections.ObjectModel;

using LiveCharts;
using LiveCharts.Wpf;
using LiveCharts.Defaults;

namespace CFD_A2_WPF_Metro
{




    public class CfdSolver : INotifyPropertyChanged
    {




        private SeriesCollection simData;

        CfdA1Adapter cfd;

        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged(string name)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }

        public SeriesCollection SimulationData
        {
            get { lock (simData) { return simData; } }
            set
            {
                lock(simData)
                {
                    simData = value;
                }
                OnPropertyChanged("SimulationData");
            }
        }

        private int steps =100;
        public int Steps
        {
            get { return steps; }
            set
            {
                steps = value;
                OnPropertyChanged("Steps");
            }
        }

        private int updateInterval = 2;
        public int UpdatdateInterval
        {
            get { return updateInterval; }
            set
            {
                updateInterval = value;
                OnPropertyChanged("UpdatdateInterval");
            }
        }



        public CfdSolver()
        {

            simData = new SeriesCollection();
            cfd = new CfdA1Adapter();

            AddSeriesCollection("Pressure");
        }

        public void AddSeriesCollection(string name)
        {

            var ls = new LineSeries
            {
                Title = name,
                Values = new ChartValues<ObservablePoint>(),
                PointGeometry = DefaultGeometries.None,

            };

            
            for (int iA = 0; iA < 100; iA++)
            {
                ls.Values.Add(new ObservablePoint());
            }
            

            SimulationData.Add(ls);
        }




        public int SelectedSolverIndex { set; get; } = 0;
         


        public async void RunSteps()
        {
            await Task.Run(() => this.SimulateSteps());
        }

        protected void SimulateSteps()
        {
            for (int i = 0; i < steps; i+= updateInterval)
            {
                cfd.DoSteps(updateInterval, SelectedSolverIndex);

                this.UpdateData();
            }
        }


        protected void UpdateData()
        {
            lock(simData)
            {
                int i=0;
                foreach (ObservablePoint dp in simData[0].Values)
                {
                    dp.X = i;
                    dp.Y = cfd.GetData(i);
                    i++;
                }
            }

            // hier ein on property changed


        }


    }
}
