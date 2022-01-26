package exercise03;

import javax.swing.*;

public class DrawFrame extends JFrame{//定义绘图窗口
    DrawPanel drawPanel = new DrawPanel();  //创建面板类的实例化对象

    public static void main(String[] args) {//实现测试的main函数
        DrawFrame window = new DrawFrame();
        window.setVisible(true);
    }

    public DrawFrame() {//重写构造方法
        super();        //引用超类的构造方法
        setTitle("绘图");
        setBounds(100,100,800,600);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        add(drawPanel);         //添加drawPanel组件
    }
}
