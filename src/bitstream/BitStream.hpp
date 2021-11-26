#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;

class BitStream {
    public:
        BitStream(string filename, char mode){
            this->file.open(filename, ios::binary);
            if (mode != 'w' && mode != 'r'){
                cout << "ERROR: invalid mode!" << endl;
                exit(EXIT_FAILURE);
            }
            this->bit = 0;
        }
        unsigned char readBit();
        void writeBit(unsigned char bit);
        unsigned char* readNbits(int inipos, int n);
        void writeNbits(int inipos, int n, unsigned char* bits);
        void close();
    
    private:
        fstream file;
        unsigned char buffer;   // buffer[7] Msb , ... , buffer[0] Lsb
        int bit; // goes from 0 to 7
        // bit in 'r' mode points to last read bit
        // bit in 'w' mode points to last written bit

};

#endif
