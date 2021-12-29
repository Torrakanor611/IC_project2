// g++ test_codecimg.cpp -o test_codecimg $(pkg-config --libs --cflags opencv4) -I/usr/include/python3.8 -lpython3.8

#include "../src/codecimg/Codecimg.cpp"

int main(){
    try{
        Codecimg c("../files/miuda.png");

        c.compress("miuda_encoded.bin");

        c.decompress("miuda_encoded.bin", "miuda_restored.png");
    }catch(runtime_error &e){
        cout << e.what() << endl;
    }
    return 0;
}
