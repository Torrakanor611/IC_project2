// g++ test_codecaud.cpp -o test_codecaud -lsndfile -I/usr/include/python3.8 -lpython3.8
#include "../src/codecaud/Codecaud.cpp"

int main(){

    Codecaud c("../files/sample01.wav");

    c.compress("compress.bin");
    printf("ok compress\n\n");

    c.decompress("compress.bin");
    printf("ok decompress\n\n");

    return 0;
}

