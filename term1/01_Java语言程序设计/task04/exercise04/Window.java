package exercise04;
//（1）设计图形化界面，至少包括文本类控件类。接收从键盘输入姓名、学号、成绩，并保存到文本文件中，重复进行。
//（2）从文件中读取各学生的成绩，并计算所有学生成绩的平均值、最大值和最小值，排序后输出到另一文本文件。
import jdk.jfr.Label;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import java.util.List;
import javax.swing.*;

public class Window extends JFrame {//设置窗口界面
    //初始化组件
    JTextField name;
    JTextField studentID;
    JTextField score1;
    JTextField score2;
    JTextField score3;
    JButton check;

    public Window() {//重写构造方法
        init();//初始化
        setVisible(true);//设置窗口为可见
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);//设置正常退出
        setBounds(300,300,250,300);//设置窗口大小位置
        setTitle("成绩输入端口");//设置标题
    }

    void init() {//初始化设置
        setVisible(true);
        /**
         * @param: hgap 是控制flowLayout的列（之间的）间距
         * @param: vgap 是控制flowLayout的行（之间的）间距
         * @param： CENTER 格式中心靠拢
         */
        setLayout(new FlowLayout(FlowLayout.CENTER,5,20));//采用FlowLayout排版

        //添加姓名输入框
        add(new JLabel("姓名："));
        name = new JTextField(15);
        add(name);
        //添加学号输入框
        add(new JLabel("学号："));
        studentID = new JTextField(15);
        add(studentID);
        //添加成绩输入框
        add(new JLabel("成绩1："));
        score1 = new JTextField(15);
        add(score1);
        add(new JLabel("成绩2："));
        score2 = new JTextField(15);
        add(score2);
        add(new JLabel("成绩3："));
        score3 = new JTextField(15);
        add(score3);
        //添加确认按钮
        add(new JLabel("请确认："));
        check = new JButton("确定");
        check.addMouseListener(new MouseAdapter() {//为确认按钮注册事件监听器
            @Override
            public void mouseClicked(MouseEvent e){//重写鼠标点击方法
                super.mouseClicked(e);
                //写入到read.txt文件中
                try {
                    System.out.println("read is ok");
                    BufferedWriter bw = new BufferedWriter(new FileWriter(".\\read.txt",true));
                    bw.write(name.getText() + " ");
                    bw.write(studentID.getText() + " ");
                    bw.write(score1.getText() + " ");
                    bw.write(score2.getText() + " ");
                    bw.write(score3.getText() + " \n");
                    //关闭bufferReader
                    bw.close();
                } catch (IOException ex) {
                    ex.printStackTrace();
                }
                //刷新文本框
                name.setText("");
                studentID.setText("");
                score1.setText("");
                score2.setText("");
                score3.setText("");
            }
        });
        add(check);
    }


}
class myFile{

    public myFile(){//重写构造方法
        try {
            //一些初始化操作
            FileReader fr = new FileReader(".\\read.txt");
            BufferedReader br = new BufferedReader(fr);
            FileWriter fw = new FileWriter(".\\write.txt");
            BufferedWriter bw = new BufferedWriter(fw);
            List<Student> list = new ArrayList<>();
            String line;

            Student student;
            while ((line = br.readLine()) != null) {
                String[] studentSplit = line.trim().split("\\s+");
                student = new Student();
                //将切分好的内容输入到student中
                student.name = studentSplit[0];
                student.studentID = Long.parseLong(studentSplit[1]);
                student.score1 = Integer.parseInt(studentSplit[2]);
                student.score2 = Integer.parseInt(studentSplit[3]);
                student.score3 = Integer.parseInt(studentSplit[4]);
                student.sort();//内部排序
                list.add(student);
            }
            //对数组进行排序
            sortFile(list);
            //写入新文件

            for (int i = 0;i < list.size();i++){
                System.out.println(i);
                student = new Student();
                student = list.get(i);
                bw.write(student.name + " ");
                bw.write(student.studentID + " ");
                bw.write(student.score1 + " ");
                bw.write(student.score2 + " ");
                bw.write(student.score3 + "\n");
                System.out.println("write is ok");
            }
            //关闭
            bw.close();
        } catch (IOException e){
            e.printStackTrace();
        }
    }

    public void sortFile(List<Student> stuList) {
        //排序集合
        Collections.sort(stuList, (a, b) -> {
            int comp = String.valueOf(a.avg).compareTo(String.valueOf(b.avg));
            return comp;
        });
    }
}

class Student {//student类，用于储存和排序学生的信息
    String name;//姓名
    long studentID;//ID
    int score1;//成绩1
    int score2;//成绩2
    int score3;//成绩3
    float avg;
    public void sort() {//用于内部排序
        if (score1 < score2)
            swap(score1,score2);
        if (score1 < score3)
            swap(score1,score3);
        if (score2 < score3)
            swap(score2,score3);
        //顺便求avg
        avg = (score1 + score2 + score3) / 3.0f;
    }
    void swap(int a,int b) {//交换位置
        int temp;
        temp = a;
        a = b;
        b = temp;
    }

}

class text {//测试类
    public static void main(String[] args) {
        Window window = new Window();
        myFile file = new myFile();
    }
}
