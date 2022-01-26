package exercise03;

import java.awt.*;
import javax.swing.*;

public class FourPoint extends JFrame{

    MyPanel myPanel = new MyPanel();

    public static void main(String[] args) {
        FourPoint fourPoint = new FourPoint();
        fourPoint.getContentPane().setBackground(Color.PINK);
        fourPoint.setVisible(true);
    }

    public FourPoint() {//重写构造方法
        super();        //引用超类的构造方法
        myPanel.setBackground(null);
        setTitle("四点绘图");
        setBounds(200,200,300,300);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        add(myPanel);         //添加drawPanel组件
    }

    class MyPanel extends JPanel {
//       重写paint方法进行绘图
        public void paint(Graphics g) {
            Graphics2D g2 = (Graphics2D)g;
            g2.setColor(Color.red);

            g2.fillOval(60,60,60,60);
            g2.fillOval(160,160,60,60);
            g2.fillOval(60,160,60,60);
            g2.fillOval(160,60,60,60);
        }
    }


}

