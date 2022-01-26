package exercise03;

import java.awt.*;

public class Circle {//定义Circle类绘画对象
    int x;
    int y;
    int r = 30;
    Color color = Color.yellow;

    public Circle() {}
    public Circle(int x, int y) {
        this.x = x;
        this.y = y;
    }

//    public void draw() {
//        Graphics graphics=(Graphics2D) getGraphics();
//        int x = this.x;
//        int y = this.y;
//        int r = 30;
//
//        graphics.setColor(Color.yellow);
//        graphics.fillOval(x/2-r,y/2-r,2*r,2*r);
//    }


    public void setX(int x) {
        this.x = x;
    }

    public int getX() {
        return x;
    }

    public int getY() {
        return y;
    }

    public int getR() {
        return r;
    }
}