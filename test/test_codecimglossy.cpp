// g++ test_codecimglossy.cpp -o test_codecimglossy $(pkg-config --libs --cflags opencv4) -I/usr/include/python3.8 -lpython3.8

#include "../src/codecimg/Codecimg.cpp"

int main(){
    try{
        Codecimg c("../files/miuda.png");
        //Codecimg c("miuda_restored.png");

        c.compresslossy("miuda_encoded.bin", 5, 5, 5 );

        c.decompress("miuda_encoded.bin", "miuda_restored.png");
    }catch(runtime_error &e){
        cout << e.what() << endl;
    }
    return 0;
}