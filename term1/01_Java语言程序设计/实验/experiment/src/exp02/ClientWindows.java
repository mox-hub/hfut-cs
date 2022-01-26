package exp02;

import javax.swing.border.Border;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;

public class ClientWindows extends JFrame {
    //  定义变量、流类、组件
    DataInputStream inputFromServer;    //输入流
    DataOutputStream outputToServer;    //输出流
    JTextField portField;   //服务器端口输入框
    JTextField ipField;     //服务器地址输入框
    JTextField sayField;    //发送输入框
    JTextArea dialog;       //多行对话框
    JButton connectButton;  //连接按钮
    JButton sayButton;      //发送按钮
    ClientNet clientNet;    //网络组件
    int port = 8888;        //初始化端口号
    String ip = "127.0.0.1";//初始化ip地址

    //  重写窗口构造方法
    public ClientWindows() {
        init();     //初始化
        setVisible(true);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setBounds(800, 200, 520, 340);
        setTitle("Socket通讯客户端");
        setResizable(false);
    }

    //  初始化窗口界面
    void init() {
        setLayout(new FlowLayout(FlowLayout.LEADING, 5, 15));
        /**
         * @param: hgap 是控制flowLayout的列（之间的）间距
         * @param: vgap 是控制flowLayout的行（之间的）间距
         * @param： CENTER 格式中心靠拢
         */

        //设置两个面板
        JPanel settings = new JPanel();
        JPanel saying = new JPanel();
        Border border = BorderFactory.createTitledBorder("客户机设置：");       //新建边框
        settings.setBorder(border);     //将新建的边框设置为panel的边框

        //设置setting面版的控件
        settings.add(new JLabel("Server IP:"));
        ipField = new JTextField("127.0.0.1", 10);
        settings.add(ipField);
        // 设置port
        settings.add(new JLabel("Server Port:"));
        portField = new JTextField("8888", 10);
        settings.add(portField);
        //设置connect按钮
        connectButton = new JButton("Connect");
        settings.add(connectButton);

        //设置滚动面板对话框的控件
        dialog = new JTextArea(8, 45);
        JScrollPane scrollPane = new JScrollPane(dialog);


        //设置聊天框的控件
        saying.add(new JLabel("say:"));
        sayField = new JTextField(35);
        sayButton = new JButton("say");
        saying.add(sayField);
        saying.add(sayButton);
//        添加各个窗口组件
        this.add(settings, BorderLayout.NORTH);
        this.add(scrollPane, BorderLayout.CENTER);
        this.add(saying, BorderLayout.SOUTH);

    }

    //  为窗口设置监听器
    public void setListener() {
//        为连接按钮设置监听器
        connectButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                port = Integer.parseInt(portField.getText());
                ip = ipField.getText();
                System.out.println(port);
                System.out.println(ip);
                clientNet = new ClientNet();
                clientNet.start();
            }
        });
//        为发送按钮添加监听器
        sayButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    //获取输入信息
                    String message = sayField.getText().trim();

                    //发送信息
                    outputToServer.writeUTF(message);
                    outputToServer.flush();

                    //在文本区显示
                    dialog.append("Client(Me): " + message + "\n");
                    sayField.setText(null);
                } catch (IOException ex) {
                    System.err.println(ex);
                }
            }
        });
    }

    //  单独的网络模块
    public class ClientNet extends Thread {
        Socket socket;

        //        重写run函数
        @Override
        public void run() {
            super.run();
            try {
                // 建立客户端
                socket = new Socket(ip, port);
                dialog.append("Connect to Server......");

                // 设置输入输出流
                inputFromServer = new DataInputStream(socket.getInputStream());
                outputToServer = new DataOutputStream(socket.getOutputStream());

                // 循环接收来自Server的信息
                while (true) {
                    String message = inputFromServer.readUTF();
                    dialog.append(" Server: " + message + '\n');
                }
            } catch (IOException ex) {
                dialog.append("...已断开连接..." + '\n');
            }
        }
    }

    //  main函数
    public static void main(String[] args) {
        //TODO test here
        ClientWindows client = new ClientWindows();
        client.setListener();
    }
}
