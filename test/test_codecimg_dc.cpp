// g++ test_codecimg_dc.cpp -o test_codecimg_dc $(pkg-config --libs --cflags opencv4) -I/usr/include/python3.8 -lpython3.8

#include "../src/codecimg/Codecimg.cpp"

int main(){

    Codecimg c("teste");

    c.decompress("miuda_encoded.bin");

    return 0;
}
