// g++ writeNbits.cpp -o writeNbits

// 01000001 - A

#include "../src/bitstream/BitStream.cpp"

int main(){
    BitStream bs("outN.bin", 'w');


    int n = 40;
    char bits[n] = {
        '0', '1', '0', '0', '0', '0', '0', '1', // A
        '0', '1', '0', '0', '0', '1', '0', '0', // D
        '0', '1', '1', '0', '0', '1', '0', '1', // e
        '1', '0', '0', '0', '0', '0', '0', '0',
        '0', '0', '1', '1', '0', '0', '0', '1', // 1
    };

    bs.writeNbits(bits, n);

    bs.close();

    return 0;
}