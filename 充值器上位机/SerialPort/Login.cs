using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace UserSerialPort
{
    public partial class Login : Form
    {
        private Button buttonLogin;
        private Label label1;
        private Label label2;
        private TextBox textBoxUserName;
        private Button buttonChangedPassWord;
        private PictureBox pictureBox1;
        private TextBox textBoxPassword;
    
        public Login()
        {
            InitializeComponent();
        }

        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Login));
            this.textBoxPassword = new System.Windows.Forms.TextBox();
            this.buttonLogin = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxUserName = new System.Windows.Forms.TextBox();
            this.buttonChangedPassWord = new System.Windows.Forms.Button();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // textBoxPassword
            // 
            this.textBoxPassword.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.textBoxPassword.ImeMode = System.Windows.Forms.ImeMode.Disable;
            this.textBoxPassword.Location = new System.Drawing.Point(182, 78);
            this.textBoxPassword.Name = "textBoxPassword";
            this.textBoxPassword.Size = new System.Drawing.Size(100, 29);
            this.textBoxPassword.TabIndex = 1;
            // 
            // buttonLogin
            // 
            this.buttonLogin.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.buttonLogin.Location = new System.Drawing.Point(182, 135);
            this.buttonLogin.Name = "buttonLogin";
            this.buttonLogin.Size = new System.Drawing.Size(100, 29);
            this.buttonLogin.TabIndex = 2;
            this.buttonLogin.Text = "登陆";
            this.buttonLogin.UseVisualStyleBackColor = true;
            this.buttonLogin.Click += new System.EventHandler(this.buttonLogin_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(123, 82);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(57, 19);
            this.label1.TabIndex = 2;
            this.label1.Text = "密码:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label2.Location = new System.Drawing.Point(123, 43);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(57, 19);
            this.label2.TabIndex = 4;
            this.label2.Text = "用户:";
            // 
            // textBoxUserName
            // 
            this.textBoxUserName.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.textBoxUserName.ImeMode = System.Windows.Forms.ImeMode.Disable;
            this.textBoxUserName.Location = new System.Drawing.Point(182, 39);
            this.textBoxUserName.Name = "textBoxUserName";
            this.textBoxUserName.Size = new System.Drawing.Size(100, 29);
            this.textBoxUserName.TabIndex = 0;
            // 
            // buttonChangedPassWord
            // 
            this.buttonChangedPassWord.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.buttonChangedPassWord.Location = new System.Drawing.Point(53, 135);
            this.buttonChangedPassWord.Name = "buttonChangedPassWord";
            this.buttonChangedPassWord.Size = new System.Drawing.Size(100, 29);
            this.buttonChangedPassWord.TabIndex = 3;
            this.buttonChangedPassWord.Text = "更改密码";
            this.buttonChangedPassWord.UseVisualStyleBackColor = true;
            this.buttonChangedPassWord.Click += new System.EventHandler(this.buttonChangedPassWord_Click);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox1.Image")));
            this.pictureBox1.Location = new System.Drawing.Point(40, 36);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(75, 74);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 6;
            this.pictureBox1.TabStop = false;
            // 
            // Login
            // 
            this.ClientSize = new System.Drawing.Size(332, 196);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.buttonChangedPassWord);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.textBoxUserName);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.buttonLogin);
            this.Controls.Add(this.textBoxPassword);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Login";
            this.Text = "登陆";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        private void buttonLogin_Click(object sender, EventArgs e)
        {
            if (textBoxUserName.Text == Settings.Default.UserName
                && textBoxPassword.Text == Settings.Default.PassWord
                )
            {
                this.Hide();
                AppSerialPort app = new AppSerialPort();
                app.Show();
            }
            else
            {
                MessageBox.Show("用户名或密码错误！"
                        , "错误"
                        , MessageBoxButtons.OK
                        , MessageBoxIcon.Warning);
            }
        }

        private void buttonChangedPassWord_Click(object sender, EventArgs e)
        {
            ChangePassWord form = new ChangePassWord();
            form.ShowDialog();
        }
    }
}
