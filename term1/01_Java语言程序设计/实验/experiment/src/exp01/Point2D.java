package exp01;

public class Point2D {
    //    成员属性
    int x;
    int y;

    //    构造方法
    Point2D(int x, int y) {
        this.x = x;
        this.y = y;
    }

    //    实现Point2D的平移
    public void offset(int a, int b) {
        x += a;
        y += b;
    }

}
