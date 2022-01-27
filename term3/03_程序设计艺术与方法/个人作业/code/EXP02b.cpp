/**
 * @author: Li Haporan 
 * @date: 2020.10.20
 * @details: 实验二 搜索算法的实现
*/

#include<iostream>
#include<iomanip>

using namespace std;

//define count to record step
int count = 0; 
//initialize
int chessBoard[8][8] ={0};
//check is knight out 
bool isKnightOut(int x, int y)  
{
    if (x >= 0 && x <= 7 && y >= 0 && y <= 7)
        return false;
    else
        return true;
}
//check is visited
bool isVisited(int x, int y) 
{
    if (chessBoard[x][y] != 0)
        return true;
    else
        return false;
}

void dfs(int x, int y)
{
    // traverse is over
    if (count == 64) 
        return;
    //if board is unvisited and knight is not out
    else if ( !isVisited(x, y) && !isKnightOut(x, y) )
    {
        count++;
        // record the step
        chessBoard[x][y] = count; 

        dfs(x + 2, y + 1);	
        dfs(x - 2, y - 1);  
        dfs(x + 2, y - 1);
        dfs(x - 2, y + 1);
        dfs(x - 1, y - 2);
        dfs(x + 1, y - 2);
        dfs(x + 1, y + 2);
        dfs(x - 1, y + 2);

        return;
    }
    // if knight out, return
    else  
        return;
}
int main()
{
    //define start point 
    int x, y;
    cout << "please input the start x, y:";
    //check input right
    while (1)
    {
        cin >> x >> y;    //输入坐标
        if (x > 7 || x < 0 || y> 7 || y < 0)
            cout << "the start point is wrong,please inptu again:" << endl;
        else
            break;
    }
    // start travel
    dfs(x, y);
    //print result
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            cout << setw(2) << chessBoard[i][j] << "  ";
        }
        cout << endl;
    }
    return 0;
}

