#include<iostream>
using namespace std;

int x = 0;
int y;

int main()
{
    cout<<"Hello enter an integer:"<<endl;
    cin>>y;
     if(y%2 == 0)
     {
        cout<<y <<" is even\n";
     }
     if (y % 2 != 0)
     {
        cout<< y <<" is odd";
     }
     return 0;
}