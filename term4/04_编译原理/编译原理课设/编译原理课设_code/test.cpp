#include <iostream>
#include <string>
#include <VECTOR>
#include <stack>

using namespace std;

struct info
{
    char left;
    vector<string> right;
    vector<char> first;
    vector<char> last;
};

vector<info> lang;

char mtr[9][9]; //算符优先矩阵
stack<char> sta;

void getGrammar(); //获取文法
void getFirstLast(); //求FirstVT 和 LastVT
void getMatrix(); //求算符优先矩阵
void start(); //开始转换

int cmp(char a, char b); //比较两个运算符的优先级
void out(string num,char now, string avg1, string avg2); //打印三元式
int ope(char op, int a, int b); //定义三元式计算方法

int main()
{
    cout << "<--- 开始载入文法 --->" << endl;
    getGrammar();
    cout << "<--- 求解FirstVT、LastVT --->" << endl;
    getFirstLast();
    cout << "<--- 求解算符优先矩阵 --->" << endl;
    getMatrix();
    cout << "<--- 开始进行解析 --->" << endl;
    start();
    return 0;
}

void getGrammar()
{
    info temp, temp1, temp2;
    temp.left = 'E';
    temp.right.push_back("E+T");
    temp.right.push_back("E-T");
    temp.right.push_back("T");
    temp.right.push_back("i");

    temp1.left = 'T';
    temp1.right.push_back("T*F");
    temp1.right.push_back("T/F");
    temp1.right.push_back("F");

    temp2.left = 'F';
    temp2.right.push_back("(E)");
    temp2.right.push_back("i");

    lang.push_back(temp);
    lang.push_back(temp1);
    lang.push_back(temp2);

    cout << "<========================================>" << endl;
    cout << "<--- 文法载入完成 --->" << endl;
    cout << endl;
    // 输出文法
    for (int i = 0; i < (int)lang.size(); i++)
    {
        for (int j = 0; j < (int)lang[i].right.size(); j++)
        {
            cout << lang[i].left << " --> ";
            cout << lang[i].right[j] << endl;
        }
    }
    cout << endl;
}

void getFirstLast()
{
    //FIRSTVT:B=>b......
    int i, j, sign = 0, sign1 = 0;
    for (i = 0; i < (int)lang.size(); i++)
    {
        for (j = 0; j < (int)lang[i].right.size(); j++)
        {
            string temp = lang[i].right[j]; //获取右部

            if (temp[0] > 'Z' || temp[0] < 'A')
            { //终结符
                lang[i].first.push_back(temp[0]);
            }

            else if (temp.length() >= 2)
            { //终结符
                if (temp[1] > 'Z' || temp[1] < 'A')
                {
                    lang[i].first.push_back(temp[1]);
                }
            }
        }
    }
    //LASTVT:B=>.....a
    for (i = 0; i < (int)lang.size(); i++)
    {
        for (j = 0; j < (int)lang[i].right.size(); j++)
        {
            string temp = lang[i].right[j]; //获取右部

            if ((temp[0] > 'Z' || temp[0] < 'A') && temp.length() == 1)
            { //终结符
                lang[i].last.push_back(temp[0]);
            }
            else if (temp.length() >= 3)
            { //终结符
                if (temp[1] > 'Z' || temp[1] < 'A')
                    lang[i].last.push_back(temp[1]);

                else if (temp[2] > 'Z' || temp[2] < 'A') //终结符
                    lang[i].last.push_back(temp[2]);
            }
        }
    }
    //FIRSTVT:B=>Cb......
    while (sign == 0)
    { //迭代FirstVT
        sign = 1;
        for (i = 0; i < (int)lang.size(); i++)
        {
            for (j = 0; j < (int)lang[i].right.size(); j++)
            {
                string temp = lang[i].right[j]; //获取右部
                if (temp.length() == 1 && (temp[0] <= 'Z' && temp[0] >= 'A'))
                { //可以迭代
                    for (int k = 0; k < (int)lang.size(); k++)
                    {
                        if (lang[k].left == temp[0])
                        { //找到了，添加元素
                            for (int p = 0; p < (int)lang[k].first.size(); p++)
                            {
                                sign1 = 0;
                                char ch = lang[k].first[p];
                                for (int q = 0; q < (int)lang[i].first.size(); q++)
                                {
                                    if (lang[i].first[q] == ch)
                                    { //包含了
                                        sign1 = 1;
                                    }
                                }
                                if (sign1 == 0)
                                {
                                    lang[i].first.push_back(ch);
                                    sign = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    //LASTVT:B=>......aC
    sign = 0;
    while (sign == 0)
    { //迭代LastVT
        sign = 1;
        for (i = 0; i < (int)lang.size(); i++)
        {
            for (j = 0; j < (int)lang[i].right.size(); j++)
            {
                string temp = lang[i].right[j]; //获取右部

                if (temp.length() == 1 && (temp[0] <= 'Z' && temp[0] >= 'A'))
                { //可以迭代
                    for (int k = 0; k < (int)lang.size(); k++)
                    {
                        if (lang[k].left == temp[0])
                        { //找到了，添加元素
                            for (int p = 0; p < (int)lang[k].last.size(); p++)
                            {
                                sign1 = 0;
                                char ch = lang[k].last[p];
                                for (int q = 0; q < (int)lang[i].last.size(); q++)
                                {
                                    if (lang[i].last[q] == ch)
                                    { //包含了
                                        sign1 = 1;
                                    }
                                }

                                if (sign1 == 0)
                                {
                                    lang[i].last.push_back(ch);
                                    sign = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    
    cout << "<========================================>" << endl;
    cout << "<--- 求解FirstVT、LastVT成功 --->" << endl;
    // 输出firstVT和LastVT
    cout << "FirstVT:" << endl;
    for (i = 0; i < (int)lang.size(); i++)
    {
        cout << lang[i].left << " : ";
        for (j = 0; j < (int)lang[i].first.size(); j++)
        {
            cout << lang[i].first[j] << "  ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "LastVT:" << endl;
    for (i = 0; i < (int)lang.size(); i++)
    {
        cout << lang[i].left << " : ";
        for (j = 0; j < (int)lang[i].last.size(); j++)
        {
            cout << lang[i].last[j] << "  ";
        }
        cout << endl;
    }
    cout << endl;
}

void getMatrix()
{
    int i, j;
    for (i = 0; i < 9; i++)
    { //初始化
        for (j = 0; j < 9; j++)
        {
            mtr[i][j] = 'n';
        }
    }

    string temp = "+-*/()i#";
    for (i = 1; i < 9; i++)
    {
        mtr[i][0] = temp[i - 1];
        mtr[0][i] = temp[i - 1];
    }
    vector<string> str;

    //aU==>a < FirstVT(U)
    for (i = 0; i < (int)lang.size(); i++)
    { //aU   a < FirstVT(U)
        for (j = 0; j < (int)lang[i].right.size(); j++)
        {
            string ss = lang[i].right[j];
            string ok = "";
            if (ss.length() > 2)
            {
                if ((ss[0] > 'Z' || ss[0] < 'A') && (ss[1] <= 'Z' && ss[1] >= 'A'))
                { //aU
                    ok = "";
                    ok += ss[0];
                    ok += ss[1];
                    str.push_back(ok);
                }
                if ((ss[1] > 'Z' || ss[1] < 'A') && (ss[2] <= 'Z' && ss[2] >= 'A'))
                { //aU
                    ok = "";
                    ok += ss[1];
                    ok += ss[2];
                    str.push_back(ok);
                }
            }
        }
    }

    //找到a和FIRSTVT(U)
    for (i = 0; i < (int)str.size(); i++)
    {
        for (j = 1; j < 9; j++)
        {
            if (mtr[j][0] == str[i][0])
            {
                for (int k = 0; k < (int)lang.size(); k++)
                {
                    if (lang[k].left == str[i][1])
                    {
                        for (int p = 0; p < (int)lang[k].first.size(); p++)
                        {
                            for (int q = 1; q < 9; q++)
                            {
                                if (mtr[q][0] == lang[k].first[p])
                                {
                                    mtr[j][q] = '<';
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //Ua==>LastVT(U) > a
    str.clear();
    for (i = 0; i < (int)lang.size(); i++)
    {
        for (j = 0; j < (int)lang[i].right.size(); j++)
        {
            string ss = lang[i].right[j];
            string ok = "";
            if (ss.length() > 2)
            {
                if ((ss[1] > 'Z' || ss[1] < 'A') && (ss[0] <= 'Z' && ss[0] >= 'A'))
                { //Ua
                    ok = "";
                    ok += ss[0];
                    ok += ss[1];
                    str.push_back(ok);
                }

                if ((ss[2] > 'Z' || ss[2] < 'A') && (ss[1] <= 'Z' && ss[1] >= 'A'))
                { //Ua
                    ok = "";
                    ok += ss[1];
                    ok += ss[2];
                    str.push_back(ok);
                }
            }
        }
    }

    //找到a和LASTVT(U)
    for (i = 0; i < (int)str.size(); i++)
    {
        for (j = 1; j < 9; j++)
        {
            if (mtr[0][j] == str[i][1])
            { //Find a Then Find LastVt(U)

                for (int k = 0; k < (int)lang.size(); k++)
                {
                    if (lang[k].left == str[i][0])
                    { //Find U

                        for (int p = 0; p < (int)lang[k].last.size(); p++)
                        {
                            for (int q = 1; q < 9; q++)
                            {
                                if (mtr[0][q] == lang[k].last[p])
                                {
                                    mtr[q][j] = '>';
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //aUb || ...ab...  ==> a = b
    str.clear();
    for (i = 0; i < (int)lang.size(); i++)
    { //ab  aUb  a = b
        for (j = 0; j < (int)lang[i].right.size(); j++)
        {
            string ss = lang[i].right[j];
            string ok = "";
            if (ss.length() > 2)
            {
                if ((ss[1] > 'Z' || ss[1] < 'A') && (ss[0] > 'Z' || ss[0] < 'A'))
                { //aa
                    ok = "";
                    ok += ss[0];
                    ok += ss[1];
                    str.push_back(ok);
                }

                if ((ss[2] > 'Z' || ss[2] < 'A') && (ss[1] > 'Z' || ss[1] < 'A'))
                { //aa

                    ok = "";
                    ok += ss[1];
                    ok += ss[2];
                    str.push_back(ok);
                }

                if ((ss[2] > 'Z' || ss[2] < 'A') && (ss[0] > 'Z' || ss[0] < 'A'))
                { //aUa

                    ok = "";
                    ok += ss[0];
                    ok += ss[2];
                    str.push_back(ok);
                }
            }
        }
    }

    for (i = 0; i < (int)str.size(); i++)
    {
        for (j = 1; j < 9; j++)
        {
            if (str[i][0] == mtr[j][0])
            {
                for (int k = 1; k < 9; k++)
                {
                    if (mtr[0][k] == str[i][1])
                    {
                        mtr[j][k] = '=';
                    }
                }
            }
        }
    }

    for (i = 0; i < (int)lang[0].first.size(); i++)
    { //#

        for (j = 1; j < 9; j++)
        {
            if (lang[0].first[i] == mtr[0][j])
            {
                mtr[8][j] = '<';
            }
        }
    }

    for (i = 0; i < (int)lang[0].first.size(); i++)
    { //#

        for (j = 1; j < 9; j++)
        {
            if (lang[0].first[i] == mtr[j][0])
            {
                mtr[j][8] = '>';
            }
        }
    }
    mtr[8][8] = '=';

    //输出矩阵
    cout << "<========================================>" << endl;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (mtr[i][j] != 'n')
                cout << mtr[i][j] << "\t";
            else
                cout << "\t";
        }
        cout << endl;
    }
    cout << "<========================================>" << endl;
    cout << endl;
}

void start()
{
    string str;
    int i,j=0;

    stack<string> data;
    stack<int> flag;
    stack<char> op;

    char now = 'n'; //记录当前栈顶操作符
    int sign = 0;

    cout << "<========================================>" << endl;
    cout << "请输入算术表达式：" << endl;
    cin >> str;

    str += '#';
    op.push('#');

    for (i = 0; i < (int)str.length(); i++)
    {
        sign = 0;
        int number = 0;
        int tmp = i;
        //操作数
        while (str[tmp] >= '0' && str[tmp] <= '9')
        { 
            number = number * 10;
            number += str[tmp] - '0';
            if(str[tmp+1] <= '0' || str[tmp+1] >= '9')
            {
                string temp = to_string(number);
                data.push(temp);
                number = 0;
                i = tmp;
            }
            tmp++;
        }
        
        //运算符
        if(str[i] <= '0' || str[i] >= '9')
        { 
            op.push(str[i]);
            sign = 1;
        }
        //有可比性,并且操作符栈有更新
        if (now != 'n' && sign == 1)
        { 
            if (!op.empty())
            {
                char top = op.top(); //栈顶元素

                while (cmp(now, top) == 1)
                { //需要规约
                    j++;
                    string avg1 = data.top();
                    data.pop();
                    string avg2 = data.top();
                    data.pop();
                    string num = "(" + to_string(j) + ")";
                    out(num,now, avg2, avg1); //打印三元式
                    // data.push(ope(now, avg2, avg1));
                    
                    data.push(num);
                    op.pop();
                    op.pop();

                    if (!op.empty())
                    {
                        now = op.top();
                    }

                    else
                    {
                        now = 'n';
                    }

                    op.push(top);
                }

                if (cmp(now, top) == 0)
                {
                    op.pop();
                    op.pop();

                    if (!op.empty())
                    {
                        now = op.top();
                    }
                    else
                    {
                        char temp = '=';
                        if (!data.empty())
                        {
                            string da = data.top();
                            string num = "(" + to_string(j) + ")";
                            out(num,temp, da, "0");
                        }
                    }
                }
            }
        }
        //不需要比较
        else
        { 
            if (!op.empty())
            {
                now = op.top();
            }
        }
    }
}
//比较优先级
int cmp(char a, char b)
{
    int i, j;
    for (i = 1; i < 9; i++)
    {
        if (mtr[i][0] == a)
        {
            for (j = 1; j < 9; j++)
            {
                if (mtr[0][j] == b)
                {
                    if (mtr[i][j] == '>')
                    {
                        return 1;
                    }
                    else if (mtr[i][j] == '=')
                    {
                        return 0;
                    }
                    else if (mtr[i][j] == '<')
                    {
                        return -1;
                    }
                }
            }
        }
    }
    return 2;
}
//计算结果
int ope(char op, int a, int b)
{
    if (op == '+')
        return a + b;
    if (op == '-')
        return a - b;
    if (op == '*')
        return a * b;
    if (op == '/')
        return a / b;
    if (op == '=')
        return a;

    return 0;
}
//输出
void out(string num,char now, string avg1, string avg2)  
{

    cout << "----------------------------------------" << endl;
    cout << num << "  ( " << now << ",\t" << avg1 << ",\t" << avg2 << " )" << endl;        

}