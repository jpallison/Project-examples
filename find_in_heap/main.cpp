/* Joseph Allison
 * 3/3/2014
 * CSE 431
 * 
 */
#include <iostream>
#include <cstdlib>
#include <vector> 
using namespace std;

int hfind(vector<int> T, int x, int i)
{
    if (i >= T.size()) 
    {
        return 0;
    }
    if (x == T[i])
    {
        return i;
    }
    if (x > T[i])
    {
        return 0;
    }
    
    int value = hfind(T,x,2*i+1);
    if (value!=0)
    {
        return value;
    }
    return hfind(T,x,2*i+2);

}


int find_in_heap (vector<int> T, int x)
    {
    return hfind(T,x,0);
    }



int main(int argc, char** argv) {

    vector<int> x;
    x.push_back(14);
    x.push_back(12);
    x.push_back(10);
    x.push_back(7);
    x.push_back(8);
    x.push_back(9);
    x.push_back(6);
    
    int possition1 =find_in_heap(x,12);
    int possition2 =find_in_heap(x,35);
    int possition3 =find_in_heap(x,8);
    int possition4 =find_in_heap(x,6);
    
    cout << " " << possition1<< " " << possition2<< " " << possition3<< " " << possition4;
 
}

