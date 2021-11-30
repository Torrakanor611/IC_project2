// g++ encodeGolomb.cpp -o encodeGolomb

#include "../src/golomb/Golomb.cpp"
#include "../src/bitstream/BitStream.cpp"
#include "util/util.cpp"

void printAsBinary(char byte[]);
void printAsChar(char byte[]);

int main(){
    Golomb gb("t.bin", 'e', 1);
}



