#include <iostream>
#include <algorithm>
using std::cout;
using std::endl;
using std::string;

string toBinary(int n)
{
    string r;
    int numbits = 15;
    while (n != 0){
        r += ( n % 2 == 0 ? "0" : "1" );
        n /= 2;
        numbits --;
    }
    //reverse(r.begin(), r.end());
    while(numbits != 0){
        r+= "0";
        numbits--;
    }
    cout << endl;
    for(int i = 0; i < 15; i++ ){
        cout << r[i] << endl;
    }
    return r;
}

int main() {
    int number = 7;

    cout << "decimal: " << number << endl;
    cout << "binary : " << toBinary(number) << endl;

    return EXIT_SUCCESS;
}