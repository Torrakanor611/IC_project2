// g++ writebit.cpp -o writebit

// 01000001 - A

#include "../src/bitstream/BitStream.cpp"

int main(){
    BitStream bs("out.bin", 'w');

    // char A 0x41
    unsigned char byte[8] = {0, 1, 0, 0, 0, 0, 0, 1};

    // char B 0x42
    unsigned char byte2[8] = {0, 1, 0, 0, 0, 0, 1, 0};


    // write 8 bits
    for (int i = 0; i < 8; i++){
        bs.writeBit(byte[i]);
    }

    // for (int i = 0; i < 8; i++){
    //     bs.writeBit(byte2[i]);
    // }
}