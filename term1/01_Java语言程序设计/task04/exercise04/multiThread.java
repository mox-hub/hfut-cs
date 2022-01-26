package exercise04;

import java.util.concurrent.atomic.AtomicInteger;

public class multiThread {
    static AtomicInteger atom = new AtomicInteger(0);//设置原子变量，防止线程意外终止

    public static void main(String[] args) {//main函数开始测试
        long sum1,sum2;//储存两种总和
        long plus;//最后乘积的结果
//        创建两个Thread
        sumOfPrimeNumber sumOfPrime = new sumOfPrimeNumber();
        sumOfPerfectNumber sumOfPerfect = new sumOfPerfectNumber();
//        实现run方法，线程运行
        sumOfPerfect.start();
        sumOfPrime.start();
        try {
            sumOfPrime.join();
            sumOfPerfect.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
//
        sum1 = sumOfPrime.getSum();
        sum2 = sumOfPerfect.getSum();
        plus = sum1 * sum2;
        System.out.println("结果为：" + plus);
    }

    static class sumOfPrimeNumber extends Thread {
        int sum = 0;
        //    重写run方法
        @Override
        public void run() {
            for (int i = 0; i < 10000;i++) {
                if (isPrime(i)){//isPerfect返回值是布尔类型
                    sum += i;
                }
            }
            atom.getAndIncrement();//原子计数器
        }
        //判断是否是素数
        public boolean isPrime(final int number) {//final用来固定number
            if (number <= 1)
                return false;
            for (int i = 2; i <= Math.sqrt(number); i++) {
                if (number % i == 0)
                    return false;
            }
            return true;
        }
        //获得sum的方法
        public int getSum() {
            return sum;
        }
    }

    static class sumOfPerfectNumber extends Thread{
        int sum = 0;
        //    重写run方法
        @Override
        public void run() {
            for (int i = 0;i < 10000; i++) {
                if (isPerfect(i)) {//isPerfect返回值是布尔类型
                    sum += i;
                }
            }
            atom.getAndIncrement();
        }
        //判断是否是完全数
        public boolean isPerfect(final int number) {//final int 不可修改
            int temp = 0;
            for (int i = 1;i < number;i ++) {
                if((number % i) == 0) {
                    temp += i;
                }
            }
            if (temp == number)
                return true;
            else
                return false;
        }
        //获得sum的方法
        public int getSum() {
            return sum;
        }
    }
}
