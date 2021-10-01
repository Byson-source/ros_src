#include <iostream>
#include <string>
#include <vector>

using namespace std;
void test(int a){
    a+=2;
    cout<<a<<endl;
}

int main(int argc, char *argv[]){
    int a{3};
    int *a_ptr{&a};

    test(a);
    cout<<a<<endl;
    cout<<"argc is "<<argc<<endl;
    cout<<"argv is "<<argv<<endl;
    return 0;

}
