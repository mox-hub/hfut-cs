#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
using namespace std;

void getData();
void analyze(string strMsg);
void calculate(vector<vector<string>> result);

//point class 
class point 
{
    public:
        double x;
        double y;
    public:
        point(double in_x, double in_y)
        {
            x = in_x;
            y = in_y;
        }
        void show()
        {
            cout << "(" << x << "," << y << ")" << endl;
        }
};
    //set global variable
    point C(0, 0);
    point p1(-52.5, -32), p2(-52.5, 32), p3(52.5, 32), p4(52.5, -32), p5(0, -32);
    point p6(0, 32), p7(-30, -7), p8(-30, 7), p9(30, 7), p10(30, -7);
    point G1(-52.5, 0), G2(52.5, 0);
//tool of split
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

void getData()
{
    char strBuf[256];
    ifstream in("test.txt");
    if (! in.is_open())
    {
        cout << "Error" << endl;
        exit(1);
    }
    while (!in.eof())
    {
        in.getline(strBuf, 256);
        // cout << strBuf << endl;
        string strMsg = strBuf;
        analyze(strMsg);
    }   
}

//get data from txt "test"
void analyze(string strMsg)
{
    vector<vector<string>> result;
    int flag = 0;
    int last = 0, first = 0;
    char* strBuf = (char*)strMsg.c_str();
    for (int i = 0; i < strlen(strBuf); i++)
    {
        if (strBuf[i] == '(') { flag++; }
        else if (strBuf[i] == ')') { flag--; }
        if (flag == 0)
        {
            last = i+1;
            string substr = strMsg.substr(first, last - first);
            if(!substr.empty())
            {
                substr.erase(0, 1);
                substr.erase(substr.length()-1, 1);
                // cout << substr << endl;
                vector<string> subresult{split(substr,' ') };
                result.push_back(subresult);
            }
            first = last;
        }
    }
    calculate(result);
}

//the part of math calculate 
void calculate(vector<vector<string>> result)
{
    double x1 = 0, y1 = 0;
    double x2 = 0, y2 = 0;
    double r1 = atof(result[0][1].c_str());
    double r2 = atof(result[1][1].c_str());
    double alpha1 = atof(result[0][2].c_str());
    double alpha2 = atof(result[1][2].c_str());
    int Sign = 1;
    /*jurge the signal*/
    if (alpha1 -alpha2 < 0 )
    {
        Sign = -1;
    }
    // cout << result[0][0] << endl;
    // cout << result[1][0] << endl;
    if(result[0][0] == "P8")
    {
         
        x1 = p8.x; 
        y1 = p8.y;
    }
    if(result[0][0] == "C")
    {
        x1 = C.x;
        y1 = C.y;
    }
    if(result[1][0] == "P7")
    {
        x2 = p7.x;
        y2 = p7.y;
    }    
    if(result[1][0] == "P5")
    {
        x2 = p5.x;
        y2 = p5.y;
    }
    cout << "y1 = " <<y1<< endl;
    cout << "y2 = " <<y2<< endl;
    double dx = x1 - x2;
    double dy = y1 - y2;
    double d = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    // cout << d << endl; 
    double cosa = dx / d;
    double sina = dy / d;

    double a = (r1 * r1 - r2 * r2 + d * d) / (2 * d);
     cout << a << endl;
    double h = sqrt(r1 * r1 - a * a);
    cout << h << endl;


    double x = x1 + a * cosa - h * Sign * sina;
    double y = y1 - a * sina + h *Sign *cosa;

    // cout << "dx = " << dx << endl; 
    // cout << "dy = " << dy << endl;
    
    //the output
    cout << "(" << x << "," << y << ")" << endl;

}

//the start of main
int main(void)
{
    //start at getData
    getData();

    return 0;
}