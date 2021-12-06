#include "Golomb.hpp"
#include "../bitstream/BitStream.cpp"
#include <math.h>
#include <cstdlib>

using namespace std;


Golomb::Golomb(const char *filename, char mode, int mvalue){
    if (mode != 'd' && mode != 'e'){
        cout << "ERROR: invalid mode!" << endl;
        exit(EXIT_FAILURE);
    }
    if (mode == 'd')
        Gfile = BitStream(filename, 'r');
    else
        Gfile = BitStream(filename, 'w');
    m = mvalue;
    b =  ceil(log2(m));
}



int Golomb::encode (int n){
    n = fold(n);
    int q = floor((int)(n / m));
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
        aux += ( value % 2 == 0 ? '0' : '1' );
        value /= 2;
        numbitsAux --;
    }
    while(numbitsAux != 0){
        aux+= '0';
        numbitsAux--;
    }

    //Size of the word to return in the end
    int size = numbits;

    //Write binary value in Bitstream
    for(int i = 0; i < numbits; i++ )
        Gfile.writeBit(aux[i]);

    //Calculate unary part
    Gfile.writeBit('0');
    size++;
    for (int i = 0 ; i < q; i++){
        Gfile.writeBit('1');
        size++;
    }
    return size;
}




signed int Golomb::decode(int size){
    int A = 0;
    int R = 0;

    //Read value to be decoded
    char byte[size+1];    
    Gfile.readNbits(byte, size+1);
    //Close BitStream
    Gfile.close();

    //Count number of 1s before the first zero (Msbits) -> A
    for (int i = size-1; i >= 0; i--){
        if(byte[i] == 0x0)
            break;
        A++;
    }

    //If m is power of two 
    if (m!=0 && (m & (m-1)) == 0){
        //Get b+1 Lsbits of the origingal value to be decoded (R = b+1 Lsbit in decimal)
        for( int i = 0; i < b+1; i++){
            if(byte[i] != 0x0)
                R+= pow(2, i);
        }
        //Calculate decoded value
        return unfold(m*A + R);
    }
    //If m is not power of two
    else{
        int aux = b-1;
        //Extract b-1 Msbs of the original word without the first ones and the first zero and calculate R in decimal
        for (int i = size-1-A-1; i >= 0; i--){
            aux--;
            if(aux>=0){     
                if(byte[i] != 0x0){
                    R+= pow(2, aux);
                }
            }
            else
                break;
        }
        if(R < pow(2, b) - m)
            return unfold(m*A + R);
        else{
            aux = b;
            R = 0;
            //Extract b Msbs of the original word without the first ones and the first zero and calculate R in decimal
            for (int i = size-1-A-1; i >= 0; i--){
                aux--;
                if(aux>=0){     
                    if(byte[i] != 0x0){
                        R+= pow(2, aux);
                    }
                }
                else
                    break;
            }
            return unfold(m*A + R - (pow(2, b) - m)); 
        }
    }
}


int Golomb::fold(int n){
    if (n >= 0)
        return n*2;
    else
        return abs(n)*2-1;

}


int Golomb::unfold(int n){
    if (n % 2 == 0)
        return n/2;
    else
        return (-1)*ceil(n/2)-1;

}


void Golomb::close(){
    Gfile.close();
}
