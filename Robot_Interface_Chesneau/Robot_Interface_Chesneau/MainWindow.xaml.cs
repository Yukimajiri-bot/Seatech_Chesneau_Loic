using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using ExtendedSerialPort_NS;
using System.IO.Ports;
using System.Diagnostics.Tracing;
using Microsoft.Win32.SafeHandles;
using System.Reflection;

namespace Robot_Interface_Chesneau
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        ExtendedSerialPort SerialPort1;
        public MainWindow()
        {
            InitializeComponent();
            SerialPort1 = new ExtendedSerialPort("COM5", 115200, Parity.None, 8, StopBits.One);
            SerialPort1.DataReceived += SerialPort1_DataReceived;
            SerialPort1.Open();
            
        }
        private void Envoyer_Click(object sender, RoutedEventArgs e)
        {
            SendMessages();
        }
        public void SendMessages()
        {
            SerialPort1.WriteLine(TextBoxEmission.Text);
        }

        public void SerialPort1_DataReceived(object sender, DataReceivedArgs e)
        {
            SerialPort1.WriteLine(TextBoxRéception.Text);

        }

        private void TextBoxEmission_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
                SendMessages();
        }
        

        private void ButtonClear_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}