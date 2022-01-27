<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="myhome.aspx.cs" Inherits="WebApplication1.myhome" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>我的空间</title>
</head>
<body>
    <form id="form1" runat="server">
        <div>
            <asp:Label ID="Label1" runat="server" Text="欢迎来到您的空间！"></asp:Label>
        </div>
        <div>
            <asp:Label ID="Label2" runat="server" Text=""></asp:Label>
        </div>
        <div>
            <asp:Label ID="Label3" runat="server" Text=""></asp:Label>
        </div>
        <div>
            <asp:Button ID="Exit" runat="server" Text="退出" onClick="Exit_Click" />
        </div>
        
    </form>
</body>
</html>

