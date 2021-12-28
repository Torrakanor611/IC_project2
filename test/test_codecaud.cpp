// g++ test_codecaud.cpp -o test_codecaud -lsndfile -I/usr/include/python3.8 -lpython3.8
#include "../src/codecaud/Codecaud.cpp"

int main(){

    // Codecaud c("../files/sample01.wav");
    Codecaud c("../../IC/P1/wavefiles/sample05.wav");

    char op1 = 0;
    while(!(op1 == '1' || op1 == '2')) {
        cout << "Choose codec option (1 for lossless or 2 for lossy): ";
        cin >> op1;
    }
    
    if(op1 == '2') {
        c.compress("compress.bin", 1, 1);
        printf("ok compress (lossy)\n\n");
    }
    else{
        c.compress("compress.bin", 1, 0);
        printf("ok compress (lossless)\n\n");
    }

    c.decompress("compress.bin");
    printf("ok decompress\n\n");

    char op2 = 0;
    while(!(op2 == 'y' || op2 == 'n')) {
        cout << "Calculate the histograms and entropy value of the residuals (y/n)? ";
        cin >> op2;
    }
    if(op2 == 'y') {
        c.showHistEnt();
    }
    return 0;
}

