package exercise05;

import java.awt.desktop.SystemEventListener;
import java.io.*;
import java.net.*;
import java.util.Scanner;

public class Client {
    //定义static变量由用户输入地址和端口
    static String address;
    static int port;

    public Client() {
        //建立一些Client的流类
        String fileName;
        Socket socket = null;
        DataInputStream in = null;
        DataOutputStream out = null;
        FileInputStream fis = null;
        FileOutputStream fos = null;

        try {
            String key;
            Scanner scanner = new Scanner(System.in);

            //建立socket连接
            socket = new Socket(InetAddress.getByName(address),port);
            System.out.println("已建立连接......");

            //判断是否发送文件
            System.out.println("是否要发送文件？（yes/no）");
            key = scanner.nextLine();
            if (key.equals("yes")){
                System.out.println("请输入文件名：");
                fileName = scanner.nextLine();

                //开始进行输入
                int messageOut;
                fis = new FileInputStream(fileName);
                out = new DataOutputStream(socket.getOutputStream());
                while ((messageOut = fis.read()) != -1) {
                    out.writeInt(messageOut);
                }
                out.writeInt(messageOut);
                System.out.println("文件发送成功！");
            }
            else {
                System.out.println("那么请等待服务端的文件......");
                in = new DataInputStream(socket.getInputStream());

                //开始读取数据并写入文件中
                int messageIn;
                fos = new FileOutputStream("ClientReceived.txt");
                messageIn = in.readInt();
                while (messageIn != -1) {
                    fos.write(messageIn);
                    messageIn = in.readInt();
                }
                System.out.println("文件接收成功！");
            }
        } catch (FileNotFoundException e) {
            System.out.println(e);
        } catch (IOException e) {
            System.out.println(e);
        }
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        //进行地址和端口的输入
        System.out.println("请输入服务器地址：");
        address = scanner.nextLine();
        System.out.println("请输入端口号：");
        port = scanner.nextInt();

        //建立客户端连接
        Client client = new Client();
    }
}
