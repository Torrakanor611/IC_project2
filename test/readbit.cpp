// g++ readbit.cpp -o readbit

// 01000001 - A ascci table

#include "../src/bitstream/BitStream.cpp"

void printAsBinary(char byte[]);
void printAsChar(char byte[]);

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

int main(){
    BitStream bs("t.bin", 'r');

    char byte[8];

    // read 8 bits
    for (int i = 0; i < 8; i++){
        byte[i] = bs.readBit();
    }
    printAsBinary(byte);
    printAsChar(byte);   
}

void printAsBinary(char byte[]){
    printf("binary: ");
    for (int i = 0; i < 8; i++){
        printf("%d", byte[i]);
    }
    printf("\n");
}

void printAsChar(char byte[]){
    unsigned char buff = 0;
    unsigned char bit;
    for(int i = 0; i < 8; i++){
        bit = (byte[i] & 0x01) << (7 - i);
        buff = buff | bit;
        // printf("buff it. %d: "BYTE_TO_BINARY_PATTERN"\n", i, BYTE_TO_BINARY(buff));
    }
    printf("char: %c\n", buff);
}


