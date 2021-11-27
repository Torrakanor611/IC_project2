#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;

class BitStream {
    public:
        BitStream(const char *filename, char mode){
            if (mode != 'w' && mode != 'r'){
                cout << "ERROR: invalid mode!" << endl;
                exit(EXIT_FAILURE);
            }
            if(mode == 'w'){
                file.open(filename, ios::binary | ios::out);
                bit = 8;
            }else{  // mode == 'r'
                file = fstream(filename, ios::binary | ios::in);
                bit = 0;
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
        int bit; // goes from 0 to 7
        // bit in 'r' mode points to last read bit
        // bit in 'w' mode points to last written bit

};

#endif
