#include "BitStream.hpp"

using namespace std;

unsigned char readBit();

void writeBit(unsigned char bit);

unsigned char* readNbits(int inipos, int n);

void writeNbits(int inipos, int n, unsigned char* bits);
