// g++ readbit.cpp -o readbit

// 01000001 - A ascci table

#include "../src/bitstream/BitStream.cpp"

void printAsBinary(char byte[]);
void printAsChar(char byte[]);

int main(){
    BitStream bs("out.bin", 'r');

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
        buff |= bit;
    }
    printf("char: %c\n", buff);
}


