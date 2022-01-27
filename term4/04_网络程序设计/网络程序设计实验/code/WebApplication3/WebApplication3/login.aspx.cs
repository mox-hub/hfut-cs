using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data.SqlClient;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace WebApplication3
{
    public partial class login : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {

        }

        protected void btnLogin_Click(object sender, EventArgs e)
        {
            string userID = txtUserName.Text;
            string userPwd = txtPwd.Text;
            SqlConnection conn;
            SqlCommand cmd;
            SqlDataReader rd;

            string conStr = "Integrated Security=SSPI;Persist Security Info=False;Initial Catalog=NetSchool;Data Source=.";
            conn = new SqlConnection(conStr);
            // conn = new SqlConnection(@"Data Source=jf508-029;Initial Catalog=Netschool;Integrated Security=True");
            
            conn.Open();
            string sun;
            sun = "select * from  [NetSchool].[dbo].[STUDENT]where USERID='" + txtUserName.Text + "'and PASSWORD='" + txtPwd.Text + "'";
            cmd = new SqlCommand(sun, conn);
            rd = cmd.ExecuteReader();

            if (rd.HasRows)
            {
                Session["USERID"] = userID;
                Session["PASSWORD"] = userPwd;
                Response.Redirect("myhome.aspx");

            }
            else
            {
                Label3.Text = "请输入正确的用户名与密码！";
            }

            rd.Close();
            conn.Close();

        }
        protected void btnReset_Click(object sender, EventArgs e)
        {

            txtUserName.Text = "";
            txtPwd.Text = "";
        }
    }
}