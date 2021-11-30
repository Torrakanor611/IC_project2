#include "Golomb.hpp"
#include <math.h>

using namespace std;

void Golomb::encode (int n){
    int q = floor(n / m);
    int r = n - q*m;

    // Variable to store number of bits of the binary part
    int numbits;
    //Value to be coded in the binary part
    int value;

    //If m is power of two 
    if (m!=0 && (m & (m-1)) == 0){
        value = r;
        numbits = b;
    }
    //If m is not power of two
    else{
        if (r < pow(2, b) - m){
            value =  r; 
            numbits = b-1;
        }
        else{
            value = r + pow(2, b) - m;
            numbits = b;
        }
    }

    //Calculate Binary part
    string aux;
    int numbitsAux = numbits;
    while (value != 0){
        aux += ( value % 2 == 0 ? "0" : "1" );
        value /= 2;
        numbitsAux --;
    }
    while(numbits != 0){
        aux+= "0";
        numbitsAux--;
    }
    //Write binary value in Bitstream
    for(int i = 0; i < numbits; i++ )
        file.writeBit(aux[i]);

    //Calculate unary part
    file.writeBit('0'); 
    for (int i = 0 ; i < n; i++)
        file.writeBit('1');
}

