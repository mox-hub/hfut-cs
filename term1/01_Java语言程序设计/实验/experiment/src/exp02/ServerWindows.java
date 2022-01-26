package exp02;

import javax.swing.border.Border;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class ServerWindows extends JFrame {
    //  定义变量、流类、组件
    DataInputStream inputFromClient;    //输入流
    DataOutputStream outputToClient;    //输出流
    JTextField portField;   //服务器端口输入框
    JButton startButton;    //开始服务按钮
    JTextArea dialog;       //多行对话框
    JTextField sayField;    //发送输入框
    JButton sayButton;      //发送按钮
    ServerNet serverNet;    //服务器网络组件
    int port = 8888;        //初始化端口号

    //  重写构造方法
    public ServerWindows() {
        init();     //初始化
        setVisible(true);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setBounds(200, 200, 520, 340);
        setTitle("Socket通讯服务器");
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
        Border border = BorderFactory.createTitledBorder("服务器设置：");       //新建边框
        settings.setBorder(border);     //将新建的边框设置为panel的边框

        //设置setting面版的控件
        settings.add(new JLabel("Port: "));
        portField = new JTextField(33);
        startButton = new JButton("Start");
        settings.add(portField);
        settings.add(startButton);

        //设置滚动面板对话框的控件
        dialog = new JTextArea(8, 45);
        JScrollPane scrollPane = new JScrollPane(dialog);


        //设置聊天框的控件
        saying.add(new JLabel("say:"));
        sayField = new JTextField(35);
        sayButton = new JButton("say");
        saying.add(sayField);
        saying.add(sayButton);

        this.add(settings, BorderLayout.NORTH);
        this.add(scrollPane, BorderLayout.CENTER);
        this.add(saying, BorderLayout.SOUTH);

    }

    //  为窗口设置监听器
    public void setListener() {
        //  start 的监听器
        startButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                port = Integer.parseInt(portField.getText());
                serverNet = new ServerNet();
                serverNet.start();
            }
        });
        // say 的监听器
        sayButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    //获取输入信息
                    String message = sayField.getText().trim();

                    //发送信息
                    outputToClient.writeUTF(message);
                    outputToClient.flush();

                    //在文本区显示
                    dialog.append(" Server(Me): " + message + "\n");
                    sayField.setText(null);
                } catch (IOException ex) {
                    System.err.println(ex);
                }
            }
        });
    }

    //  单独的网络模块
    public class ServerNet extends Thread {
        ServerSocket serverSocket;

        //重写run方法
        @Override
        public void run() {
            super.run();
            try {
                // 建立服务器，端口为port
                serverSocket = new ServerSocket(port);
                dialog.append("Server starting......" + '\n');
                //  等待被连接
                Socket socket = serverSocket.accept();
                //成功建立联系，并准备传输数据
                dialog.append("Client Connected......");
                System.out.println("用户已连接，用户IP" +
                        serverSocket.getInetAddress().getHostAddress());

                // 设置输入输出流
                inputFromClient = new DataInputStream(socket.getInputStream());
                outputToClient = new DataOutputStream(socket.getOutputStream());

                // 循环接收来自client的信息
                while (true) {
                    String message = inputFromClient.readUTF();
                    dialog.append(" Client: " + message + '\n');
                }
            } catch (IOException ex) {
                dialog.append(ex.getMessage() + '\n');
                return;
            }
        }
    }

    //  main函数
    public static void main(String[] args) {
        //TODO test here
        ServerWindows server = new ServerWindows();
        server.setListener();
    }

}

