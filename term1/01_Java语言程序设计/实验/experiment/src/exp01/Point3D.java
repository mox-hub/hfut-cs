package exp01;

import com.sun.org.apache.xerces.internal.xinclude.XPointerElementHandler;

public class Point3D extends Point2D {
    //    添加的变量z
    int z;

    //    第一种构造函数
    Point3D(int x, int y, int z) {
        super(x, y);
        this.z = z;
    }

    //    第二种构造函数
    Point3D(Point2D p, int z) {
        super(p.x, p.y);
        this.z = z;
    }

    public void offset(int a, int b, int c) {
        super.x += a;
        super.y += b;
        this.z += c;
    }

    public static void main(String[] args) {
//        实例化两个Point2D的对象
        Point2D p2d1 = new Point2D(1, 1);
        Point2D p2d2 = new Point2D(4, 5);
//        计算距离并输出
        double distance2D =
                Math.sqrt((p2d1.x - p2d2.x) * (p2d1.x - p2d2.x) + (p2d1.y - p2d2.y) * (p2d1.y - p2d2.y));
        System.out.println("p2d1和p2d2之间的距离为： " + distance2D);
//        实例化两个Point3D的对象
        Point3D p3d1 = new Point3D(0, 0, 0);
        Point3D p3d2 = new Point3D(1, 2, 2);
//        计算距离并输出
        double distance3D =
                Math.sqrt((p3d1.x - p3d2.x) * (p3d1.x - p3d2.x) + (p3d1.y - p3d2.y) * (p3d1.y - p3d2.y) + (p3d1.z - p3d2.z) * (p3d1.z - p3d2.z));
        System.out.println("p3d1和p3d2之间的距离为： " + distance3D);

    }
}
