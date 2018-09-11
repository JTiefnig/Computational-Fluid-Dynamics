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
            var drawingContext = backingStore.Open();

            const int linewidth = 5;

            for(int i=0; i<this.ActualWidth-20; i+= linewidth)
            {
                Pen mypen = new Pen(new SolidColorBrush(Color.FromArgb(255, 255, (byte)(255 - i), (byte)i)), linewidth);
                mypen.Freeze();
                drawingContext.DrawLine(mypen, new Point(i + 20, 20), new Point(i+20, 200));
            }
            
            


            FormattedText formattedText = new FormattedText(
                CustomText,
                CultureInfo.GetCultureInfo("en-us"),
                FlowDirection.LeftToRight,
                new Typeface("Verdana"),
                32,
                Brushes.Black);
            drawingContext.DrawText(formattedText, new Point(200, 50));
            drawingContext.Close();

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

        // todo my own dependency property :)

    }
}
