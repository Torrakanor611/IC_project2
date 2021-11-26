#include "BitStream.hpp"

using namespace std;


unsigned char BitStream::readBit(){
    if(file.is_open()){
        if (bit == 0){
            bit = 7;
            file.read((char*)&buffer , 1);
            return (buffer >> bit) & 0x01; 
        }
        bit--;
        return (buffer >> bit) & 0x01;
    }
}

void BitStream::writeBit(unsigned char bit){
    ;
}

unsigned char* BitStream::readNbits(int inipos, int n){
    ;
}

void BitStream::writeNbits(int inipos, int n, unsigned char* bits){
    ;
}

// write buffer on file
void BitStream::close(){
    ;
}