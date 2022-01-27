//LR(1)分析法（有界面）

package bianYiYuanLi;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.Map;
import java.util.Scanner;
import java.util.Stack;
import java.net.*;
import java.io.*;
import java.awt.*;
import javax.swing.*;
import java.awt.event.*;

/*class project{
	String produceFormula_2; //产生式
	ArrayList<String> searchChar=new ArrayList<String>(); //向前搜索符
}*/

public class work3_1 {
	static ArrayList<String> produceFormula=new ArrayList<String>(Arrays.asList("E->E+T","E->T","T->T*F","T->F","F->(E)","F->i")); //产生式,%表示空
	//static ArrayList<String> produceFormula=new ArrayList<String>(Arrays.asList("S->BB","B->aB","B->b"));
	static ArrayList<String> vt=new ArrayList<>(); //终结符集
	static ArrayList<String> vn=new ArrayList<>(); //非终结符集
	static HashMap<String,ArrayList<String>> first=new HashMap<>(); //first集
	static HashMap<String,ArrayList<String>> produceFormula_1=new HashMap<>(); //产生式的另一种形式
	static String [][] action; //action表
	static String [][] GOTO; //goto表
	static String projectSet[][]=new String[50][50];//项目集族，每行的第一个元素存项目数
	static int ii=0;// prjectSet的行数，也就是项目集数-1
	static String projectSetLink[][]=new String[50][50]; //暂时存放项目集之间连接关系
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
						if(str2.equals(str)) break;
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
	
	
	
	//Closure(I)
	static void closure(String projectset[]) {
		int j=0; //项目集中正在处理的地方
		int jj=Integer.parseInt(projectset[0]); //取得项目集中项目的个数
		
		if(jj==0) return; //如果为0，直接结束
		
		while(true) { //生成项目集
			j++;
			String str1=projectset[j]; //取出要处理的串
			for(int n=0;n<str1.length()-1;n++) { //遍历串
				if(str1.charAt(n)=='.'&&vn.contains(str1.charAt(n+1)+"")) { //找到类似于.vn的结构
					String str2=str1.substring(n+2); //取出结构.vn..后面的部分
					str2=str2.replaceAll(",",""); //除去,
					ArrayList<String> arraylist1=produceFormula_1.get(str1.charAt(n+1)+""); //取出vn的产生式右部
					for(int m=0;m<arraylist1.size();m++) {
						
						ArrayList<String> arrayList2=first.get(str2.charAt(0)+"");
						for(int k=0;k<arrayList2.size();k++) {
							
							String temp=str1.charAt(n+1)+"->."+arraylist1.get(m)+","+arrayList2.get(k); //形成项目
							boolean has=hasProject(projectset,jj,temp); //判断项目是否存在
							
							if(has==false) { //项目不存在，加入
								jj++;
								projectset[jj]=temp;
							}
								
						}
						
					}
				break;
				}
				
			}
			if(j==jj) { //判断闭包构造完成
				String str3=projectset[j];//取出最后一个串
				boolean flag=false; //结束标志
				for(int n=0;n<str3.length();n++) {
					if(str3.charAt(n)=='.'&&!vn.contains(str3.charAt(n+1)+""))
						flag=true;
				}
				if(flag) break;
			}
		}
		
		projectset[0]=jj+"";
		
		//projectSet[i][jj[i]+1]="0";
	}
	
	//判断项目集中是否已经有这个项目
		static boolean hasProject(String projectset[],int jj,String temp) {
			for(int kk=1;kk<=jj;kk++) {
				if(temp.equals(projectset[kk]))
					return true;
			}
			return false;
		}	
	
	//goto函数
	static String[] Goto(String projectset[],String x) {
		
		String projectsetTemp[]=new String[50]; //存放项目集
		int num=0;// 项目集中项目的数量
		int jj=Integer.parseInt(projectset[0]);
		for(int m=1;m<=jj;m++) {
			
			String str2=projectset[m];
			int pointIndex=str2.indexOf('.');
			if((str2.charAt(pointIndex+1)+"").equals(x)) {
				str2=str2.substring(0,pointIndex)+x+"."+str2.substring(pointIndex+2);
				num++;
				projectsetTemp[num]=str2;
			}
			
		}
		projectsetTemp[0]=num+"";
		
		closure(projectsetTemp); //闭包
		
		return projectsetTemp;
		
	}
	
	//判断项目集族中是否有这个项目集
	static int isExistence(String projectset[]) {
		int a=Integer.parseInt(projectset[0]);
		int b;//
		String projectsetTemp[];
		for(int ip=0;ip<=ii;ip++) {
			projectsetTemp=projectSet[ip];
			b=Integer.parseInt(projectsetTemp[0]);
			if(a!=b) continue;
			int tag=0; //每找到一个重复的+1
			for(int m=1;m<=a;m++) {
				for(int n=1;n<=b;n++) {
					if(projectset[m].equals(projectsetTemp[n])) {
						tag++;
						break;
					}
				}
			}
			if(tag==a) {
				return ip; //重复的是第ip个项目集
			}
			
		}
		return -1; //没有重复
	}
	
	//构造项目集
	static void projectSetCreate() {
		first.put("#", new ArrayList<String>(Arrays.asList("#"))); //在first集中添加<#,#>
		String str=vn.get(0); //取得开始符(文法需要拓广,用A代替开始符',)
		String str1="A->."+str+",#";
		projectSet[0][1]=str1;
		projectSet[0][0]="1";
		
		int i=0; //正在处理的项目集的索引
		closure(projectSet[0]); //构造I0闭包
		while(true) {
			
			for(int m=0;m<vn.size();m++) {
				String x=vn.get(m);
				String projectset[]=Goto(projectSet[i],x);
				int num=Integer.parseInt(projectset[0]);
				if(num==0) {
					continue;
				}
				else {
					int ip=isExistence(projectset);
					if(ip==-1) { //不存在重复
						ii++;
						projectSet[ii]=projectset;
						projectSetLink[i][ii]=x;
					}
					else {
						projectSetLink[i][ip]=x;
					}
				}
				
			}
			
			for(int m=0;m<vt.size();m++) {
				String x=vt.get(m);
				String projectset[]=Goto(projectSet[i],x);
				int num=Integer.parseInt(projectset[0]);
				if(num==0) {
					continue;
				}
				else {
					int ip=isExistence(projectset);
					if(ip==-1) { //不存在重复
						ii++;
						projectSet[ii]=projectset;
						projectSetLink[i][ii]=x;
					}
					else {
						projectSetLink[i][ip]=x;
					}
				}
				
			}
			
			if(i==ii) {
				break;
			}
			i++;
		}
		
	}
	
	
	
	
	
	//打印项目集
	static void printProjectSet() {
		for(int i=0;i<=ii;i++) {
			System.out.println(i);
			int num=Integer.parseInt(projectSet[i][0]);
			for(int j=1;j<=num;j++) {
				if(j==10)
					System.out.println();
				System.out.print(projectSet[i][j]+" ");
			}
			System.out.println();
		}
			
	}
	
	//打印projectSetLink
	static void printprojectSetLink() {
		for(int i=0;i<=ii;i++) {
			System.out.println();
			for(int j=0;j<=ii;j++) {
				System.out.print(projectSetLink[i][j]+'\t');
			}
		}
	}
	
	//构造分析表
	static void createAnalysisTable() {
		vt.add("#"); //终结符中加入#
		action=new String[ii+1][vt.size()]; //初始化action
		GOTO=new String[ii+1][vn.size()]; //初始化goto
		
		int tag[]=new int[ii+1]; //记录这个状态是不是末尾状态，1不是，0是
		//部分
		for(int i=0;i<=ii;i++) {
			for(int j=0;j<=ii;j++) {
				if(projectSetLink[i][j]!=null) {
					tag[i]=1;
					String str=projectSetLink[i][j];
					if(vn.contains(str)) { //非终结符
						GOTO[i][vn.indexOf(str)]=j+"";
					}
					else { //终结符
						action[i][vt.indexOf(str)]="s"+j;
					}
				}
			}
		}
		
		//归约部分
		for(int i=0;i<=ii;i++) {
			if(tag[i]==0) {
				String projectset[]=projectSet[i]; //取出末尾项目集
				int num=Integer.parseInt(projectset[0]);
				for(int j=1;j<=num;j++) {
					String str=projectset[j];
					int pointIndex=str.indexOf('.');
					String strTail=str.substring(str.length()-1);
					str=str.substring(0,pointIndex);
					
					int produceIndex=produceFormula.indexOf(str)+1;
					action[i][vt.indexOf(strTail)]="r"+produceIndex;
				
				}
			}
			else {
				String projectset[]=projectSet[i]; //取出含有可归约的项目的项目集（并非全都是）
				int num=Integer.parseInt(projectset[0]);
				for(int j=1;j<=num;j++) {
					String str=projectset[j];
					int pointIndex=str.indexOf('.');
					int douIndex=str.indexOf(',');
					if(douIndex==pointIndex+1) { //A->a.,?这种结构
						if(str.charAt(0)=='A') {
							action[i][vt.indexOf("#")]="acc";
						}
						else{
							String strTail=str.substring(str.length()-1);
							str=str.substring(0,pointIndex);
							
							int produceIndex=produceFormula.indexOf(str)+1;
							action[i][vt.indexOf(strTail)]="r"+produceIndex;
						}
						
					}
				}
			}
		}
		
		//acc
		//action[1][vt.size()-1]="acc";
		
	}
	
	//打印分析表
	static void printAnalysisTable() {
		System.out.println("ACTION"+'\t'+'\t'+'\t'+'\t'+'\t'+'\t'+'\t'+"GOTO");
		System.out.print("状态"+'\t');
		for(int i=0;i<vt.size();i++)
			System.out.print(vt.get(i)+'\t');
		for(int i=0;i<vn.size();i++)
			System.out.print(vn.get(i)+'\t');
		
		System.out.println();
		for(int i=0;i<=ii;i++) {
			System.out.print(i+""+'\t');
			for(int j=0;j<vt.size();j++)
				System.out.print(action[i][j]+'\t');
			for(int j=0;j<vn.size();j++)
				System.out.print(GOTO[i][j]+'\t');
			System.out.println();
		}
		
	}
	//[start] userxxx
	//识别程序
	static void distinguish() {
		int step=0; //步骤
		Stack<Integer> stateStack=new Stack<Integer>(); //状态栈
		Stack<String> symbolStack=new Stack<String>(); //符号栈
		String inStr=work3Window.inputStr;//读入字符串
		//String inStr="a";
		String a; //输入符号a
		int aIndex=0; //指向输入串的头
		int s; //栈顶状态s
		String actionStr=""; //动作
		
		//判断输入串是否正规
		for(int i=0;i<inStr.length();i++) {
			if(!vt.contains(inStr.charAt(i)+"")) {//输入串中有不在vt中的符号
				stepStr+="!-----输入串错误-----!";
				return ;
			}
		}
		
		stateStack.push(0);
		symbolStack.push("#");
		
		stepStr+=("输入串:"+inStr+'\n');
		stepStr+=String.format("%-18s", "步骤")+String.format("%-18s", "状态栈")+String.format("%-18s", "符号栈")+String.format("%-18s", "输入串")+"动作"+'\n';
		
		while(true) {
			step++;
			s=stateStack.peek();
			a=inStr.charAt(aIndex)+"";
			
			//获取栈中的值
			//状态栈
			String strStateTemp="";
			Stack<Integer> stackStateTemp=new Stack<Integer>();
			while(!stateStack.empty()) {
				
				stackStateTemp.push(stateStack.pop());
			}
			while(!stackStateTemp.empty()) { //恢复栈
				int iState=stackStateTemp.pop();
				strStateTemp+=iState+" ";
				stateStack.push(iState);
			}
			//符号栈
			String strSymbolTemp="";
			while(!symbolStack.empty()) {
				strSymbolTemp+=symbolStack.pop();
			}
			String strSymbolTemp1=new StringBuffer(strSymbolTemp).reverse().toString(); //反转字符串
			for(int i=0;i<strSymbolTemp1.length();i++){
				symbolStack.push(strSymbolTemp1.charAt(i)+""); //重新恢复栈
			}
			//输入串
			String inStrTemp=inStr.substring(aIndex);
			stepStr+=String.format("%-20s", (step+""))+String.format("%-20s", strStateTemp)+String.format("%-20s", strSymbolTemp1)+String.format("%-20s", inStrTemp);
			
			
			String acStr=action[s][vt.indexOf(a)];
			if(acStr!=null) { //如果存在action
				if(acStr.charAt(0)=='s') { //acStr=s？
					stateStack.push(Integer.parseInt(acStr.substring(1))); //状态入栈
					symbolStack.push(a); //输入符号入栈
					actionStr="ACTION["+s+","+a+"]="+acStr+",状态"+Integer.parseInt(acStr.substring(1))+"入栈";
					aIndex++;
				}
				else if(acStr.charAt(0)=='r') { //acStr=r?
					int produceIndex=Integer.parseInt(acStr.substring(1)); //获取产生式的索引
					String produce=produceFormula.get(produceIndex-1); //产生式
					String produceR=produce.substring(3); //产生式右部
					String produceL=produce.substring(0,1); //产生式左部
					int R_Length=produceR.length(); //产生式右部的长度
					//出栈
					for(int i=0;i<R_Length;i++) {
						stateStack.pop();
						symbolStack.pop();
					}
					
					s=stateStack.peek(); //状态栈头
					String goStr=GOTO[s][vn.indexOf(produceL)]; //取得goto=?
					stateStack.push(Integer.parseInt(goStr)); //?入栈
					symbolStack.push(produceL); //归约后，产生式左部入栈
					actionStr=acStr+":"+produce+"归约,GOTO("+s+","+produceL+")="+goStr+"入栈";
				}
				else if(acStr.equals("acc")){ //acStr=acc,结束
					actionStr="ACC,分析成功";
					stepStr+=actionStr+'\n';
					break;
				}
				else { //出错
					actionStr="error";
					stepStr+=actionStr+'\n';
					break;
				}
			}
			else { //出错
				actionStr="没有对应的action----error------";
				stepStr+=actionStr+'\n';
				break;
			}
			
			stepStr+=actionStr+'\n';
		}
		//System.out.println(stepStr);
	}
	//[end]
	
	
	
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
    	
    	projectSetCreate();
    	//printProjectSet();
    	//printprojectSetLink();
    	
    	//分析表
    	createAnalysisTable();
    	printAnalysisTable();

    	//调用窗口
    	new work3Window();
    	//distinguish();
    	
    }
}

//窗口
class work3Window extends JFrame implements ActionListener{

	JButton action;
	JTextField inputText;
	JTextArea outputText;
	static String inputStr=null;
	
	public work3Window() {
		action=new JButton("分析");
		action.setFont(new Font("楷体", Font.PLAIN, 20));
		inputText=new JTextField(50); //输入框
		inputText.setFont(new Font("楷体", Font.PLAIN, 20));
		inputText.setText("i+i*i#");
		outputText=new JTextArea(); //显示框
		outputText.setFont(new Font("楷体", Font.PLAIN, 20));
		outputText.setEditable(false);
		
		JPanel pNorth=new JPanel();
		pNorth.add(inputText);
		
		JPanel pSouth=new JPanel();
		pSouth.add(action);
		
		add(pNorth,BorderLayout.NORTH);
		add(new JScrollPane(outputText),BorderLayout.CENTER);
		add(pSouth,BorderLayout.SOUTH);
		
		action.addActionListener(this);
		setBounds(200,200,1200,800);
		setVisible(true);
		setTitle("LR(1)分析");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	
	@Override
	public void actionPerformed(ActionEvent e) {
		if(e.getSource()==action) {
			inputStr=inputText.getText();//取得输入串
			if(inputStr.length()>1)
				work3_1.distinguish();//调用分析程序
			
			outputText.setText(work3_1.stepStr); //输出
			work3_1.stepStr="";
			//outputText.append(work2_temp.stepStr);
		}
		
	}
	
}
