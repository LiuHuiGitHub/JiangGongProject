using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.IO.Ports;
using System.Threading;


namespace UserSerialPort
{
    public partial class AppSerialPort : Form
    {
        ComRxClass s_comRxBuff = new ComRxClass();
        ComStdClass s_comTxBuff = new ComStdClass();

        private Color buttonSerialPortSwitchColor;
        public SerialPort Comm = new SerialPort();
        private bool b_ReceiveFlag = false;
        public AppSerialPort()
        {
            InitializeComponent();

            //初始化串口号下拉列表框
            comboPortNum.Items.AddRange(SerialPort.GetPortNames());
            comboPortNum.SelectedIndex = 0;

        }
        private bool SerialPortOpen()
        {
            try
            {
                Comm.PortName = comboPortNum.SelectedItem.ToString();
                Comm.BaudRate = 9600;
                Comm.Parity = Parity.None;
                Comm.StopBits = StopBits.One;
                Comm.DataBits = 8;

                Comm.WriteTimeout = 1000;
                Comm.ReadTimeout = 1000;
                Comm.NewLine = "\r\n";      //新行的文本，用于WriteLine方法中由系统附加在text后 

                Comm.Open();
                comboPortNum.Enabled = false;
                buttonSerialPortSwitchColor = buttonSerialPortSwitch.BackColor;
                buttonSerialPortSwitch.BackColor = Color.DeepSkyBlue;
                buttonSerialPortSwitch.Text = "关闭";
                return true;
            }
            catch
            {
                MessageBox.Show("请检查串口是否可用！"
                    , "错误"
                    , MessageBoxButtons.OK
                    , MessageBoxIcon.Warning);
            }
            return false;
        }
        private void SerialPortClose()
        {
            Comm.DataReceived -= CommDataReceiveHandler;    //反注册事件，避免下次再执行进来
            int i = Environment.TickCount;
            while (Environment.TickCount - i < 2000 && b_ReceiveFlag)
            {
                Application.DoEvents();
            }
            Comm.Close();
            buttonSerialPortSwitch.Text = "打开";
            comboPortNum.Enabled = true;
        }
        public string ByteArrayToHexString(Byte[] Bytes, string space)
        {
            string tmpShowString = "";
            for (int i = 0; i < Bytes.Length; i++)//逐字节变为16进制字符，并以space隔开
            {
                tmpShowString += Bytes[i].ToString("X2") + space;
            }
            return tmpShowString;
        }
        void CommDataReceiveHandler(object sender, SerialDataReceivedEventArgs e)
        {
            int rxLen = Comm.BytesToRead;
            if (rxLen > 0)
            {
                b_ReceiveFlag = true;      //开始读 
                byte[] tmpBuf = new byte[rxLen];
                Comm.Read(tmpBuf, 0, rxLen);
                s_comRxBuff.getArray(tmpBuf, rxLen);
                if (s_comRxBuff.Checked())
                {
                    s_comRxBuff.update = true;
                }
                b_ReceiveFlag = false;      //结束读
            }
        }

        void CommClose()
        {
            SerialPortClose();
            buttonClearMoney.Enabled = false;
            buttonAddMoney.Enabled = false;
            button重复充值.Enabled = false;
            timer.Enabled = false;
            pictureBoxPowerOff.Visible = true;
            pictureBoxPowerOn.Visible = false;
            buttonSerialPortSwitch.BackColor = buttonSerialPortSwitchColor;
        }
        private void buttonSerialPortSwitch_Click(object sender, EventArgs e)
        {
            if (Comm.IsOpen)
            {
                CommClose();
            }
            else
            {
                if (SerialPortOpen())
                {
                    buttonClearMoney.Enabled = true;
                    buttonAddMoney.Enabled = true;
                    button重复充值.Enabled = true;
                    timer.Interval = 500;
                    timer.Enabled = true;
                    pictureBoxPowerOff.Visible = false;
                    pictureBoxPowerOn.Visible = true;
                    Comm.DataReceived += CommDataReceiveHandler;
                }
            }
        }
        private void comboPortNum_Click(object sender, EventArgs e)
        {
            //初始化串口号下拉列表框
            comboPortNum.Items.Clear();
            comboPortNum.Items.AddRange(SerialPort.GetPortNames());
            comboPortNum.SelectedIndex = 0;
        }
        public void TextBoxShowString(TextBox textBox, string str)
        {
            textBox.Text += str;
            textBox.SelectionStart = textBox.Text.Length;
            textBox.ScrollToCaret();
        }
        public void TextBoxShowString(TextBox textBox, string str, string newLine)
        {
            textBox.Text += str + newLine;
            textBox.SelectionStart = textBox.Text.Length;
            textBox.ScrollToCaret();
        }
        public void TextBoxShowString(TextBox textBox)
        {
            textBox.Text = null;
        }

        void AddMoney()
        {
            查询等待时间 = 6;
            int u16_money;
            try
            {
                u16_money = int.Parse(textBoxMoney.Text);
                if (u16_money > 200)
                {
                    重复充值 = false;
                    button重复充值.Text = "重复充值";
                    MessageBox.Show("最大充值金额为200元！"
                            , "错误"
                            , MessageBoxButtons.OK
                            , MessageBoxIcon.Warning);
                }
                else
                {
                    s_comTxBuff = new ComStdClass(0x02, (Byte)u16_money);
                    s_comTxBuff.trig = true;
                }
            }
            catch
            {
                重复充值 = false;
                button重复充值.Text = "重复充值";
                MessageBox.Show("请输入正确的金额！"
                        , "错误"
                        , MessageBoxButtons.OK
                        , MessageBoxIcon.Warning);
            }
        }
        private void buttonAddMoney_Click(object sender, EventArgs e)
        {
            重复充值 = false;
            button重复充值.Text = "重复充值";
            查询等待时间 = 6;
            AddMoney();
        }

        private void buttonClearMoney_Click(object sender, EventArgs e)
        {
            重复充值 = false;
            button重复充值.Text = "重复充值";
            查询等待时间 = 6;
            s_comTxBuff = new ComStdClass();
            s_comTxBuff.trig = true;
        }
        Byte TimeInt = 0;
        bool 重复充值 = false;
        Byte 查询等待时间 = 0;
        private void time_Tick(object sender, EventArgs e)
        {
            while (b_ReceiveFlag) ;
            if (s_comTxBuff.trig)
            {
                s_comTxBuff.trig = false;
                try
                {
                    Comm.Write(s_comTxBuff.toArray(), 0, s_comTxBuff.Length);
                }
                catch
                {
                    CommClose();
                    MessageBox.Show("通讯错误，请检查接线并重新打开！"
                            , "错误"
                            , MessageBoxButtons.OK
                            , MessageBoxIcon.Warning);
                }
            }
            else if (查询等待时间 > 0)
            {
                查询等待时间--;
            }
            else        //查询余额
            {
                s_comTxBuff = new ComStdClass(0x01, (Byte)0);
                s_comTxBuff.trig = true;
            }
            if (s_comRxBuff.update)
            {
                s_comRxBuff.update = false;
                if ((s_comRxBuff.cmd == 0x00 || s_comRxBuff.cmd == 0x01 || s_comRxBuff.cmd == 0x02)
                    && s_comRxBuff.value == 0x00
                    && s_comRxBuff.expand)
                {
                    labelShowMoney.Text = string.Format("{0:d3}.{1:d2}元", s_comRxBuff.std.dat[0], s_comRxBuff.std.dat[1]);
                    labelShowMoney.ForeColor = Color.Blue;
                    if (s_comRxBuff.cmd == 0x00)
                    {
                        string paylog = DateTime.Now.ToShortDateString() + " "
                                                   + DateTime.Now.ToLongTimeString() + " "
                                                   + "清零：" + "      "
                                                   + "余额：" + string.Format("{0:d3}.{1:d2}元", s_comRxBuff.std.dat[0], s_comRxBuff.std.dat[1]);
                        TextBoxShowString(textBoxPayMessage, paylog, "\r\n");
                    }
                    else if (s_comRxBuff.cmd == 0x01)
                    {
                        string paylog = DateTime.Now.ToShortDateString() + " "
                                                   + DateTime.Now.ToLongTimeString() + " "
                                                   + "查询：" + "      "
                                                   + "余额：" + string.Format("{0:d3}.{1:d2}元", s_comRxBuff.std.dat[0], s_comRxBuff.std.dat[1]);
                        TextBoxShowString(textBoxPayMessage, paylog, "\r\n");
                    }
                    else if (s_comRxBuff.cmd == 0x02)
                    {
                        string paylog = DateTime.Now.ToShortDateString() + " "
                                                + DateTime.Now.ToLongTimeString() + " "
                                                + "充值：" + string.Format("{0:d3}元", Convert.ToByte(textBoxMoney.Text)) + " "
                                                + "余额：" + string.Format("{0:d3}.{1:d2}元", s_comRxBuff.std.dat[0], s_comRxBuff.std.dat[1]);
                        TextBoxShowString(textBoxPayMessage, paylog, "\r\n");
                        string payLogSave = DateTime.Now.ToShortDateString() + " "
                                                + DateTime.Now.ToLongTimeString() + " "
                                                + "卡号：" + s_comRxBuff.std.dat[2].ToString("X2")
                                                    + s_comRxBuff.std.dat[3].ToString("X2")
                                                    + s_comRxBuff.std.dat[4].ToString("X2")
                                                    + s_comRxBuff.std.dat[5].ToString("X2")
                                                    + s_comRxBuff.std.dat[6].ToString("X2") + " "
                                                + "充值：" + string.Format("{0:d3}元", Convert.ToByte(textBoxMoney.Text)) + " "
                                                + "余额：" + string.Format("{0:d3}.{1:d2}元", s_comRxBuff.std.dat[0], s_comRxBuff.std.dat[1]);
                        using (StreamWriter LogSave = new StreamWriter("充值记录.txt", true))
                        {
                            LogSave.WriteLine(payLogSave);
                            LogSave.Close();
                        };
                    }
                }
            }
            else
            {
                labelShowMoney.ForeColor = Color.Black;
            }
            if (重复充值)
            {
                if (TimeInt == 0)
                {
                    TimeInt = 2;
                }
            }
            else
            {
                TimeInt = 0;
            }

            if (TimeInt > 0)
            {
                TimeInt--;
                if (TimeInt == 0)
                {
                    AddMoney();
                }
            }
        }
        private void textBoxMoney_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != 8 && !Char.IsDigit(e.KeyChar))
            {
                e.Handled = true;
            }
        }

        private void textBoxMoney_TextChanged(object sender, EventArgs e)
        {
            重复充值 = false;
            button重复充值.Text = "重复充值";
        }

        private void AppSerialPort_FormClosed(object sender, FormClosedEventArgs e)
        {
            System.Environment.Exit(0);
        }

        private void button重复充值_Click(object sender, EventArgs e)
        {
            查询等待时间 = 6;
            if (重复充值)
            {
                重复充值 = false;
                button重复充值.Text = "重复充值";
            }
            else
            {
                重复充值 = true;
                button重复充值.Text = "  停止  ";
            }
        }
    }
}
