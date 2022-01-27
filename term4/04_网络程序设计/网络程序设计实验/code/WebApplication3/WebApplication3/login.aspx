<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="login.aspx.cs" Inherits="WebApplication3.login" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>登录页</title>
</head>
<body>
    <form runat="server">
        <div>
            <asp:Label ID="Label1" runat="server" Text="用户名"></asp:Label>
            <asp:textbox ID="txtUserName" runat="server"></asp:textbox>
        </div>
        <div>
            <asp:Label ID="Label2" runat="server" Text="密码"></asp:Label>
            <asp:textbox ID="txtPwd" runat="server"></asp:textbox>
        </div>

        <asp:Label ID="Label3" runat="server" Text="请输入用户名密码"></asp:Label>
        
        <div>
            <asp:button ID="btnLogin" runat="server" text="登录" onClick="btnLogin_Click" />
            <asp:button ID="btnReset" runat="server" text="重写" onClick="btnReset_Click" />
        </div>

    </form>
</body>

</html>
