#ifndef GOLOMB_H
#define GOLOMB_H

#include "../bitstream/BitStream.hpp"
#include <math.h> 

using namespace std;

/**
 * Class for encoding and decoding values using golomb code.
 */
class Golomb {
    public:
        /**
         * Golomb Class Constructor
         * @param filename Path to a file were to store the enconded value or to read a value to be decoded.
         * @param mode character 'e' for encoding a value 'd' for decoding a value.
         * @param mvalue value of m that will be used in golomb encoding and decoding .
         */
        Golomb(const char *filename, char mode, int mvalue);

        /**
         * Encode a number (positive or negative) using golomb code.
         * @param n Integer number, positive or negative to be encoded.
         * @return number of bits of the output encoded number.
         * */
        int encode(int n);

        /**
         * Decode a number using golomb code. It has no parameters because the value to be decoded is read from the file 
         * given when the constructor of the class Golomb is initialized.
         * @return the decoded value.
         * */
        signed int decode(int size);

        /**
         * Close the golomb stream.
         * */
        void close();
    
    private:
        BitStream Gfile; 
        int m; 
        int b;
};

#endif
