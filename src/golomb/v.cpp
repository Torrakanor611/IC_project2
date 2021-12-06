#include <iostream>
#include <algorithm>
#include <math.h>
using std::cout;
using std::endl;
using std::string;

int fold(int n){
    if (n >= 0)
        return n*2;
    else
        return abs(n)*2-1;

}


int unfold(int n){
    if (n % 2 == 0)
        return n/2;
    else
        return (-1)*ceil(n/2)-1;

}


int main() {
    int number = 7;

    cout << "decimal: " << unfold(5) << endl;

    return EXIT_SUCCESS;
}