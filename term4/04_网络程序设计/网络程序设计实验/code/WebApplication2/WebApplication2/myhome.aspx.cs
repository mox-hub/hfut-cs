using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace WebApplication1
{
    public partial class myhome : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (Session["UserID"] != null && Session["Pwd"] != null)
            {
                Label2.Text = "用户名+密码为：" + Session["UserID"].ToString() + Session["Pwd"].ToString();
                Label3.Text = DateTime.Now.ToLocalTime().ToString();
            }
            else
            {
                Response.Redirect("login.aspx");
            }
        }

        protected void Exit_Click(object sender, EventArgs e)
        {
            Session.RemoveAll();
            Response.Redirect("login.aspx");
        }
    }
}