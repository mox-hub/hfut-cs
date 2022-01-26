import java.util.Scanner;

//这是一个测试类
public class Test {
    private static Person[] a = new Person[20];     //初始化一个对象数组，并以Person为数组类型
                                                    //方便实例化子类对象
    public static void main(String[] args){
        int R;      //switch判断变量，是0，1，2随机整数
        int i;      //作用域main的循环变量

        for (i = 0; i < 20; i++){       //循环产生不同类型的子类对象
            R = (int)(Math.random() * 3);       //调用Math方法

            switch (R){         //实例化对象
                case 0: a[i] = new Student(); break;
                case 1: a[i] = new Faculty(); break;
                case 2: a[i] = new Staff(); break;
            }
        }

        for (i = 0; i < 20; i++){       //循环输入姓名和ID
            Scanner reader = new Scanner(System.in);       //设置reader阅读者
            System.out.printf("请输入第%d个人的姓名:",i+1);
            a[i].setName(reader.next());
            //调用方法
            System.out.printf("请输入第%d个人的ID:",i+1);
            a[i].setID(reader.next());
        }

        for (i = 0;i < 20;i++) {
            System.out.println(a[i].toString());           //调用toString()输出
        }
    }
}
