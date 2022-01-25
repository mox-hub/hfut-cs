package exp01;

import java.io.*;
import java.util.Map;
import java.util.regex.Pattern;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
/**
 * @ClassName :exp01_1
 * @Description : 用于处理宋词内容
 * @Author :Mox
 * @Date :2022/1/16 12:03
 * @Version : v1.0
 **/

public class exp01_1 {
    public static void main(String args[]){

        Map<String,Integer> sword=new HashMap<String,Integer>();//定义一个两列的单字map
        Map<String,Integer> dword=new HashMap<String,Integer>();//定义一个两列的双字map

        try{
            File fp = new File("src\\data\\Ci.txt");
            InputStreamReader read = new InputStreamReader(new FileInputStream(fp),"GBK");//用GBK方式读Ci.txt
            BufferedReader bfread = new BufferedReader(read);
            String LText = bfread.readLine();//Ltext为每行

            while(LText!=null){
                if(LText.length() <= 5){
                    //读取到词牌名则跳过，转而读下面的词内容（纵览发现词牌名不会超过5个字）
                    LText = bfread.readLine();
                    continue;
                }else{//读取词内容
                    int len = LText.length();//词长度为len
                    for(int i = 0; i < len; i++){//挨个读并且剔出非汉字字符
                        String str = LText.substring(i,i+1);//substring() 方法用于提取字符串中介于两个指定下标之间的字符
                        int t = Pattern.compile("[\u4E00-\u9FA5]*").split(str).length;   //判断字符串中是否全为中文，如果是，则分割后无字符串，否则会存在一个字符串数组， split()方法根据匹配给定的正则表达式来拆分字符串
                        if(t!=0)//只读取汉字字符
                            continue;
                        else if(sword.containsKey(str)){//存储单字
                            int tem=sword.get(LText.substring(i,i+1))+1;//单字已经存在则加1
                            sword.put(LText.substring(i,i+1), tem);
                        }else{
                            sword.put(LText.substring(i,i+1), 1);//单字不存在则新建置1
                        }
                    }

                    for(int i = 1; i < LText.length(); i++){
                        String str = LText.substring(i-1,i+1);//每次提取两个字符
                        int t = Pattern.compile("[\u4E00-\u9FA5]*").split(str).length;
                        if(t!=0)
                            continue;
                        if(dword.containsKey(LText.substring(i-1,i+1))){
                            int tem=dword.get(LText.substring(i-1,i+1))+1;//双字已存在则加1
                            dword.put(LText.substring(i-1,i+1), tem);
                        }else{
                            dword.put(LText.substring(i-1,i+1), 1);//双字不存在则置1
                        }
                    }                          //统计双字次数

                    LText=bfread.readLine();
                }

            }
            List<Map.Entry<String, Integer>> slist = new ArrayList<Map.Entry<String, Integer>>(sword.entrySet());  //将结果写入list
            Collections.sort(slist, new Comparator<Map.Entry<String, Integer>>() {
                public int compare(java.util.Map.Entry<String, Integer> o1, java.util.Map.Entry<String, Integer> o2) {
                    return o2.getValue().compareTo(o1.getValue());
                }
            });

            List<Map.Entry<String, Integer>> dlist = new ArrayList<Map.Entry<String, Integer>>(dword.entrySet());
            Collections.sort(dlist, new Comparator<Map.Entry<String, Integer>>() {
                public int compare(java.util.Map.Entry<String, Integer> o1, java.util.Map.Entry<String, Integer> o2) {
                    return o2.getValue().compareTo(o1.getValue());
                }
            });

            bfread.close();//从list写入文件中去
            read.close();
            BufferedWriter bwrite=new BufferedWriter(new FileWriter("src\\data\\singleWord_ci.txt"));

            for(Map.Entry<String,Integer> key:slist){
                bwrite.write(key.getKey()+":"+key.getValue()+"\r\n");
            }
            bwrite.close();
            bwrite=new BufferedWriter(new FileWriter("src\\data\\doubleWord_ci.txt"));

            for(Map.Entry<String,Integer> key:dlist){
                bwrite.write(key.getKey()+":"+key.getValue()+"\r\n");
            }
            bwrite.close();

            System.out.println("程序运行结束，请检查文件");

        }catch(Exception e){
            System.out.print(e);
        }
    }
}
