using System;
using System.Globalization;
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
using System.Windows.Threading;

using LiveCharts;
using LiveCharts.Wpf;
using LiveCharts.Defaults;



namespace CFD_A2_WPF_Metro
{
    /// <summary>
    /// Interaktionslogik für CustomVisual.xaml
    /// </summary>
    public partial class CustomVisual : UserControl
    {
        public CustomVisual()
        {
            InitializeComponent();

        }

        protected override void OnRender(DrawingContext drawingContext)
        {
            base.OnRender(drawingContext);
            MyRender();
            drawingContext.DrawDrawing(backingStore);

        }

        DrawingGroup backingStore = new DrawingGroup();

        private void MyRender()
        {
            if (AreaSeries == null || PressureSeries == null)
                            return;
            var drawingContext = backingStore.Open();


            double leftDrawingoffset = 80;

            double visualWidth = this.ActualWidth- leftDrawingoffset*1.5;
            double visualHeight = this.ActualHeight - leftDrawingoffset*1.5;

            double linewidth = visualWidth/PressureSeries.Length;


            double center = this.ActualHeight / 2;


            double maxp, minp;
            minp = PressureSeries[0];
            maxp = minp;
            foreach (double p in PressureSeries)
            {
                if (p > maxp)
                    maxp = p;

                if (p < minp)
                    minp = p;
            }
            double specp = maxp - minp;


            double maxa;
            maxa = AreaSeries[0];
        
            foreach (double a in AreaSeries)
            {
                if (a > maxa)
                    maxa = a;
            }
           

            for (int i = 0; i < PressureSeries.Length; i++)
            {

                double lineH = AreaSeries[i]/maxa * visualHeight/2;

                
                Pen mypen = new Pen(new SolidColorBrush(GetHeatColor(specp, PressureSeries[i]-minp)), linewidth);
                mypen.Freeze();
                drawingContext.DrawLine(mypen, new Point(i * linewidth + leftDrawingoffset, center - lineH), new Point(i * linewidth + leftDrawingoffset, center + lineH));
            }

            
            // Draw legned // could be a seperate control element


            double legendLineWidth = visualHeight / 255;


            for(int i =0; i<256; i++)
            {
                Pen mypen = new Pen(new SolidColorBrush(GetHeatColor(255, i)), legendLineWidth);
                mypen.Freeze();
                double ypos = (center + visualHeight / 2) - i * legendLineWidth;
                drawingContext.DrawLine(mypen, new Point(10, ypos), new Point(40, ypos));

            }

            FormattedText upperlimText = new FormattedText(
                string.Format("{0} Pa", maxp.ToString("N1")),
                CultureInfo.GetCultureInfo("en-us"),
                FlowDirection.LeftToRight,
                new Typeface("Verdana"),
                14,
                Brushes.Black);

            drawingContext.DrawText(upperlimText, new Point(45, center-visualHeight/2-10));


            FormattedText lowerlimText = new FormattedText(
                string.Format("{0} Pa", minp.ToString("N1")),
                CultureInfo.GetCultureInfo("en-us"),
                FlowDirection.LeftToRight,
                new Typeface("Verdana"),
                14,
                Brushes.Black);

            drawingContext.DrawText(lowerlimText, new Point(45, center + visualHeight / 2-10));

            drawingContext.Close();

        }


        private Color GetHeatColor(double maxval, double val)
        {

            byte valr = (byte)((val/maxval)*255);
            byte valg = (byte)Math.Abs(valr*2-255);
            byte valb = (byte)(255 - valr);

            return Color.FromArgb(100, valr, valg, valb);
        }


        public static readonly DependencyProperty CustomTextProperty =
        DependencyProperty.Register(nameof(CustomText),
        typeof(string),
        typeof(CustomVisual),
        new FrameworkPropertyMetadata(string.Empty, new PropertyChangedCallback(OnTextChanged)));

        
        public static void OnTextChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {  // Break point is hit
            CustomVisual ob = d as CustomVisual;
            ob.MyRender();//InvalidateVisual();
        }


        public string CustomText
        {
            get => (string)GetValue(CustomTextProperty);
            set
            {
                SetValue(CustomTextProperty, value);
                
            }
        }




        public static readonly DependencyProperty PressureSeriesProperty =
        DependencyProperty.Register(nameof(PressureSeries),
        typeof(double[]),
        typeof(CustomVisual),
        new FrameworkPropertyMetadata(null, new PropertyChangedCallback(OnModelChanged)));

        public double[] PressureSeries
        {
            get => (double[])GetValue(PressureSeriesProperty);
            set
            {
                SetValue(PressureSeriesProperty, value);

            }
        }


        public static readonly DependencyProperty AreaSeriesProperty =
        DependencyProperty.Register(nameof(AreaSeries),
        typeof(double[]),
        typeof(CustomVisual),
        new FrameworkPropertyMetadata(null, new PropertyChangedCallback(OnModelChanged)));

        public double[] AreaSeries
        {
            get => (double[])GetValue(AreaSeriesProperty);
            set
            {
                SetValue(AreaSeriesProperty, value);
            }
        }



        public static void OnModelChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {  // Break point is hit
            CustomVisual ob = d as CustomVisual;
            ob.MyRender();//InvalidateVisual();
        }



        




        /// <summary>
        /// The series property
        /// </summary>
        //public static readonly DependencyProperty SeriesProperty = DependencyProperty.Register(
        //    "Series", typeof(SeriesCollection), typeof(CustomVisual),
        //    new PropertyMetadata(default(SeriesCollection), OnSeriesChanged));

        ///// <summary>
        ///// Gets or sets chart series collection to plot.
        ///// </summary>
        //public SeriesCollection Series
        //{
        //    get { return (SeriesCollection)GetValue(SeriesProperty); }
        //    set { SetValue(SeriesProperty, value); }
        //}

        //private static void OnSeriesChanged(DependencyObject o, DependencyPropertyChangedEventArgs e)
        //{
        //    CustomVisual ob = o as CustomVisual;
        //    ob.MyRender();//InvalidateVisual();
        //}





        ///// <summary>

        ///// The series property

        ///// </summary>

        //public static readonly DependencyProperty SeriesProperty = DependencyProperty.Register(

        //    "mySeries", typeof(Series), typeof(CustomVisual),

        //    new FrameworkPropertyMetadata(default(Series), OnSeriesChanged));


        //public static void OnSeriesChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        //{  // Break point is hit
        //    CustomVisual ob = d as CustomVisual;
        //    ob.MyRender();//InvalidateVisual();
        //}


        ///// <summary>
        ///// The values property
        ///// </summary>
        //public static readonly DependencyProperty ValuesProperty = DependencyProperty.Register(
        //    "Values", typeof(IChartValues), typeof(Series),
        //    new PropertyMetadata(default(IChartValues), OnSeriesChanged));
        ///// <summary>
        ///// Gets or sets chart values.
        ///// </summary>
        //public IChartValues Values
        //{
        //    get { return (IChartValues)GetValue(ValuesProperty); }
        //    set { SetValue(ValuesProperty, value); }
        //}


        ///// <summary>

        ///// Gets or sets chart series collection to plot.

        ///// </summary>

        //public Series mySeries

        //{

        //    get {
        //        return (Series)GetValue(SeriesProperty);
        //    }

        //    set {
        //        SetValue(SeriesProperty, value);
        //    }

        //}

        // todo my own dependency property :)

    }
}
