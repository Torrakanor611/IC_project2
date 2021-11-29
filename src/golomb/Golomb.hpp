#ifndef GOLOMB_H
#define GOLOMB_H

#include "../bitstream/BitStream.hpp"
#include <math.h> 

using namespace std;

class Golomb {
    public:
        Golomb();
        Golomb(const char *filename, char mode,int m){
            if (mode != 'd' && mode != 'e'){
                cout << "ERROR: invalid mode!" << endl;
                exit(EXIT_FAILURE);
            }
            if (mode == 'd')
                file = BitStream(filename, 'r');
            else
                file = BitStream(filename, 'w');
            m = m;
            b =  ceil(log2(m));
        }

        void encode(signed int n);
        signed int decode();
        void close();
    
    private:
        BitStream file;
        int m;  
        int b;
};

#endif
