//引用头文件
#include<iostream>
#include<string.h> 
#include<string>
#include<map>
#include<list>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <math.h>
#include <fstream>

//使用命名空间std
using namespace std;

//宏定义
#define Feature 100                         //特征数（feature）+1的上限 
#define Sample  7000                        //样本数量的 上限 
#define FeatureChoice 50                    //属性 取值种类 的上限 

//结构体定义
struct Node                 //决策树节点 
{
    string attribute;       //特征名
    string val;             //特征值
    bool isLeaf;            //是否为叶子节点（叶子节点判断终止）
    vector<Node*> childs;   //孩子节点
    //构造函数，设定初始值
	Node()
	{
		val = "";           //初始特征值
		attribute = "";     //初始特征名
		isLeaf = false;     //默认不是叶子节点
	}
};

//全局变量 
vector< vector<string> > trainData;         //创建训练数据集
vector< vector<string> > testData;          //创建实验数据集
vector< vector<string> > update;            //创建缓存更新数据集--用于创建决策树
vector<string> attributes;                  //创建特征
Node *root = NULL;                          //创建决策树的根节点

//全局函数
void DTA_createTrainDataset();                                                                      //从文本文件"train.data"读取文本信息，然后创建训练用数据集
void DTA_createTestDataset();                                                                       //从文本文件"test.data"读取文本信息，然后创建训练用数据集
double DTA_calcEntropy(vector< vector<string> > &data);                                             //计算给定数据集的信息熵
vector< vector<string> > DTA_splitDataSet(vector< vector<string> > data, int axis, string value);   //按照给定特征划分数据集，新的数据集的维度少了一个/axis ：特征下标/value：特征值
vector<string> DTA_createFeatureList(vector< vector<string> > &data, int axis);                     //创建特征列表
int DTA_chooseBestFeatureToSplit(vector< vector<string> > &data);                                   //选择最好的数据集划分方式
string DTA_majorityCnt(vector<string> &classList);                                                  //返回出现次数最多的分类名称/如果类标签依不唯一，采用多数表决的方法定义叶子节点的分类
Node* DTA_createTree(Node *root, vector< vector<string> > &data, vector<string> &attribute);        //递归构建决策树
void DTA_printTree(Node *root, int depth);                                                          //打印决策树到“Tree.txt”中 
void DTA_freeNode(Node *root);                                                                      //释放节点
string DTA_classify(Node *root, vector<string> &attribute, vector<string> &test);                   //预测一组数据的结果 
void DTA_printResult(vector< vector<string> > &data,Node* root);									//打印结果到“result.txt"中												//将结果打印到“result.txt”中 
//函数实现

//从文本文件"train.data"读取文本信息，然后创建训练用数据集
void DTA_createTrainDataset()
{
    int i,j;           //设置循环变量
	int num;
    char p,*p2;
    string line;
    string word;
	vector<string> temp_line;
    ifstream fin("train.data");
    
    //首先获取第一行作为属性
    getline(fin,line);
    //分割成单词
    for (string::size_type i = 0;i < line.size(); i++)
    {
        if(line[i]=='\t')
		{
			if(!word.empty())
			{
				temp_line.push_back(word);
				word = "";
			}
		}
		else
			word += line[i];
    }
	temp_line.push_back(word);
    word = "";
	for(i = 0; i <temp_line.size(); ++i)
	{
		attributes.push_back(temp_line[i]);
	} 
	temp_line.clear();

	while (fin.get(p))
	{
        getline(fin,line);
//        string::size_type 
        for (i = 0;i < line.size(); i++)
        {
            if(line[i]=='\t')
			{
				if(!word.empty())
				{
					temp_line.push_back(word);
					word = "";
				}
			}
			else
				word += line[i];
        }

        temp_line.push_back(word);
        word = "";
        trainData.push_back(temp_line); 
		temp_line.clear();
    }
	fin.close();               //所以需要将最后一行的数据再填入到二维动态数组中

    for(i = 0; i <attributes.size(); ++i)
	{
		cout << attributes[i] << "\t";
	}
    cout << endl;

    for (i = 0;i < trainData.size(); ++i)
    {
        for (j = 0;j < trainData[0].size(); ++j)
        {
            cout << trainData[i][j] << "\t";
        }
        cout << endl;
    }
}

//从文本文件"test.data"读取文本信息，然后创建训练用数据集
void DTA_createTestDataset()
{
    int i,j;           //设置循环变量
	int num;
    char p;
    string line;
    string word;
	vector<string> temp_line;
    ifstream fin("test.data");
	
	getline(fin,line); 
    while (fin.get(p))
	{
        getline(fin,line);
        for (string::size_type i = 0;i < line.size(); ++i)
        {
            if(line[i]==' ' || line[i]=='\t')
			{
				if(!word.empty())
				{
					temp_line.push_back(word);
					word = "";
				}
			}
			else
				word += line[i];
        }
        temp_line.push_back(word);
        word = "";
        testData.push_back(temp_line); 
		temp_line.clear();
    }
	fin.close();               //所以需要将最后一行的数据再填入到二维动态数组中
	
    for (i = 0;i < testData.size(); ++i)
    {
        for (j = 0;j < testData[0].size(); ++j)
        {
            cout << testData[i][j] << "\t";
        }
        cout << endl;
    }
}

//计算给定数据集的信息熵
double DTA_calcEntropy (vector< vector<string> > &data)
{
	 int n = data.size();                   //设置循环范围
	 int i;									//循环变量 
	 map<string, int> classCounts;          //设置键值对 key -- value 使用map
	 int label = data[0].size() - 1;        //去除每一行最后一个的坐标
	 for(i = 0; i < n; i++)             //初始为0
	 {
		classCounts[ data[i][label] ] = 0;
	 }
	 for(i = 0; i < data.size(); i++)  //每当出现一次，+1
	 {
		classCounts[ data[i][label] ] += 1;
	 }

     /*计算信息熵
    信息熵计算方法： Entropy(D) = -sum( p_i * log_2(p_i) )   
                    D为样本集合
                    1<= i <= 样本D中所包含的类别数
                    p_i 为第i个类别在D中所占的比例
     */
	 double entropy = 0;                     //信息熵
	 map<string, int>::iterator it;         //迭代器遍历map
	 for(it = classCounts.begin(); it != classCounts.end(); it++)
	 {
		 double prob = (double)(it->second) / (double)n;
		 entropy -= prob * ( log(prob) / log(2) );
	 }
	//	cout << "shanno is " << entropy << endl;
	//  cout << "TA_calcEntropyt is completed" << endl;
	 return entropy;
}

//按照给定特征划分数据集，新的数据集的维度少了一个 /axis ：特征下标 /value：特征值
vector< vector<string> > DTA_splitDataSet(vector< vector<string> > data, int axis, string value)
{
	vector< vector<string> > result;        //新建vector储存result
	for(int i = 0; i < data.size(); i++)
	{
		if(data[i][axis] == value)
		{
			//将“当前特征”这个维度去掉
			vector<string> removed(data[i].begin(), data[i].begin()+axis);
			//用法：在指定位置it前“插入”区间[start, end)的所有元素.
            removed.insert(removed.end(), data[i].begin()+axis+1, data[i].end());
			result.push_back(removed);
		}
	}
	// cout << "DTA_splitDataSet is completed" << endl; 
	return result;
}
//创建特征列表
vector<string> DTA_createFeatureList(vector< vector<string> > &data, int axis)
{
	int n = data.size();
	vector<string> featureList;     //特征的所有取值
	set<string> s;                  //通过set进行去重操作
	for(int j = 0; j < n; j++)      //寻找该特征的所有可能取值
		s.insert(data[j][axis]);
	set<string>::iterator it;       //之后两步将set集合压入featureList
	for(it = s.begin(); it != s.end(); it++)
	{
		featureList.push_back(*it);
	}
	// cout << "DTA_createFeatureList is completed" << endl;
	return featureList;
}

//选择最好的数据集划分方式
int DTA_chooseBestFeatureToSplit(vector< vector<string> > &data)
{
	int n = data[0].size() - 1;                  //设置循环区间
	double bestEntropy = DTA_calcEntropy(data);      //初始信息熵
	double bestInfoGain = 0;                     //最大的信息增益
	int bestFeature = 0;                         //最好的特征
	for(int i = 0; i < n; i++)                   //所有特征
	{
		double newEntropy = 0;                   //新的特征信息熵
		vector<string> featureList = DTA_createFeatureList(data, i);  //该特征的所有可能取值
		for(int j = 0; j < featureList.size(); j++)
		{
            //去除该特征
			vector< vector<string> > subData = DTA_splitDataSet(data, i, featureList[j]);
			double prob = (double)subData.size() / (double)data.size();
			newEntropy += prob * DTA_calcEntropy(subData);   
		}
		double infoGain = bestEntropy - newEntropy;  //信息增益，即熵的减少，或数据无序度的减少
        //选择最优的信息增益
		if(infoGain > bestInfoGain)
		{
			bestInfoGain = infoGain;
			bestFeature = i;
		}
	}
	// cout << "DTA_chooseBestFeatureToSplit is completed" << endl;
	// cout << "the bestFeature is " << bestFeature << endl;
	return bestFeature;

}

//返回出现次数最多的分类名称
//如果类标签依然不是唯一的，采用多数表决的方法定义叶子节点的分类
string DTA_majorityCnt(vector<string> &classList)
{
	int n = classList.size();       //设置循环范围
	map<string, int> classCount;    //创建classCount的key -- value键值对，用来投票计数
	int i;                          //循环变量i

    //初始化
	for(i = 0; i < n; i++)
	{
		classCount[classList[i]] = 0;
	}
    //投票计数
	for(i = 0; i < n; i++)
	{
		classCount[classList[i]] += 1;
	}

    //多数投票算法（Majority Vote Algorithm）
    int DTA_majorityCnt = 0;            //最多票数计数器
    map<string, int>::iterator it;  //设置迭代器
    string result = "";             //初始化result

    for(it = classCount.begin(); it != classCount.end(); it++)
    {
        if(it->second > DTA_majorityCnt)
        {
            //it->first为键值；
            //it->second为数值；
            DTA_majorityCnt = it->second;
            result = it->first;
        }
    }
	// cout << "DTA_majorityCnt is complete" << endl;
	// cout << "result is " << result << endl;
    return result;
}

//递归构建决策树（核心算法）
Node* DTA_createTree(Node *root, vector< vector<string> > &data, vector<string> &attribute)
{
	if(root == NULL)                    //新建立根结点root
		root = new Node();
	vector<string> classList;           //特征列表
	set<string> classList_removal;      //去重特征列表
	int i, j;                           //循环变量
	int label = data[0].size() - 1;     //设置循环范围
	int n = data.size();                //数据集的大小
    //TODO: move the n to the beginning as a global value(is ok)
	for(i = 0; i < n; i++)
	{
		classList.push_back(data[i][label]);
		classList_removal.insert(data[i][label]);
	}
	if(classList_removal.size() == 1)   //如果所有实例都属于同一类，停止划分
	{
		if(classList[0] == "yes")
			root->attribute = "yes";
		else
			root->attribute = "no";
		root->isLeaf = true;
		return root;
	}
	if(data[0].size() == 1)             //遍历完所有特征，采用多数表决返回出现次数最多的类别
	{
		root->attribute = DTA_majorityCnt(classList);
		return root;
	}

	int bestFeatureIndex = DTA_chooseBestFeatureToSplit(data);                   //选择最好的方式进行划分
	vector<string> featureList = DTA_createFeatureList(data, bestFeatureIndex);  //得到该属性的所有可能值
	string bestFeature = attribute[bestFeatureIndex];                            //记录最好的属性
 
	root->attribute = bestFeature;          //记录要划分的属性
 
	for(i=0; i<featureList.size(); i++)     //对于当前属性的每个可能值，创建新的分支
	{
		vector<string> subAttribute;        //新的属性列表，不包含当前要划分的属性，特征减少一个
		for(j=0; j<attribute.size(); j++)
		{
			if(bestFeature != attribute[j])
				subAttribute.push_back(attribute[j]);
		}
		Node *newNode = new Node();         //新建节点
		newNode->val = featureList[i];      //记录属性的取值

        //递归创建决策树
        update = DTA_splitDataSet(data, bestFeatureIndex, featureList[i]);
		DTA_createTree(newNode, update, subAttribute);
		root->childs.push_back(newNode);    //为父节点添加child
	}
	return root;
}

//打印决策树 
void DTA_printTree(Node *root, int depth)
{
    ofstream fout("DecisionTree.txt",ios::app);		//使用追加的方式打开文件 
	int i;              //循环变量i
	//输出算法 
	for(i = 0; i < depth; i++)
		fout << "\t";
	
	if(root->val != "")
	{
		fout << root->val << endl;
		for(i = 0; i < depth+1; i++)
			fout << "\t";
	}
	fout << root->attribute << endl;
	fout.close();
	//清空缓存区，防止未录入 
	//迭代器输出树 
	vector<Node*>::iterator it;
	for(it = root->childs.begin(); it != root->childs.end(); it++)
	{
		DTA_printTree(*it, depth+1);
	}
	// cout << "DTA_printTree is completed" << endl;
}
string DTA_classify(Node *root, vector<string> &attribute, vector<string> &test)
{
	string firstFeature = root->attribute;
	int firstFeatureIndex;
	int i;
	for(i=0; i<attributes.size(); i++)  //找到根节点是第几个特征
	{
		if(firstFeature == attribute[i])
		{
			firstFeatureIndex = i;
			break;
		}
	}
	if(root->isLeaf)  //如果是叶子节点，直接输出结果
		return root->attribute;
	for(i=0; i<root->childs.size(); i++)
	{
		if(test[firstFeatureIndex] == root->childs[i]->val)
		{
			return DTA_classify(root->childs[i], attribute, test);
		}
	}
}

//将结果打印到“result.txt”中 
void DTA_printResult(vector< vector<string> > &data,Node* root)
{
	vector<string> temp;
	string result;
	int i = 1;
	try{
		vector<vector<string> > ::iterator iter;
		for(iter = data.begin(); iter != data.end(); ++iter)
		{
			ofstream fout("result.txt",ios::app);
			temp = *iter;
			result = DTA_classify(root,attributes,temp);
			if(result != "no")
				result = "yes"; 
	 		fout << "第" << i << "个测试的结果为：" << result << endl;		
	 		i++;
	 		fout.close();
		}				
	} catch(std::bad_alloc)
	{
		cout << "bad_alloc happened" << endl;
	} 
}
//释放节点

void DTA_freeNode(Node *root)
{
	if(root == NULL)
		return;
	vector<Node*>::iterator it;
	for(it=root->childs.begin(); it != root->childs.end(); it++)
		DTA_freeNode(*it);
	delete root;
}

//main函数
int main(void)
{
//	测试printResult的代码
//	int i;				//循环 
//	string result;		
//	vector <string> test;
//		test.push_back("雨");
//		test.push_back("冷");
//		test.push_back("正常");
//		test.push_back("无");
    DTA_createTrainDataset();
	cout << "DTA_createTrainDataset is completed" << endl;
    DTA_createTestDataset();
	cout << "DTA_createTestDataset is completed" << endl;
	//获取root 
	root = DTA_createTree(root, trainData, attributes);
	cout << "DTA_createTree is completed" << endl;
	DTA_printTree(root, 0);
	cout << "DTA_printTree is completed" << endl;
//	result = DTA_classify(root, attributes, test);
//	cout << "测试的结果为：" << result << endl;
	DTA_printResult(testData,root);
	cout << "DTA_printResult is completed" << endl;
	DTA_freeNode(root);
	cout << "DTA_freeNode is completed" << endl;

    return 0;
}
