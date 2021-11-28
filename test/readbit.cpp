// g++ readbit.cpp -o readbit

#include "../src/bitstream/BitStream.cpp"
#include "util/util.cpp"

void printAsBinary(char byte[]);
void printAsChar(char byte[]);

int main(){
    BitStream bs("t.bin", 'r');

    char byte[8];

    // read 8 bits (1 byte)
    for (int i = 0; i < 8; i++){
        byte[i] = bs.readBit();
    }
    // print it
    printAsBinary(byte);
    printAsChar(byte);


    // read another 8 bits
    for (int i = 0; i < 8; i++){
        byte[i] = bs.readBit();
    }
    // print
    printAsBinary(byte);
    printAsChar(byte);

    // read plus 5 bits
    printf("%d\n", bs.readBit());
    printf("%d\n", bs.readBit());
    printf("%d\n", bs.readBit());
    printf("%d\n", bs.readBit());
    printf("%d\n", bs.readBit());
}



