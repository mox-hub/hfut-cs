/**
 * @author 
 * @date 2020_9_21
 * @version 1.0
 * @property it is the first program of robot homework
 */

//ͷ�ļ�����
#include <iostream>
#include <fstream>
#include <stack>
#include <string.h>
#include <stdlib.h>
#include <vector>

//ʹ�ñ�׼�����ռ�
using namespace std;
//��������
#define MAX_MSG 256

//�����ı�ͷ
/* ������Ϣ���ж���Ϣ���ͽ��з���ĺ���������see��hear��analyze��������*/
void handleMessage();
/* robot��hear�������������ڴ���hear�������ַ���*/
void analyzeHearMessage(string strMsg);
/* robot��see�������������ڴ���see�������ַ���*/
void analyzeSeeMessage(string strMsg);
/*�ַ�����������split*/
const vector<string> split(const string& s, const char& c)
{
    string buff{ "" };
    vector<string> v;

    for (auto n : s)
    {
        if (n != c) buff += n; else
            if (n == c && buff != "") { v.push_back(buff); buff = ""; }
    }
    if (buff != "") v.push_back(buff);

    return v;
}

/*main ������ʼ*/
int main(int argc, char const* argv[])
{
    cout << "******      program is going      ******" << endl;
    handleMessage();
    cout << "******      program is end        ******" << endl;
    return 0;
}

void handleMessage()
{
    char strBuf[MAX_MSG] = "(hear 1022 -30 passto(23,24))(see 1022 ((ball) -20 20 1 -2) ((player hfut1 2) 45 23 0.5 1 22 40 ) ((goal r) 12 20) ((Line r) - 30))";   //������
    int flag = 0;
    int first = 0,last = 0;
    stack<char> StackTemp; // 

    cout << "string : "<< strBuf << endl;
    cout << "stringLength: "<< strlen(strBuf) << endl << endl;
    string str(strBuf);
    for (int i = 0; i < strlen(strBuf); i++)
    {
        if (strBuf[i] == '(') { flag++; }
        else if (strBuf[i] == ')') { flag--; }
        if (flag == 0)
        {
            last = i+1;
            string substr = str.substr(first, last);
            //cout << substr << endl;
            if (substr[1] == 's')
            {
                //cout << "******  Start analyze seeMsg  ******" << endl;
                analyzeSeeMessage(substr);
            }
            else if (substr[1] == 'h')
            {
                //cout << "******  Start analyze hearMsg  ******" << endl;
                analyzeHearMessage(substr);
            }
            first = last;
        }
    }
}

void analyzeHearMessage(string strMsg)
{
    int hearPeriod;
    int hearSender;
    string hearMessage;

    strMsg.erase(0, 1);
    strMsg.erase(strMsg.length()-1, 1);
    vector<string> result{ split(strMsg,' ') };
    //�����ݸ�ֵ
    hearPeriod = atoi(result.at(1).c_str());
    hearSender = atoi(result.at(2).c_str());
    hearMessage = result.at(3);

    cout << "��" << hearPeriod << "���� hear ��" << hearSender << "���� ������" << hearMessage << endl;
    //cout << "******  End analyze hearMsg  ******" << endl;    
}

void analyzeSeeMessage(string strMsg)
{
    int seePeriod;
    vector<string> seeMessage;
    vector<string> result;
    int flag = 0;
    int first = 0, last = 0;
    char seeMsg[MAX_MSG];

    strMsg.erase(0, 1);
    strMsg.erase(strMsg.length() - 1, 1);
    strcpy_s(seeMsg, strMsg.c_str());
    for (int i = 0; i < strlen(seeMsg); i++)
    {
        if (seeMsg[i] == '(') { flag++; }
        else if (seeMsg[i] == ')') { flag--; }
        if (flag == 0 && seeMsg[i] == ' ')
        {
            last = i;
            string substr = strMsg.substr(first, last - first);
            result.push_back(substr);
            first = last + 1;
        }
    }
    seePeriod = atoi(result.at(1).c_str());
    cout << "��" << seePeriod << "���� see";
    string str01;
    for (int i = 3; i != result.size(); ++i)
    {
        string str01 = result[i];
        char str02[MAX_MSG];
        int flag = 0;
        int first = 0, last = 0;
        str01.erase(0, 1);
        str01.erase(strMsg.length() - 1, 1);
        strcpy_s(str02, str01.c_str());
        vector<string> subresult;
        result[i].erase(0, 1);
        result[i].erase(result[i].length() - 1, 1);
        vector<string> subResult;
        for (int i = 0; i < strlen(str02); i++)
        {
            if (str02[i] == '(') { flag++; }
            else if (str02[i] == ')') { flag--; }
            if (flag == 0 && str02[i] == ' ')
            {
                last = i;
                string substr = str01.substr(first, last - first);
                subResult.push_back(substr);
                first = last + 1;
            }
        }
        if (5 == subResult.size())
        {
            cout << subResult.at(1) << "�����ҵ�Direction��" << subResult.at(2) << ",Distance��" << subResult.at(3) << "DistChng��" 
                << subResult.at(4) << "��DirChng��" << subResult.at(5);
        }
        if (7 == subResult.size())
        {
            cout << subResult.at(1) << "�����ҵ�Direction��" << subResult.at(2) << ",Distance��" << subResult.at(3) << "DistChng��" 
                << subResult.at(4) << "��DirChng��" << subResult.at(5)<< "���� BodyDir ��" << subResult.at(6) << "�� HeadDir ��" << subResult.at(7);
        }
        if (3 == subResult.size())
        {
            cout << subResult.at(1) << "�����ҵ�Direction��" << subResult.at(2) << ",Distance��" << subResult.at(3);
        }
        if (2 == subResult.size())
        {
            cout << subResult.at(1) << "�����ҵ�Distance��" << subResult.at(2);
        }
    }
    //cout << "******  End analyze seeMsg  ******" << endl;
}
