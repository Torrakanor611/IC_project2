// g++ test_codecimg.cpp -o test_codecimg $(pkg-config --libs --cflags opencv4) -I/usr/include/python3.8 -lpython3.8

#include "../src/codecimg/Codecimg.cpp"

int main(){

    Codecimg c("../files/miuda.png");

    c.applyPredJLS();

    c.encode("miuda_encoded.bin");

    c.decode("miuda_encoded.bin");

    c.restore("miuda_restored.png");

    return 0;
}
