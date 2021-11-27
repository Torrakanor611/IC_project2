#include "BitStream.hpp"

using namespace std;

unsigned char BitStream::readBit(){
    if(bit == 0){
        bit = 7;
        file.read((char*)&buffer , 1);
        return (buffer >> bit) & 0x01; 
    }
    bit--;
    return (buffer >> bit) & 0x01;
}

void BitStream::writeBit(unsigned char bit){
    if(bit == 0){
        buffer = buffer | (bit & 0x01);
        file.write((char*)&buffer, 1);
        bit = 7;
        buffer = (bit & 0x01) << bit;
    }
    bit--;
    buffer = buffer | ((bit & 0x01) << bit);
}

unsigned char BitStream::readNbits(int n){
    ;
}

void BitStream::writeNbits(int n, unsigned char* bits){
    ;
}

// write buffer on file
void BitStream::close(){
    ;
}