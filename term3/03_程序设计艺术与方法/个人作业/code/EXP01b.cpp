/**
 * @author: Li Haporan 
 * @date: 2020.10.20
 * @details: 实验一STL的熟悉与使用
*/

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <ctime>
#include <iomanip>
using namespace std;

#define count 10

//define up and down of sort
bool up(int vector1,int vector2)
{
    return vector1 < vector2;
}

bool down(int vector1,int vector2)
{
    return vector1 > vector2;
}
int main(void)
{
    list<int> list;
    vector<int> vector;
    std::vector<int>::iterator iter;
    //make random number
    srand(time(NULL));

    for (size_t i = 0; i < count; i++) 
    {
        //make 10 rand number of int
        vector.push_back( rand() );
    }

    //before sort output vector
    cout << "vector: ";
	for (iter = vector.begin(); iter != vector.end(); iter++) 
    {
		cout << *iter << std::setw(8);
	}
    cout << endl;
    cout << "-----------------------------------------" << endl;
    //sort
    sort(vector.begin(), vector.end(), up);

    //after sort output vector
    cout << "vector: ";
	for (iter = vector.begin(); iter != vector.end(); iter++)
	{
		cout << *iter << std::setw(8);
	}
    cout << endl;

    cout << "-----------------------------------------" << endl;
    //sort
    sort(vector.begin(), vector.end(), down);

    //after sort output vector
    cout << "vector: ";
	for (iter = vector.begin(); iter != vector.end(); iter++)
	{
		cout << *iter << std::setw(8);
	}
    cout << endl;

    
    cout << "-----------------------------------------" << endl;
    int min = vector[0];
    for (iter = vector.begin(); iter != vector.end(); iter++)
	{
		if (*iter < min)
        {
            min = *iter;
        }
	}
    cout << "the min number is:" << min << endl;
    
    int max = vector[0];
    for (iter = vector.begin(); iter != vector.end(); iter++)
	{
		if (*iter > min)
        {
            max = *iter;
        }
	}
    cout << "the max number is:" << max << endl;
    return 0;
}