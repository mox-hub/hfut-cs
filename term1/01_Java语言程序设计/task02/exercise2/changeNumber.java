import java.util.Scanner;

public class changeNumber {
//    整数字符串转换为整数的函数change
    static int change(String s) {
        int result;
//       调用Integer的方法parseInt使字符串转化为十进制整数
        result = Integer.parseInt(s);
        return result;
    }

    public static void main(String[] args){
        try {
            String str;     //字符串
            int result;     //结果返回值
            Scanner reader = new Scanner(System.in);       //设置阅读器
            System.out.print("请输入整数字符串：");
            str = reader.next();

//            将所有非数字"\\D"转化为""
//            使用了String中的replaceAll方法
//            从而实现字符串多余字符的去除
            str = str.replaceAll("\\D","");

//           调用了本地的change方法，实现纯数字转换
            result = change(str);
            System.out.println(result);
        }catch (NumberFormatException e){       //捕获异常
            System.out.println("发生异常：" + e.getMessage());
        }
    }
}

