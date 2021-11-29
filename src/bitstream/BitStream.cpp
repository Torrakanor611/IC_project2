#include "BitStream.hpp"

#include <math.h>

using namespace std;

unsigned char BitStream::readBit(){
    if(pointer == 0){
        pointer = 8;
        file.read(reinterpret_cast<char*>(&buffer), 1);
    }
    pointer--;
    return (buffer >> pointer) & 0x01;
}

void BitStream::writeBit(char bit){
    if(pointer == 1){
        buffer |= (bit & 0x01);
        file.write(reinterpret_cast<char*>(&buffer), 1);
        buffer = 0;
        pointer = 8;
        return;
    }
    pointer--;
    buffer |= ((bit & 0x01) << pointer);
}

void BitStream::readNbits(char* bits, int n){
    for(int i = 0; i < n; i++){
        bits[i] = readBit();
    }
}

void BitStream::writeNbits(char* bits, int n){
    for(int i = 0; i < n; i++){
        writeBit(bits[i]);
    }
}

// write remain buffer on file
void BitStream::close(){
    if (pointer != 8){
        file.write(reinterpret_cast<char*>(&buffer), 1);
    }
    file.close();
}