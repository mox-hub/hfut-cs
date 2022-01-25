package exp02;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Random;
import java.util.function.DoubleToIntFunction;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.io.*;
import java.util.Scanner;

/**
 * @ClassName :exp02
 * @Description :随机生成词
 * @Author :Mox
 * @Date :2022/1/16 12:54
 * @Version : v1.0
 **/

public class exp02 {
    /**
     * @input 单词文件 src/data/singleWord_ci.txt
     * @input 双词文件 src/data/doubleWord_ci.txt
     * @input 宋词文件 src/data/Ci.txt
     * @return 数据结构部分写入 src/data/Struct.txt，生成的词部分写入控制台
     */
    public static void main(String args[]){
        //定义变量
        Map<String,String> map = new HashMap<String,String>();//构造map
        ArrayList<String> singleWord = new ArrayList<String>();//构造动态数组
        ArrayList<String> doubleWord = new ArrayList<String>();
        Pattern pattern = Pattern.compile("([\u4E00-\u9FA5]*)(.)");
        Matcher mat;
        Random random = new Random();
        int m,clen = 100;
        Boolean exist[] = new Boolean[clen];
        for(int i = 0; i < clen; i++){
            exist[i] = false;
        }

        try{
            // 预读取文件
            String title="";
            String single, doubles;
            BufferedReader singleBufferRead = new BufferedReader(new FileReader("src\\data\\singleWord_ci.txt"));//读取实验1生成的文件
            BufferedReader doubleBufferRead = new BufferedReader(new FileReader("src\\data\\doubleWord_ci.txt"));
            BufferedReader bufferRead = new BufferedReader(new FileReader(new File("src\\data\\Ci.txt")));
            BufferedWriter bufferWrite = new BufferedWriter(new FileWriter(new File("src\\data\\Struct.txt")));
            String text = bufferRead.readLine();
            String sent="";
            String struct="";

            //将若干single，double词从文件中读取放入动态数组singleWord，doubleWord中
            single = singleBufferRead.readLine();
            while(single != null){
                singleWord.add(single);
                single = singleBufferRead.readLine();
            }

            doubles = doubleBufferRead.readLine();
            while(doubles!=null){
                doubleWord.add(doubles);
                doubles = doubleBufferRead.readLine();
            }

            singleBufferRead.close();
            doubleBufferRead.close();

            //处理词的源文件，获取结构
            while(text != null){
                if(text.length() < 2){//去掉一些乱码
                    text = bufferRead.readLine();
                    continue;
                }else if(text.length()<6){//不到6则判定为标题，在下面存入标题中
                    title = text;
                }else{
                    if(map.containsKey(title)){//如果已有该title，则text继续读入下一行
                        text=bufferRead.readLine();
                        continue;
                    }else{
                        mat = pattern.matcher(text);//去掉词中的非汉字字符
                        while(mat.find()){
                            sent = mat.group(1);//取分段后的第一段
                            int len = sent.length();//读取该段长度
                            while(len > 0){
                                if(len > 3){//如果是7个字符，则划分为223，如果是5个字符，则划分为23，以此类推
                                    struct += 2;//struc加上字符串'2'
                                    len -= 2;//长度-2
                                }else{
                                    struct += len;//struc加上字符串为'该段长度'，如7，则此时就是3
                                    len = 0;//该段长度置0
                                }
                            }
                            struct += mat.group(2);//开始处理分段后的第二段
                        }
                        map.put(title, struct);//将得到的格式放入map
                        bufferWrite.write(title+":"+struct+"\r\n");
                        struct = "";
                    }
                }
                text = bufferRead.readLine();//读取
            }

            bufferWrite.close();
            bufferRead.close();

//            System.out.println(map);

        }catch(Exception e){
            System.out.println(e);
        }

        //开始进行宋词的生成
        System.out.println("请输入你想生成的宋词(词牌名)类型:");//根据词牌名生成词
        Scanner input = new Scanner(System.in);
        String title = input.nextLine();//将输入的词牌名存入title
        input.close();
        String text;
        if(map.containsKey(title)){
            text = map.get(title);//取出对应的格式
            String output = title+"\n";//先将输出的词牌名写入输出
            for(int i = 0;i<text.length();i++){//词的总字数text不够，则继续随机加词产生句子
                m = random.nextInt(clen);//随机产生0-100的数字
                while(exist[m])
                    m = random.nextInt(clen);
                exist[m]=true;//text的格式是223这样
                if(text.charAt(i) == '1'){//charAt()返回指定索引位置的char值
                    output += singleWord.get(m).split(":")[0];//需要一个字符，给一个单字
                }else if(text.charAt(i) == '2'){
                    output += doubleWord.get(m).split(":")[0];//需要两个字符，给一个双字
                }else if(text.charAt(i)=='3'){
                    output += singleWord.get(m).split(":")[0];//需要三个字符，给一双一单
                    output += doubleWord.get(m).split(":")[0];
                }else{
                    output += (text.charAt(i)+"\n");
                }
            }
            System.out.println(output);
        }else{
            System.out.println("warning：词牌名不存在！");
        }
    }
}
