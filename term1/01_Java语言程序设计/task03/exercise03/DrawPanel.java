package exercise03;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import java.util.Vector;

public class DrawPanel extends JPanel {     //新建一个JPanel类的画板
    public int x;
    public int y;
    int number;
    public Graphics graphics;
        //公共的变量，用于储存鼠标点击结果

    Vector<Circle>  circleVector = new Vector<Circle>();
    Vector<Square>  squareVector = new Vector<Square>();

    public void reDraw() {
        int i;
        for (i = 0;i < squareVector.size(); i++) {
            graphics.clearRect(squareVector.elementAt(i).getX(),squareVector.elementAt(i).getY(),
                    squareVector.elementAt(i).getWidth(),squareVector.elementAt(i).getHeight());
        }
        for (i = 0;i < circleVector.size(); i++) {
            graphics.clearRect(circleVector.elementAt(i).getX(),circleVector.elementAt(i).getY(),
                    circleVector.elementAt(i).getR(),circleVector.elementAt(i).getR());
        }
    }

    public void clear(Graphics graphics) {
        graphics.clearRect(0,0,1920,1080);
    }


    public void draw(Circle circle) {

        int x = circle.x;
        int y = circle.y;
        int r = circle.r;
        Graphics graphics=(Graphics2D) getGraphics();
        graphics.setColor(Color.yellow);
        graphics.fillOval(x-r,y-r,2*r,2*r);
    }

    public void draw(Square square) {
        Graphics graphics=(Graphics2D) getGraphics();
        int x = square.x;
        int y = square.y;
        int width = 40;
        int height = 80;
        graphics.setColor(Color.red);
        graphics.drawRect(x-width/2,y-height/2,width,height);
    }

    public DrawPanel() {
        super();
        JFrame frame = new JFrame();

        addMouseListener(new  MouseListener(){//注册鼠标监听器
            @Override
            public void mouseClicked(MouseEvent e) {//主要代码在鼠标点击事件里

                if (e.getClickCount() == 2){//双击
                    System.out.println("double click" );//测试代码
                    clear(DrawPanel.super.getGraphics());
                }

                else if(e.getButton() == e.BUTTON1) {//左键
                    System.out.println("left");//测试代码

                    x = e.getX();       //获取鼠标现在的位置
                    y = e.getY();

//                  新建circle对象并加入数组
                    Circle circle = new Circle(x,y);
                    circleVector.add(circle);
                    draw(circle);
                }

                else if(e.getButton() == e.BUTTON3) {//右键
                    System.out.println("right");//测试代码
                    x = e.getX();       //获取鼠标现在的位置
                    y = e.getY();

//                  新建square对象并加入数组
                    Square square = new Square(x,y);
                    squareVector.add(square);
                    draw(square);
//                    graphics.clearRect(0,0,100,100);
                }
            }
//          未使用的鼠标方法
            @Override
            public void mousePressed(MouseEvent e) {

            }

            @Override
            public void mouseReleased(MouseEvent e) {

            }

            @Override
            public void mouseEntered(MouseEvent e) {

            }

            @Override
            public void mouseExited(MouseEvent e) {

            }
        });
        //the end of MouseListener
    }
}
