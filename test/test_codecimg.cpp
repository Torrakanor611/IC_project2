// g++ test_codecimg.cpp -o test_codecimg $(pkg-config --libs --cflags opencv4) -I/usr/include/python3.8 -lpython3.8

#include "../src/codecimg/Codecimg.cpp"

int main(){

    Codecimg c("../files/miuda.png");

    cout << "chegou aqui!" << endl;

    c.applyPredJLS();

    c.encode();

    return 0;
}
