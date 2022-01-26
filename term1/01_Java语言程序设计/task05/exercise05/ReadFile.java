package exercise05;

import java.io.*;

public class ReadFile {
    public static void main(String[] args) {
        String s;
        File file = new File(".\\read.txt");
        FileInputStream fis;
        InputStreamReader isr;
        BufferedReader br;
        System.out.println("读取的文件数据为：");
        try {
            fis = new FileInputStream(file);
            isr = new InputStreamReader(fis,"UTF-8");
            br = new BufferedReader(isr);
            s = br.readLine();
            while (s != null) {
                System.out.println(s);
                s = br.readLine();
            }
        } catch (FileNotFoundException e) {
            System.out.println("Error:" + e);
        } catch (IOException e) {
            System.out.println("Error:" + e);
        }
    }
}
