#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <map>

using namespace std;

class BitStream {
    public:
        BitStream(const char *filename, char mode){
            if (mode != 'w' && mode != 'r'){
                cout << "ERROR: invalid mode!" << endl;
                exit(EXIT_FAILURE);
            }
            if(mode == 'w'){
                file = fstream(filename, ios::binary | ios::out);
                pointer = 8;
                buffer = 0;
            }else{  // mode == 'r'
                file = fstream(filename, ios::binary | ios::in);
                pointer = 0;
                buffer = 0;
            }
        }
        unsigned char readBit();
        void writeBit(unsigned char bit);
        unsigned char readNbits(int n);
        void writeNbits(int n, unsigned char* bits);
        void close();
    
    private:
        fstream file;
        unsigned char buffer;   // buffer[7] Msb , ... , buffer[0] Lsb
        int pointer; // goes from 0 to 7
        // pointer in 'r' mode points to last read bit
        // pointer in 'w' mode points to last written bit
};

#endif
