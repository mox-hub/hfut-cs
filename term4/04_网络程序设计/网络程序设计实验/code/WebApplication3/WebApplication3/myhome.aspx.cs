using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace WebApplication3
{
    public partial class myhome : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (Session["USERID"] != null && Session["PASSWORD"] != null)
            {
                Label2.Text = "用户名+密码为：" + Session["USERID"].ToString() + Session["PASSWORD"].ToString();
                Label3.Text = DateTime.Now.ToLocalTime().ToString();
                ListBMyClass.Items.Clear();
                ListBMyClass.Items.Add("我所选修的课程");
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

        protected void btnSearchMyClass_Click(object sender, EventArgs e)
        {
            SqlConnection conn;
            SqlCommand cmd;
            SqlDataReader rd;
            string conStr = "Integrated Security=SSPI;Persist Security Info=False;Initial Catalog=NetSchool;Data Source=.";
            conn = new SqlConnection(conStr);

            String cmd_test = "SELECT * FROM STUDENT_CLASS where USERID = " + Session["USERID"].ToString();

            cmd = new SqlCommand(cmd_test, conn);
            conn.Open();

            rd = cmd.ExecuteReader();
            while (rd.Read())
            {
                ListBMyClass.Items.Add(new ListItem(rd["CLASSID"].ToString(), rd["CLASSID"].ToString()));
            }

            if (rd.HasRows == false)
            {
                ListBMyClass.Items.Add("你没有选修任何一门课程");
            }

            rd.Close();
            conn.Close();
        }
    }
}