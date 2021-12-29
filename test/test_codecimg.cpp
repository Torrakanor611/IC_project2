// g++ test_codecimg.cpp -o test_codecimg  $(pkg-config --libs --cflags opencv4) -I/usr/include/python3.8 -lpython3.8
#include "../src/codecimg/Codecimg.cpp"
#include <ctime>

int main(int argc, char* argv[]){

    //Command line arguments processing
    if(argc != 2){
        cout << "\033[1;31mError: Following argument is required <input filename>\033[0m" << endl;
        return 0; 
    }
    char *inputfile = argv[1];

    bool verbose;
    char x;
    while(!(x == 'y' || x == 'n')) {
        cout << "Present histograms and images(verbose mode) (y/n)? ";
        cin >> x;
    }
    verbose = x == 'y' ? true : false;

    int op1 = -1;
    while(!(op1 == 0 || op1 == 1)) {
        cout << "Choose codec option (0 for lossless or 1 for lossy): ";
        cin >> op1;
    }
    int bits = -1;
    if(op1 == 1){
        while(bits < 0 || bits > 7) {
            cout << "Choose quantization step(number of bits to remove): ";
            cin >> bits;
        }
    }
    Codecimg c(inputfile, verbose);
    
    clock_t begin = clock();
    if(op1 == 0){
        c.compress("compressed_img.bin");
    }else{
        c.compresslossy("compressed_img.bin", bits, bits, bits);
    }

    clock_t end = clock();
    cout <<"Duration: " <<  (double(end - begin) / CLOCKS_PER_SEC) <<"seg "<< endl;
    printf("ok compress\n\n");

    c.decompress("compressed_img.bin", "miuda_restored.png");
    printf("ok decompress\n\n");

    return 0;
}

