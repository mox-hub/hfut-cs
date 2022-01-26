package exercise05;

import java.io.*;
import java.net.*;
import java.util.Scanner;

public class Server {

    public Server() {
        //初始化服务器，及用到的端口和流类
        String fileName = "ServerReceived.txt";
        int port = 8888;
        ServerSocket serverForClient = null;
        Socket socketOnServer;
        DataInputStream in;
        DataOutputStream out;
        FileInputStream fis;
        FileOutputStream fos;

        try {
            //建立服务端的准备
            serverForClient = new ServerSocket(port);
            System.out.println("服务器已经开启......");
        } catch (IOException e) {
            System.out.println(e);
        }
        try {
            String key;
            Scanner scanner = new Scanner(System.in);
            //准备与客户端建立联系
            System.out.println("正在等待用户呼叫......");
            socketOnServer = serverForClient.accept();

            //成功建立联系，并准备传输数据
            System.out.println("用户已连接，用户IP" +
                    socketOnServer.getInetAddress().getHostAddress());
            out = new DataOutputStream(socketOnServer.getOutputStream());
            in = new DataInputStream(socketOnServer.getInputStream());

            //判断是否发送文件
            System.out.println("是否要发送文件？（yes/no）");
            key = scanner.nextLine();
            if (key.equals("yes")){
                System.out.println("请输入文件名：");
                fileName = scanner.nextLine();

                //开始进行输入
                int messageOut;
                fis = new FileInputStream(fileName);
                out = new DataOutputStream(socketOnServer.getOutputStream());
                while ((messageOut = fis.read()) != -1) {
                    out.writeInt(messageOut);
                }
                out.writeInt(messageOut);
                System.out.println("文件发送成功！");
            }
            else {
                System.out.println("那么请等待客户端的文件......");
                //开始读取数据并写入文件中
                int messageIn;
                fos = new FileOutputStream(fileName);
                messageIn = in.readInt();
                while (messageIn != -1) {
                    fos.write(messageIn);
                    messageIn = in.readInt();
                }
                System.out.println("文件接受成功！");
            }

        } catch (FileNotFoundException e) {
            System.out.println(e);
        } catch (IOException e) {
            System.out.println("Error:" + e);
        }
    }

    public static void main(String[] args) {
        //建立服务器
        Server server = new Server();
    }
}
