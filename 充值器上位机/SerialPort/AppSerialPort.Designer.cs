namespace UserSerialPort
{
    partial class AppSerialPort
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AppSerialPort));
            this.comboPortNum = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBoxCommSetting = new System.Windows.Forms.GroupBox();
            this.pictureBoxPowerOn = new System.Windows.Forms.PictureBox();
            this.pictureBoxPowerOff = new System.Windows.Forms.PictureBox();
            this.buttonSerialPortSwitch = new System.Windows.Forms.Button();
            this.textBoxPayMessage = new System.Windows.Forms.TextBox();
            this.groupBoxTransmitData = new System.Windows.Forms.GroupBox();
            this.textBoxMoney = new System.Windows.Forms.TextBox();
            this.groupBoxReceiveData = new System.Windows.Forms.GroupBox();
            this.buttonClearMoney = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.buttonAddMoney = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.labelShowMoney = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.timer = new System.Windows.Forms.Timer(this.components);
            this.button重复充值 = new System.Windows.Forms.Button();
            this.groupBoxCommSetting.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxPowerOn)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxPowerOff)).BeginInit();
            this.groupBoxTransmitData.SuspendLayout();
            this.groupBoxReceiveData.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // comboPortNum
            // 
            this.comboPortNum.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.comboPortNum.FormattingEnabled = true;
            this.comboPortNum.Location = new System.Drawing.Point(73, 27);
            this.comboPortNum.Name = "comboPortNum";
            this.comboPortNum.Size = new System.Drawing.Size(74, 27);
            this.comboPortNum.TabIndex = 0;
            this.comboPortNum.Click += new System.EventHandler(this.comboPortNum_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(13, 30);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(57, 19);
            this.label1.TabIndex = 4;
            this.label1.Text = "端口:";
            // 
            // groupBoxCommSetting
            // 
            this.groupBoxCommSetting.Controls.Add(this.pictureBoxPowerOn);
            this.groupBoxCommSetting.Controls.Add(this.pictureBoxPowerOff);
            this.groupBoxCommSetting.Controls.Add(this.buttonSerialPortSwitch);
            this.groupBoxCommSetting.Controls.Add(this.comboPortNum);
            this.groupBoxCommSetting.Controls.Add(this.label1);
            this.groupBoxCommSetting.Location = new System.Drawing.Point(10, 9);
            this.groupBoxCommSetting.Name = "groupBoxCommSetting";
            this.groupBoxCommSetting.Size = new System.Drawing.Size(166, 123);
            this.groupBoxCommSetting.TabIndex = 8;
            this.groupBoxCommSetting.TabStop = false;
            this.groupBoxCommSetting.Text = "设置";
            // 
            // pictureBoxPowerOn
            // 
            this.pictureBoxPowerOn.Enabled = false;
            this.pictureBoxPowerOn.Image = ((System.Drawing.Image)(resources.GetObject("pictureBoxPowerOn.Image")));
            this.pictureBoxPowerOn.Location = new System.Drawing.Point(12, 63);
            this.pictureBoxPowerOn.Name = "pictureBoxPowerOn";
            this.pictureBoxPowerOn.Size = new System.Drawing.Size(47, 43);
            this.pictureBoxPowerOn.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBoxPowerOn.TabIndex = 12;
            this.pictureBoxPowerOn.TabStop = false;
            this.pictureBoxPowerOn.Visible = false;
            // 
            // pictureBoxPowerOff
            // 
            this.pictureBoxPowerOff.Image = ((System.Drawing.Image)(resources.GetObject("pictureBoxPowerOff.Image")));
            this.pictureBoxPowerOff.Location = new System.Drawing.Point(12, 63);
            this.pictureBoxPowerOff.Name = "pictureBoxPowerOff";
            this.pictureBoxPowerOff.Size = new System.Drawing.Size(47, 43);
            this.pictureBoxPowerOff.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBoxPowerOff.TabIndex = 11;
            this.pictureBoxPowerOff.TabStop = false;
            // 
            // buttonSerialPortSwitch
            // 
            this.buttonSerialPortSwitch.Cursor = System.Windows.Forms.Cursors.Hand;
            this.buttonSerialPortSwitch.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.buttonSerialPortSwitch.Location = new System.Drawing.Point(73, 71);
            this.buttonSerialPortSwitch.Name = "buttonSerialPortSwitch";
            this.buttonSerialPortSwitch.Size = new System.Drawing.Size(74, 30);
            this.buttonSerialPortSwitch.TabIndex = 10;
            this.buttonSerialPortSwitch.Text = "打开";
            this.buttonSerialPortSwitch.UseVisualStyleBackColor = true;
            this.buttonSerialPortSwitch.Click += new System.EventHandler(this.buttonSerialPortSwitch_Click);
            // 
            // textBoxPayMessage
            // 
            this.textBoxPayMessage.ImeMode = System.Windows.Forms.ImeMode.Disable;
            this.textBoxPayMessage.Location = new System.Drawing.Point(8, 16);
            this.textBoxPayMessage.Multiline = true;
            this.textBoxPayMessage.Name = "textBoxPayMessage";
            this.textBoxPayMessage.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBoxPayMessage.Size = new System.Drawing.Size(308, 240);
            this.textBoxPayMessage.TabIndex = 22;
            // 
            // groupBoxTransmitData
            // 
            this.groupBoxTransmitData.Controls.Add(this.textBoxPayMessage);
            this.groupBoxTransmitData.Location = new System.Drawing.Point(182, 9);
            this.groupBoxTransmitData.Name = "groupBoxTransmitData";
            this.groupBoxTransmitData.Size = new System.Drawing.Size(323, 265);
            this.groupBoxTransmitData.TabIndex = 26;
            this.groupBoxTransmitData.TabStop = false;
            this.groupBoxTransmitData.Text = "消息";
            // 
            // textBoxMoney
            // 
            this.textBoxMoney.Cursor = System.Windows.Forms.Cursors.IBeam;
            this.textBoxMoney.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.textBoxMoney.ImeMode = System.Windows.Forms.ImeMode.Disable;
            this.textBoxMoney.Location = new System.Drawing.Point(70, 19);
            this.textBoxMoney.MaxLength = 3;
            this.textBoxMoney.Name = "textBoxMoney";
            this.textBoxMoney.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBoxMoney.Size = new System.Drawing.Size(53, 29);
            this.textBoxMoney.TabIndex = 9;
            this.textBoxMoney.Text = "0";
            this.textBoxMoney.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.textBoxMoney.TextChanged += new System.EventHandler(this.textBoxMoney_TextChanged);
            this.textBoxMoney.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBoxMoney_KeyPress);
            // 
            // groupBoxReceiveData
            // 
            this.groupBoxReceiveData.Controls.Add(this.button重复充值);
            this.groupBoxReceiveData.Controls.Add(this.buttonClearMoney);
            this.groupBoxReceiveData.Controls.Add(this.label3);
            this.groupBoxReceiveData.Controls.Add(this.buttonAddMoney);
            this.groupBoxReceiveData.Controls.Add(this.label2);
            this.groupBoxReceiveData.Controls.Add(this.textBoxMoney);
            this.groupBoxReceiveData.Location = new System.Drawing.Point(12, 280);
            this.groupBoxReceiveData.Name = "groupBoxReceiveData";
            this.groupBoxReceiveData.Size = new System.Drawing.Size(493, 58);
            this.groupBoxReceiveData.TabIndex = 25;
            this.groupBoxReceiveData.TabStop = false;
            this.groupBoxReceiveData.Text = "充值";
            // 
            // buttonClearMoney
            // 
            this.buttonClearMoney.Cursor = System.Windows.Forms.Cursors.Hand;
            this.buttonClearMoney.Enabled = false;
            this.buttonClearMoney.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.buttonClearMoney.Location = new System.Drawing.Point(177, 19);
            this.buttonClearMoney.Name = "buttonClearMoney";
            this.buttonClearMoney.Size = new System.Drawing.Size(73, 30);
            this.buttonClearMoney.TabIndex = 25;
            this.buttonClearMoney.Text = "清零";
            this.buttonClearMoney.UseVisualStyleBackColor = true;
            this.buttonClearMoney.Click += new System.EventHandler(this.buttonClearMoney_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label3.Location = new System.Drawing.Point(124, 25);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(28, 19);
            this.label3.TabIndex = 24;
            this.label3.Text = "元";
            // 
            // buttonAddMoney
            // 
            this.buttonAddMoney.Cursor = System.Windows.Forms.Cursors.Hand;
            this.buttonAddMoney.Enabled = false;
            this.buttonAddMoney.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.buttonAddMoney.Location = new System.Drawing.Point(274, 19);
            this.buttonAddMoney.Name = "buttonAddMoney";
            this.buttonAddMoney.Size = new System.Drawing.Size(73, 30);
            this.buttonAddMoney.TabIndex = 23;
            this.buttonAddMoney.Text = "充值";
            this.buttonAddMoney.UseVisualStyleBackColor = true;
            this.buttonAddMoney.Click += new System.EventHandler(this.buttonAddMoney_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label2.Location = new System.Drawing.Point(12, 25);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(57, 19);
            this.label2.TabIndex = 22;
            this.label2.Text = "充值:";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.labelShowMoney);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Location = new System.Drawing.Point(10, 140);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(166, 134);
            this.groupBox1.TabIndex = 27;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "余额";
            // 
            // labelShowMoney
            // 
            this.labelShowMoney.AutoSize = true;
            this.labelShowMoney.Font = new System.Drawing.Font("楷体", 24F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.labelShowMoney.ForeColor = System.Drawing.Color.Black;
            this.labelShowMoney.Location = new System.Drawing.Point(11, 65);
            this.labelShowMoney.Name = "labelShowMoney";
            this.labelShowMoney.Size = new System.Drawing.Size(150, 33);
            this.labelShowMoney.TabIndex = 5;
            this.labelShowMoney.Text = "000.00元";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label4.Location = new System.Drawing.Point(14, 24);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(95, 19);
            this.label4.TabIndex = 4;
            this.label4.Text = "卡上余额:";
            // 
            // timer
            // 
            this.timer.Interval = 1000;
            this.timer.Tick += new System.EventHandler(this.time_Tick);
            // 
            // button重复充值
            // 
            this.button重复充值.Cursor = System.Windows.Forms.Cursors.Hand;
            this.button重复充值.Enabled = false;
            this.button重复充值.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button重复充值.Location = new System.Drawing.Point(372, 19);
            this.button重复充值.Name = "button重复充值";
            this.button重复充值.Size = new System.Drawing.Size(96, 30);
            this.button重复充值.TabIndex = 26;
            this.button重复充值.Text = "重复充值";
            this.button重复充值.UseVisualStyleBackColor = true;
            this.button重复充值.Click += new System.EventHandler(this.button重复充值_Click);
            // 
            // AppSerialPort
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(513, 345);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.groupBoxTransmitData);
            this.Controls.Add(this.groupBoxReceiveData);
            this.Controls.Add(this.groupBoxCommSetting);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "AppSerialPort";
            this.Text = " 三江电子 充值器";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.AppSerialPort_FormClosed);
            this.groupBoxCommSetting.ResumeLayout(false);
            this.groupBoxCommSetting.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxPowerOn)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxPowerOff)).EndInit();
            this.groupBoxTransmitData.ResumeLayout(false);
            this.groupBoxTransmitData.PerformLayout();
            this.groupBoxReceiveData.ResumeLayout(false);
            this.groupBoxReceiveData.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ComboBox comboPortNum;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBoxCommSetting;
        private System.Windows.Forms.Button buttonSerialPortSwitch;
        private System.Windows.Forms.TextBox textBoxPayMessage;
        private System.Windows.Forms.GroupBox groupBoxTransmitData;
        private System.Windows.Forms.TextBox textBoxMoney;
        private System.Windows.Forms.GroupBox groupBoxReceiveData;
        private System.Windows.Forms.Button buttonAddMoney;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label labelShowMoney;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.SaveFileDialog saveFileDialog;
        private System.Windows.Forms.Timer timer;
        private System.Windows.Forms.Button buttonClearMoney;
        private System.Windows.Forms.PictureBox pictureBoxPowerOn;
        private System.Windows.Forms.PictureBox pictureBoxPowerOff;
        private System.Windows.Forms.Button button重复充值;
    }
}

