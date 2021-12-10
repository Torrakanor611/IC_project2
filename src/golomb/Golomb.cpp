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
    cout << "Value to be decoded after Folding: " << n << endl;
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




int Golomb::decode(){
    int A = 0;
    int R = 0;
    int size = 0;

    //First char indicates the value of the pointer of bitstream
    char firstChar[8];
    Gfile.readNbits(firstChar, 8);
    int pointerValue = extractPointerValue(firstChar);
    
    //Read value to be decoded
    char c;
    string byte;
    while(!Gfile.eof()){
        c = Gfile.readBit();
        if((c & 0x01) == 1)
            byte+=char(0x1);
        else
            byte+=char(0x0);
        size++;
    }
    size = size - pointerValue;
    cout << "The number of bits is: " << size << endl;
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


int Golomb::extractPointerValue(char array[]){
    char result = array[7];
    int p = 0;

    for(int i=6; i >=4; i--){
        p++;
        if(array[i] == 0x1)
            result = result | (0x01 << p);
    }
    return int(result);
}




void Golomb::close(){
    Gfile.close();
}
