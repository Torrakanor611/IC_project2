#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;

class BitStream {
    public:
        BitStream(string filename, char mode){
            inoutfs.open(filename);
            if (mode != 'w' && mode != 'r'){
                cout << "ERROR: invalid mode!" << endl;
                exit(EXIT_FAILURE);
            }
            
        }
        unsigned char readBit();
        void writeBit(unsigned char bit);
        unsigned char* readNbits(int inipos, int n);
        void writeNbits(int inipos, int n, unsigned char* bits);
    
    private:
        fstream inoutfs;
        unsigned char* buffer;              // buffer de trabalho
        int numBits;
};
