//LL(1)分析法（有界面）


package bianYiYuanLi;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashSet;
import java.util.Map;
import java.util.Scanner;
import java.util.Stack;
import java.awt.*;
import javax.swing.*;
import java.awt.event.*;

public class Work2_1 {
	static ArrayList<String> produceFormula=new ArrayList<String>(Arrays.asList("E->TG","G->+TG|-TG","G->%","T->FS","S->*FS|/FS","S->%","F->(E)","F->i")); //产生式,%表示空
	static ArrayList<String> vt=new ArrayList<>(); //终结符集
	static ArrayList<String> vn=new ArrayList<>(); //非终结符集
	static HashMap<String,ArrayList<String>> first=new HashMap<>(); //first集
	static HashMap<String,ArrayList<String>> follow=new HashMap<>(); //follow集
	static HashMap<String,ArrayList<String>> produceFormula_1=new HashMap<>(); //产生式的另一种形式
	static String [][] analysisTable; //分析表
	
	static String stepStr=""; //存放输出框的内容
	
	//产生式形式转换
	static void produceFormulaHandle() {
		//先构建所有产生式，右部为空
		for(int i=0;i<vn.size();i++) 
			produceFormula_1.put(vn.get(i),new ArrayList<String>());
		
		for(int i=0;i<produceFormula.size();i++) {
			String str=produceFormula.get(i);
			String key=str.substring(0,1); //取出产生式左边
			ArrayList<String> valueTemp=produceFormula_1.get(key); //取出对应key的value
			str=str.substring(3); //截取产生式右边
			//处理右边的式子，以|分割
			String [] str1=str.split("\\|");
			Collections.addAll(valueTemp,str1);
			produceFormula_1.put(key,valueTemp); //处理完后，放入哈希表中
		}
	}
	
	//打印产生式
	static void printpro() {
		System.out.println("产生式");
		Iterator iter = produceFormula_1.entrySet().iterator();
		while (iter.hasNext()) {
			Map.Entry entry = (Map.Entry) iter.next();
			Object key1 = entry.getKey();
			Object value1 = entry.getValue();
			System.out.println(key1 + ":" + value1);
		}
	}
	
	//构造非终结符集和终结符集
	static void vtAndvn() {
		String str;
		char ch;
		for(int i=0;i<produceFormula.size();i++) {
			str=produceFormula.get(i);
			for(int j=0;j<str.length();j++) {
				ch=str.charAt(j);
				if(ch>='A'&&ch<='Z') {
					if(!vn.contains(ch+""))//如果非终结符里面没有就加入
						vn.add(ch+"");
				}
				else if(j!=1&&ch!='>'&&ch!='|'&&ch!='%') { //消除->和|和空串的影响
					if(!vt.contains(ch+""))//如果终结符里面没有就加入
						vt.add(ch+"");
				}
			}
		}
	}
	
	//打印vn和vt
	static void printVnAndVt() {
		System.out.println("vn");
		for(int i=0;i<vn.size();i++) {
			System.out.println(vn.get(i));
		}
		System.out.println("vt");
		for(int i=0;i<vt.size();i++) {
			System.out.println(vt.get(i));
		}
	}
	
	//构造first集
	static void firstCreate(String str) {
		
		if(first.containsKey(str)) //判断是否构造过str的first
			return;
		
		if(vt.contains(str)) { //终结符
			ArrayList<String> arrayListTemp=new ArrayList<String>();
			Collections.addAll(arrayListTemp,str);
			first.put(str, arrayListTemp);
		}
		else { //非终结符
			ArrayList<String> firstValue=new ArrayList<String>(); //用来存放str的first集
			ArrayList<String> arrayListTemp=produceFormula_1.get(str); //取出str的所有右部候选式
			for(int i=0;i<arrayListTemp.size();i++) { //对非终结符每个右部候选式分析
				String str1=arrayListTemp.get(i);
				if(vt.contains(str1.substring(0, 1))||str1.charAt(0)=='%') //如果候选式首字符为终结符或者是空串，直接将其加入first
					firstValue.add(str1.substring(0, 1));
				else { //如果候选式首字符为非终结符，则从左到右对每一个字符进行判断
					int j;
					for(j=0;j<str1.length();j++) {
						String str2=str1.charAt(j)+"";
						ArrayList<String> arrayListTemp1=produceFormula_1.get(str2); //取出str2的所有右部候选式
						if(arrayListTemp1.contains("%")) //如果str2可以推出空
							continue;
						else {
							firstCreate(str2);
							ArrayList<String> arrayListTemp2=first.get(str2); //取出str2的first集
							firstValue.addAll(arrayListTemp2); //将str2的first集加到str的first中
							break;
						}
					}
					if(j==str1.length()) //如果都能推出空
						firstValue.add("%"); //将空加入
				}
			}
			first.put(str,firstValue); //存入hashmap中
		}
	}
	
	//打印first集
	static void printFirst() {
		System.out.println("first");
    	Iterator iter = first.entrySet().iterator();
		while (iter.hasNext()) {
			Map.Entry entry = (Map.Entry) iter.next();
			Object key = entry.getKey();
			Object value = entry.getValue();
			System.out.println(key + ":" + value);
		}
	}
	
	
	//构造follow集
	static void followCreate(String str) {
		
		if(follow.containsKey(str)) //判断是否构造过str的follow
			return;
		
		ArrayList<String> followtValue=new ArrayList<String>(); //用来存放str的follow集
		if(vn.get(0).equals(str)) //如果str是开始符号，将#加入follow集
			followtValue.add("#");
		//遍历所有的产生式
		for(int i=0;i<vn.size();i++) { 
			if(vn.get(i).equals(str))
				continue;
			ArrayList<String> arrayList1=produceFormula_1.get(vn.get(i));
			//遍历每个产生式右部
			for(int j=0;j<arrayList1.size();j++) {
				String str1=arrayList1.get(j);
				for(int m=0;m<str1.length();m++) {
					if((str1.charAt(m)+"").equals(str)) { //搜索到字符串中有str
						if(m==str1.length()-1) { //若是A->aB的形式
							followCreate(vn.get(i)); //构造A的follow集
							ArrayList<String> arrayList2=follow.get(vn.get(i)); //取出follow(A)
							followtValue.addAll(arrayList2); //follow(A)加到follow(B)中
						}
						else { //若是A->aBb的形式
							ArrayList<String> arrayList3=first.get(str1.charAt(m+1)+""); //取出first(b)
							if(arrayList3.contains("%")) { //first(b)里面有空
								arrayList3.remove("%"); //除去空
								followCreate(vn.get(i)); //构造A的follow集
								ArrayList<String> arrayList4=follow.get(vn.get(i)); //取出follow(A)
								followtValue.addAll(arrayList4); //follow(A)加到follow(B)中
							}
							followtValue.addAll(arrayList3); //将first(b)除去空之后加到follow(B)
							
						}
						break;
					}
					
				}
			}
		}
		LinkedHashSet<String> hashSet = new LinkedHashSet<>(followtValue);
        followtValue = new ArrayList<>(hashSet);
		follow.put(str, followtValue);
	}
	
	//打印follow集
	static void printFollow() {
		System.out.println("follow");
    	Iterator iter = follow.entrySet().iterator();
		while (iter.hasNext()) {
			Map.Entry entry = (Map.Entry) iter.next();
			Object key = entry.getKey();
			Object value = entry.getValue();
			System.out.println(key + ":" + value);
		}
	}
	
	//构造分析表
	static void analysisTableCreate() {
		
		analysisTable=new String[vn.size()][vt.size()+1];
		
		for(int i=0;i<vn.size();i++) {
			ArrayList<String> arrayList1=produceFormula_1.get(vn.get(i)); //拿出第一个非终结符的产生式右部
			for(int j=0;j<arrayList1.size();j++) {
				String str1=arrayList1.get(j); //str1存储产生式
				if(str1.equals("%")) { //若是A->%的形式
					ArrayList<String> arrayList2=follow.get(vn.get(i)); //取出A的follow集
					for(int m=0;m<vt.size();m++) { //遍历vt
						if(arrayList2.contains(vt.get(m))) //非终结符b属于follow(A)
							analysisTable[i][m]=vn.get(i)+"->"+str1; //A->%加至M[A,b]
					}
					if(arrayList2.contains("#")) 
						analysisTable[i][vt.size()]=vn.get(i)+"->"+str1;
				}
				else { //若是A->a(的形式）
					ArrayList<String> arrayList3=first.get(str1.charAt(0)+""); //取出产生式首符的first集
					for(int m=0;m<vt.size();m++) { //遍历vt
						if(arrayList3.contains(vt.get(m))) //非终结符b属于first(a)
							analysisTable[i][m]=vn.get(i)+"->"+str1; //A->a加至M[A,b]
					}
					if(arrayList3.contains("%")) { //如果first(a)里面有空串
						ArrayList<String> arrayList2=follow.get(vn.get(i)); //取出A的follow集
						for(int m=0;m<vt.size();m++) { //遍历vt
							if(arrayList2.contains(vt.get(m))) //非终结符b属于follow(A)
								analysisTable[i][m]=vn.get(i)+"->"+str1; //A->a加至M[A,b]
						}
						if(arrayList2.contains("#")) 
							analysisTable[i][vt.size()]=vn.get(i)+"->"+str1;
					}
				}
			}
		}
	}
	
	//打印分析表
	static void printAnalysic() {
		System.out.println("分析表");
		System.out.print("table"+'\t');
		for(int i=0;i<vt.size();i++) {
			System.out.print(vt.get(i)+'\t');
		}
		System.out.println("#"+'\t');
		for(int i=0;i<vn.size();i++)
		{
			System.out.print(vn.get(i)+'\t');
			for(int j=0;j<vt.size()+1;j++)
				System.out.print(analysisTable[i][j]+'\t');
			System.out.println();
		}
	}
	
	//分析程序
	static void distinguish() {
		
		vt.add("#"); //暂时在终结符集中加入#
		
		Stack<String> symbolStack=new Stack<String>(); //符号栈
		//读入字符串
		String inStr=work2Window.inputStr;
		
		String a; //存放输入符号a
		String x; //存放弹出的栈顶符号
		int step=-1; //步骤数
		String proStr="null"; //存放所用产生式
		String pushStr=""; //存放push动作的对象
		String actionStr=""; //存放动作
		
		//判断输入串是否正规
		for(int i=0;i<inStr.length();i++) {
			if(!vt.contains(inStr.charAt(i)+"")) {//输入串中有不在vt中的符号
				stepStr+="!-----输入串错误,不包含此符号-----!";
				return ;
			}
		}
		
		symbolStack.push("#");//#入栈
		symbolStack.push(vn.get(0)); //开始符号入栈
		actionStr="初始化";
		
		stepStr+=("输入串:"+inStr+'\n');
		stepStr+=("步骤"+'\t'+"分析栈"+'\t'+"剩余串"+'\t'+"产生式"+'\t'+"动作"+'\t'+'\n');
		
		a=inStr.charAt(0)+""; //取一输入符号a
			
			
			
		while(true) {
			step++; //步骤+1
			
			//获得分析栈所要打印的字符串
			String stackStr=""; //存放分析栈里的元素
			while(!symbolStack.empty()) {
				stackStr+=symbolStack.pop();
			}
			String stackStr1=new StringBuffer(stackStr).reverse().toString(); //反转字符串
			for(int i=0;i<stackStr1.length();i++){
				symbolStack.push(stackStr1.charAt(i)+""); //重新恢复栈
			}
			
			//打印步骤
			stepStr+=((step+"")+'\t'+stackStr1+'\t'+inStr+'\t'+proStr+'\t'+actionStr+'\t'+'\n');
			proStr="null";
			actionStr="";
			
			x=symbolStack.pop(); //弹出栈顶符号放入x
			
			if(!vn.contains(x)) { //x是不是非终结符
				if(vt.contains(x)) { //x是终结符
					if(x.equals(a)) { //x=a
						if(x.equals("#")) //x=# 代表结束，成功
							break;
						else { //x!=#开始下一步骤
							inStr=inStr.substring(1);
							a=inStr.charAt(0)+""; //取一输入输入符号a
							actionStr="GETNEXT(I)";
							continue;
						}
					}
					else {
						//System.out.println("x!=a"); //error
						stepStr+=("Error--------------------x!=a"+'\n');
						break;
					}
				}
				else {
					//System.out.println("not vt and not vn"); //error
					stepStr+=("Error--------------------not vt and not vn"+'\n');
					break;
				}
			}
				
			else {
				//取得x,a的位置
				int xIndedx=vn.indexOf(x);
				int aIndedx=vt.indexOf(a);
				
				if(analysisTable[xIndedx][aIndedx]!=null) {
					//symbolStack.pop(); //栈顶出栈
					actionStr+="pop";
					String str1=analysisTable[xIndedx][aIndedx]; //str1=M[x,a]
					proStr=str1;
					
					String str2=new StringBuffer(str1).reverse().toString(); //反转字符串
					if(str2.charAt(0)=='%') //若产生式是推出空串的情况
						continue;
					//倒序入栈
					for(int j=0;j<str1.length();j++) {
						if(str2.charAt(j)=='>')
							break;
						symbolStack.push(str2.charAt(j)+"");
						pushStr+=str2.charAt(j)+"";
					}
					
					actionStr+=",push("+pushStr+")";
					pushStr="";
					
					continue;
				}
			
				else {
					//System.out.println("no M[A,a]"); //error
					stepStr+=("Error--------------------no M[A,a]"+'\n');
					break;
				}	
			}
		}
			
		vt.remove("#"); //移去#，恢复vt
	}
	
	
    public static void main(String[] args) {
    	
    	vtAndvn();//构造vn和vt
    	//printVnAndVt();
    	
    	produceFormulaHandle(); 
    	//printpro();
    	
    	//构造first集
    	for(int i=0;i<vn.size();i++)
    		firstCreate(vn.get(i));
    	for(int i=0;i<vt.size();i++)
    		firstCreate(vt.get(i));
    	//printFirst();
    	
    	//构造follow集
    	for(int i=0;i<vn.size();i++)
    		followCreate(vn.get(i));
    	//printFollow();
    	
    	//构造分析表
    	analysisTableCreate();
    	printAnalysic();
    	
    	//调用窗口
    	new work2Window();
    	
    }
}




//窗口
class work2Window extends JFrame implements ActionListener{

	JButton action;
	JTextField inputText;
	JTextArea outputText;
	static String inputStr=null;
	
	public work2Window() {
		action=new JButton("分析");
		action.setFont(new Font("楷体", Font.PLAIN, 30));
		inputText=new JTextField(50); //输入框
		inputText.setFont(new Font("楷体", Font.PLAIN, 30));
		inputText.setText("i+i*i#");
		outputText=new JTextArea(); //显示框
		outputText.setFont(new Font("楷体", Font.PLAIN, 30));
		outputText.setEditable(false);
		
		JPanel pNorth=new JPanel();
		pNorth.add(inputText);
		
		JPanel pSouth=new JPanel();
		pSouth.add(action);
		
		add(pNorth,BorderLayout.NORTH);
		add(new JScrollPane(outputText),BorderLayout.CENTER);
		add(pSouth,BorderLayout.SOUTH);
		
		action.addActionListener(this);
		setBounds(200,200,1000,600);
		setVisible(true);
		setTitle("LL(1)分析");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	
	@Override
	public void actionPerformed(ActionEvent e) {
		if(e.getSource()==action) {
			inputStr=inputText.getText();//取得输入串
			if(inputStr.length()>1)
				Work2_1.distinguish();//调用分析程序
			
			outputText.setText(Work2_1.stepStr); //输出
			Work2_1.stepStr="";
			//outputText.append(work2_temp.stepStr);
		}
		
	}
	
}
