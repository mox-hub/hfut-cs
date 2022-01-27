package bianYiYuanLi;

import java.io.*;
import java.util.*;

public class work1 {
	static ArrayList<String> k=new ArrayList<String>(Arrays.asList("do","end","for","if","printf","scanf","then","while")); //关键字
	static ArrayList<String> s=new ArrayList<String>(Arrays.asList(",",";","(",")","[","]")); //分界符
	static ArrayList<String> a=new ArrayList<String>(Arrays.asList("+","-","*","/")); //算术运算符
	static ArrayList<String> r=new ArrayList<String>(Arrays.asList("<","<=",">",">=","=","<>")); //关系运算符
	static ArrayList<String> ci=new ArrayList<>(); //常数
    static ArrayList<String> id=new ArrayList<>(); //标识符
    static int hangX=1; //行
    static int lieY=1; //列
    
    //查找识别方法
    static void searchDis(String str) {
    	int i=0; //区分String的首符，1为字母，2为数字，3为其他
    	char ch=str.charAt(0);
    	if ((ch>=65&&ch<=90)||(ch>=97&&ch<=122))
    		i=1;
    	else if(ch>=48&&ch<=57)
    		i=2;
    	else
    		i=3;
    	
    	//开始查找识别
    	if (i==1) { //首符是字母，查关键字和标识符
    		boolean find=k.contains(str);
    		if(find==true) {
    			//System.out.println(str+'\t'+'\t'+"(1,"+str+")"+'\t'+'\t'+"关键字"+'\t'+'\t'+"("+hangX+","+lieY+")");
    			System.out.printf("%-10s%-10s%-10s%-10s",str,"(1,"+str+")","关键字","("+hangX+","+lieY+")");
    			System.out.println();
    		}
    		else { //不是关键字，查询标识符
    			find=id.contains(str);
    			if(find==true) {
    				//System.out.println(str+"  (6,"+str+")  "+"标识符"+"  ("+hangX+","+lieY+")");
    				System.out.printf("%-10s%-10s%-10s%-10s",str,"(6,"+str+")","标识符","("+hangX+","+lieY+")");
        			System.out.println();
    			}
    			else { //标识符表没有时，插入
    				//System.out.println(str+"  (6,"+str+")  "+"标识符"+"  ("+hangX+","+lieY+")");
    				System.out.printf("%-10s%-10s%-10s%-10s",str,"(6,"+str+")","标识符","("+hangX+","+lieY+")");
        			System.out.println();
    				id.add(str); 
    			}
    		}
    	}
    	else if(i==2) { //首符是数字,查常数
    		boolean find=ci.contains(str);
    		if(find==true) {
    			//System.out.println(str+"  (5,"+str+")  "+"常数"+"  ("+hangX+","+lieY+")");
    			System.out.printf("%-10s%-10s%-10s%-10s",str,"(5,"+str+")","常数","("+hangX+","+lieY+")");
    			System.out.println();
    		}
    		else { //不在常数表中，插入
    			//System.out.println(str+"  (5,"+str+")  "+"常数"+"  ("+hangX+","+lieY+")");
    			System.out.printf("%-10s%-10s%-10s%-10s",str,"(5,"+str+")","常数","("+hangX+","+lieY+")");
    			System.out.println();
    			ci.add(str);
    		}
    	}
    	else { //查分界符，算术、逻辑运算符
    		if(s.contains(str)) {
    			System.out.printf("%-10s%-10s%-10s%-10s",str,"(2,"+str+")","分界符","("+hangX+","+lieY+")");
    			System.out.println();
    		}
    		else if(a.contains(str)) {
    			System.out.printf("%-10s%-10s%-10s%-10s",str,"(3,"+str+")","算术运算符","("+hangX+","+lieY+")");
    			System.out.println();
    		}
    			
    		else if(r.contains(str)) {
    			System.out.printf("%-10s%-10s%-10s%-10s",str,"(4,"+str+")","关系运算符","("+hangX+","+lieY+")");
    			System.out.println();
    		}
    			
    		else {
    			System.out.printf("%-10s%-10s%-10s%-10s",str,"Error","Error","("+hangX+","+lieY+")");
    			System.out.println();
    		}
    			
    	}
    		
    }
    
    //分析串函数
    static void analysisString(String str2) {
    	String strNow=""; //当前识别的串
    	char ch1;
    	for(int i=0;i<str2.length();i++) {
    		ch1=str2.charAt(i);
    		if(ch1==' ')
    			continue;
    		if((ch1>=65&&ch1<=90)||(ch1>=97&&ch1<=122)) { //识别字母
    			strNow+=ch1;
    			i++;
    			//超前搜索
    			do {
    				ch1=str2.charAt(i);
    				if((ch1>=65&&ch1<=90)||(ch1>=97&&ch1<=122)||(ch1>=48&&ch1<=57)) {
    					strNow+=ch1;
    					i++;
    				}
    				else
    					break;
    			}while(i<str2.length());
    			i--; //指针复位
    			searchDis(strNow);
    			strNow="";
    			lieY++;
    			continue;
    		}
    		
    		else if(ch1>=48&&ch1<=57) { //识别数字
    			strNow+=ch1;
    			i++;
    			//超前搜索
    			//将后续如果是字母或者是数字全部加进去
    			while(i<str2.length()){
    				ch1=str2.charAt(i);
    				if((ch1>=65&&ch1<=90)||(ch1>=97&&ch1<=122)||(ch1>=48&&ch1<=57)||ch1=='.') {
    					strNow+=ch1;
    					i++;
    				}
    				else
    					break;
    			}
    			i--; //指针复位
    			//判断数字中是否有字母，有输出为error
    			int j;
    			boolean errorDone=false; //标志是否做出了出错判定
    			for(j=0;j<strNow.length();j++) {
    				char ch2=strNow.charAt(j);
    				if((ch2>=65&&ch2<=90)||(ch2>=97&&ch2<=122)) {
    					System.out.printf("%-10s%-10s%-10s%-10s",strNow,"Error","Error","("+hangX+","+lieY+")");
    					System.out.println();
    					errorDone=true;
    					break;
    				}
    			}
    			//判断数字中有几个小数点，有两个及以上出error
    			int m;
    			boolean tag=false; //tag为true时，表示已经识别到一个小数点
    			for(m=0;m<strNow.length();m++) {
    				char ch2=strNow.charAt(m);
    				if(ch2=='.'&&tag) {
    					if(!errorDone) { //由于上面已经输出error，下面就不用再输出
    						System.out.printf("%-10s%-10s%-10s%-10s",strNow,"Error","Error","("+hangX+","+lieY+")");
    						System.out.println();
    					}
    					errorDone=true;
    					break;
    				}
    				else if(ch2=='.'&&!tag) {  //第一个小数点识别
    					tag=true;
    				}
    			}
    			if(!errorDone)
    				searchDis(strNow);
    			strNow="";
    			lieY++;
    			continue;
    		}
    		
    		else { //识别其他
    			strNow+=ch1;
    			if(s.contains(strNow)) { //识别为分界符
    				searchDis(strNow);
    				strNow="";
        			lieY++;
        			continue;
    			}
    			else if(a.contains(strNow)||r.contains(strNow)) { //识别为算术运算符或关系运算符
    				i++;
    				//判断运算符后面是否是关系或算术运算符，是则继续添加到strNow中
    				while(i<str2.length()) {
    					ch1=str2.charAt(i);
    					if(a.contains(ch1+"")||r.contains(ch1+"")) {
    						strNow+=ch1;
    						i++;
    					}
    					else
    						break;
    				}
    				searchDis(strNow);
    				i--; //复位
    				strNow="";
        			lieY++;
        			continue;
    			}
    			else { //识别非法字符
    				searchDis(strNow);
    				strNow="";
        			lieY++;
        			continue;
    			}
    		}
    	}
    }
    //主函数
    public static void main(String[] args) {
    	try {
    		RandomAccessFile inF=new RandomAccessFile("test.txt","r");
			String str1=null;
			while((str1=inF.readLine())!=null) { //一行一行的读取
				//删除注释
				for(int i=0;i<str1.length()-1;i++) {
					if(str1.charAt(i)=='/'&&str1.charAt(i+1)=='/') {
						str1=str1.substring(0, i);
						break;
					}
				}
				analysisString(str1); //调用分析函数
				hangX++; //行加1
				lieY=1; //列置1
			}
			
			inF.close();
		}
		catch(IOException e) {
			System.out.println(e);
		}
    }
}
