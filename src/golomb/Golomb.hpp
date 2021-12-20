#ifndef GOLOMB_H
#define GOLOMB_H

#include "../bitstream/BitStream.cpp"
#include <math.h> 

using namespace std;

/**
 * Class for encoding and decoding values using golomb code.
 */
class Golomb {
    public:
        Golomb();

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
        int decode();

        /**
         * Convert an integer (positive or negative) into positive using folding tecnique
         * @param n integer number to be converted
         * @return converted value
         * */
        int fold(int n);


        /**
         * Convert a positive number into integer according to the folding tecnique
         * @param n integer number to be converted
         * @return converted value
         * */
        int unfold(int n);

        /**
         * Close the golomb stream.
         * */
        void close();

        void encodeM(int n);

        int decodeM();

        void setM(int mi);

    private:
        BitStream Gfile; 
        int m; 
        int b;
        bool pt;
};

#endif
