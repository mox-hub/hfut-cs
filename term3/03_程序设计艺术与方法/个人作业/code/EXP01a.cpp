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
//main start
int main(void)
{
    //define vector and list
    vector<int> vector;
    list<int> list;
    std::vector<int>::iterator iter;
    //make random number
    srand(time(NULL));

    /* test 1 */
    cout << "-----------------------------------------" << endl;
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

    /* test 2 */
    cout << "-----------------------------------------" << endl;
    int temp = rand();
    cout << "The random number insert into the header is： " << temp << endl;
    //insert into header
    vector.insert(vector.begin(), temp);
    // output vector
    cout << "vector: ";
    for (iter = vector.begin(); iter != vector.end(); iter++)
	{
		cout << *iter << std::setw(8);
	}
    cout << endl;

    /* test 3 */
    cout << "-----------------------------------------" << endl;
    temp = rand();
    cout << "number to find is:" << temp << endl;
    iter = find(vector.begin(), vector.end(), temp);
    if(temp == *iter)
    {
        cout << "number is be find" << endl;
    }
    else 
    {
        cout << "number is not be find" << endl;
        vector.insert(vector.end(), temp);
        // output vector again
        cout << "vector: ";
        for (iter = vector.begin(); iter != vector.end(); iter++)
        {
            cout << *iter << std::setw(8);
        }
        cout << endl;
    }
    /* test 4 */
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

    /* test 5 */
    cout << "-----------------------------------------" << endl;
    // delete the end number
    vector.pop_back();
    // output vector
    cout << "vector: ";
	for (iter = vector.begin(); iter != vector.end(); iter++)
	{
		cout << *iter << std::setw(8);
	}
    cout << endl;

    /* test 6 */
    cout << "-----------------------------------------" << endl;
    vector.clear();
    if(vector.empty())
    {
        cout << "vector is empty" << endl;
    }
    
    return 0;
}
//main end