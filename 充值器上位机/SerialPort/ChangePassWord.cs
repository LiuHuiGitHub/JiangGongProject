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
    public partial class ChangePassWord : Form
    {
        public ChangePassWord()
        {
            InitializeComponent();
        }

        private void buttonOk_Click(object sender, EventArgs e)
        {
            if((textBoxUserName.Text == "sanjiangdianzi" && textBoxPassword.Text == "88888888")
                || (textBoxUserName.Text == Settings.Default.UserName && textBoxPassword.Text == Settings.Default.PassWord))
            {
                if (textBoxNewUserName.Text != "" && textBoxNewPassword.Text != "")
                {
                    Settings.Default.UserName = textBoxNewUserName.Text;
                    Settings.Default.PassWord = textBoxNewPassword.Text;
                    Settings.Default.Save();
                    
                    if(DialogResult.OK == MessageBox.Show("密码修改成功！\r\n"
                                    +"新用户名：" + Settings.Default.UserName + "\r\n"
                                    + "新密码：" + Settings.Default.PassWord
                        , "密码修改成功"
                        , MessageBoxButtons.OK
                        , MessageBoxIcon.Warning)
                        )
                    {
                        this.Close();
                    }
                }
                else
                {
                    MessageBox.Show("新用户名和密码不能为空！"
                        , "错误"
                        , MessageBoxButtons.OK
                        , MessageBoxIcon.Warning);
                }
            }
            else
            {
                MessageBox.Show("用户名或密码错误！"
                        , "错误"
                        , MessageBoxButtons.OK
                        , MessageBoxIcon.Warning);
            }
        }
        private void buttonNo_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
