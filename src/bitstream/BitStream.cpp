#include "BitStream.hpp"

#include <math.h>

using namespace std;

unsigned char BitStream::readBit(){
    if(pointer == 0){
        pointer = 7;
        file.read(reinterpret_cast<char*>(&buffer), 1);
        return (buffer >> pointer) & 0x01; 
    }
    pointer--;
    return (buffer >> pointer) & 0x01;
}

void BitStream::writeBit(unsigned char bit){
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

unsigned char BitStream::readNbits(int n){
    unsigned char c;
    return c;
}

void BitStream::writeNbits(int n, unsigned char* bits){
    return;
}

// write remain buffer on file
void BitStream::close(){
    if (pointer != 8){
        file.write(reinterpret_cast<char*>(&buffer), 1);
    }
    file.close();
}