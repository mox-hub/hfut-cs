/**
 * @author: Li Haporan 
 * @date: 2020.10.20
 * @details: 实验二 搜索算法的实现
*/
#include <iostream>

using namespace std;

//eight Queens Question

//define Queens and counts of result
int Queens[8] = {0};
int counts = 0;

//the line of queen and the list of queen
bool isRight(int line,int list)
{
    for (int index = 0; index < line; index++)
    {
        //get position of other Queens 
        int pos = Queens[index];

        //check is right
        if (list == pos)
        {
            return false;
        }
        if ((index + pos) == (line + list))
        {
            return false;
        }
        if ((index - pos) == (line - list))
        {
            return false;
        }
    }
    return true;
}

void print()
{
    for (int line = 0; line < 8; line++)
    {
        for (int list = 0; list < 8; list++)
        {
            if (list == Queens[line])
            {
                cout << "1 ";
            }
            else
            {
                cout << "0 ";
            }
        }
        cout << endl;
    }
    cout << "---------------" << endl;
}

void eightQueens(int line)
{
    for (int list = 0; list < 8; list++)
    {
        //check
        if ( isRight(line, list) )
        {
            Queens[line] = list;

            if (line == 7)
            {
                counts++;
                print();
                Queens[line] = 0;
                return;
            }

            eightQueens(line + 1);
            Queens[line] = 0;
        }
    }
}

int main()
{
    //start solve eightQueens Qusetion 
    eightQueens(0);

    cout << "counts is :" << counts << endl;

    return 0;
}