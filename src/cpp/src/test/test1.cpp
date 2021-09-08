#include <iostream>
using namespace std;

int main(){
    int a{0};
    int b{0};
    int *a_ptr{&a};
    cout<<(a && b==0)<<endl;
    cout<<a_ptr<<endl;
    if(a_ptr)
        cout<<"Yes, that's right"<<endl;
//ポインタのアドレスはTrueになる！？？
    int c{a};

    return 0;

}
