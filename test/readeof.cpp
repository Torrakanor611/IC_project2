// g++ readeof.cpp -o readeof

#include "../src/bitstream/BitStream.cpp"

int main(){
    BitStream bs("outN.bin", 'r');

    int n = 45;
    char array[n];

    int i = 0;
    while(!bs.eof()){
        array[i] = bs.readBit();
        // print
        if(i % 8 == 0 && i != 0)
            printf("\n");
        printf("%d", array[i]);
        i++;
    }
    printf("\n");
    bs.close();

    return 0;
}