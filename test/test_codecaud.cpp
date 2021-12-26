// g++ test_codecaud.cpp -o test_codecaud -lsndfile -I/usr/include/python3.8 -lpython3.8
#include "../src/codecaud/Codecaud.cpp"

int main(){

    Codecaud c("../files/sample01.wav");

    c.compress("compress.bin", 1);
    printf("ok compress\n\n");

    c.decompress("compress.bin");
    printf("ok decompress\n\n");

    char op;
    cout << "Calculate the histograms and entropy value of the residuals? (y/n)" << endl;
    cin >> op;

    while(!(op == 'y' || op == 'n')) {
        cout << "Calculate the histograms and entropy value of the residuals? (y/n)" << endl;
        cin >> op;
    }
    if(op == 'y') {
        c.showHistEnt();
    }
    return 0;
}

