#include<iostream>
#include<fstream>

using namespace std;

int main(){

    fstream f("t.txt", ios::in);

    if(!f.is_open()){
        throw runtime_error("Could not open file");
    }
    char c;
    f.read(&c, 1);
    printf("%c\n", c);
    f.read(&c, 1);
    printf("%c\n", c);
    f.read(&c, 1);
    printf("%c\n", c);
    f.read(&c, 1);
    printf("%c\n", c);
    f.read(&c, 1);
    printf("%c\n", c);
    f.read(&c, 1);
    printf("%c\n", c);


    return 0;
}