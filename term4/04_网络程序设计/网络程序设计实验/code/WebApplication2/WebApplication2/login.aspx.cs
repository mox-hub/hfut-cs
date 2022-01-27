using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace WebApplication1
{
    public partial class login : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {

        }

        protected void btnLogin_Click(object sender, EventArgs e)
        {
            string name = txtUserName.Text;
            string pwd = txtPwd.Text;

            if ("张三" == name && "1234" == pwd)
            {
                Session["UserID"] = name;
                Session["Pwd"] = pwd;
                Response.Redirect("myhome.aspx");
            }
            else
            {
                Label3.Text = "请输入正确的用户名与密码！";
            }

        }
        protected void btnReset_Click(object sender, EventArgs e)
        {

            txtUserName.Text = "";
            txtPwd.Text = "";
        }
    }
}