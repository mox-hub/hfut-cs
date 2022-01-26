package exercise03;

import java.awt.*;

public class Square {//定义Square类绘画对象
    int x;
    int y;
    int width = 40;
    int height = 60;
    Color color = Color.red;

    public Square(int x, int y) {
        this.x = x;
        this.y = y;
    }

//    public void draw(Graphics2D graphics) {
//        int x = this.x;
//        int y = this.y;
//        int width = this.width;
//        int height = this.height;
//
//        graphics.setColor(Color.red);
//        graphics.fillRect(x-width/2,y-height/2,width,height);
//    }

    public int getX() {
        return x;
    }

    public int getY() {
        return y;
    }

    public int getWidth() {
        return width;
    }

    public int getHeight() {
        return height;
    }
}

