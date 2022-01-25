package exp03;

import java.io.*;
import java.util.*;
import java.lang.String;

/**
 * @ClassName :exp03
 * @Description ：实验三内容，对中文进行分词解析
 * @Author :Mox
 * @Date :2022/1/16 13:33
 * @Version : v1.0
 **/

public class exp03 {
    /**
     * FMM算法函数
     * @param str
     * @param map
     * @return result = 分词结果
     */
    public static String FMM(String str,Map<String, String> map){
        str = str.replace("/r","");
        int length = 2;
        String sub;
        String result = "";
        while (str.length() != 0) {
            sub = str.substring(0,2);
            while (!map.containsKey(sub)) {
                if(sub.length() > 1)
                    sub = sub.substring(1);
                else
                    break;
            }
            result += sub + "/";
            str = str.substring(sub.length());
        }
        return result;
    }
    /**
     * BMM算法函数
     * @param str
     * @param map
     * @return result = 分词结果
     */
    public static String BMM(String str,Map<String, String> map){
        str = str.replace("/r","");
        int length = 2;
        String sub;
        String result = "";
        while (str.length() != 0) {
            sub = str.substring(str.length() - 2);
//            System.out.println(sub);
            while (!map.containsKey(sub)) {
                if(sub.length() > 1)
                    sub = sub.substring(1);
                else
                    break;
            }
            result = sub + "/" + result;
            str = str.substring(0,str.length() - sub.length());
//            System.out.println("str = "+str);
        }
        return result;
    }

    public static void main(String args[]) {
        //定义变量
        Map<String, String> map = new HashMap<String, String>();//构造map
        try {
            // 预读取文件
            String words;
            String first = "";
            String second = "";

            BufferedReader doubleBufferRead = new BufferedReader(new FileReader("src\\data\\doubleWord_news.txt"));

            words = doubleBufferRead.readLine();
            while (words != null) {
                first = words.split(":")[0];
                second = words.split(":")[1];
                map.put(first, second);//将得到的格式放入map
                words = doubleBufferRead.readLine();
            }
            doubleBufferRead.close();
//            System.out.println(map);
        } catch (Exception e) {
            System.out.println(e);
        }
        System.out.println("请输入字符串:");
        Scanner input = new Scanner(System.in);
        String words = input.nextLine();
        input.close();

        System.out.println("FMM:" + FMM(words,map));
        System.out.println("BMM:" + BMM(words,map));
    }
}
