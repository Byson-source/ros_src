#include <iostream>
#include <string>

using namespace std;

int main(){
    int a{0};
    int *a_ptr{new int()};
    a_ptr=&a;
//    cout<<*a_ptr<<endl;

    delete a_ptr;

//    int b{0};
//    int *a_ptr{new int()};
//
//    cout<<a<<endl;
    return 0;

}
