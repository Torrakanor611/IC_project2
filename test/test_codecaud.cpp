// g++ test_codecaud.cpp -o test_codecaud -lsndfile -I/usr/include/python3.8 -lpython3.8
#include "../src/codecaud/Codecaud.cpp"

int main(){

    Codecaud c("../files/sample01.wav");

    c.compress("compress.wav");
    printf("ok compress");

    return 0;
}

