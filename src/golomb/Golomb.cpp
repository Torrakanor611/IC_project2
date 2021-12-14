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
    pt = false;
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

    //Calculate unary part and write it
    size++;
    for (int i = 0 ; i < q; i++){
        Gfile.writeBit('1');
        size++;
    }
    Gfile.writeBit('0');

    //Write binary value in Bitstream
    for(int i = numbits-1; i >= 0; i-- )
        Gfile.writeBit(aux[i]);
    return size;
}




int Golomb::decode(){
    int A = 0;
    int R = 0;
    int size = 0;

    if(pt == false)
        extractPointerValue();
    
    char c;
    //Count number of 1s before the first zero (Msbits) -> A
    while(true){
        c = Gfile.readBit();
        if((c & 0x01) == 0x00)
            break;
        A++;
    }

    //If m is power of two 
    if (m!=0 && (m & (m-1)) == 0){
        //cout << "Value has " << (A+b+1) << " bits" << endl;

        char binary[b];
        Gfile.readNbits(binary,b);
    
        //Get b+1 Lsbits of the origingal value to be decoded (R = (0)+b Lsbit in decimal)
        int aux = 0;
        for( int i = b-1; i >= 0; i--){
            if(binary[i] != 0x0)
                R+= pow(2, aux);
            aux++;
        }
        //Calculate decoded value
        return unfold(m*A + R);
    }
    //If m is not power of two
    else{
        int aux = 0;
        char binary[b];
        Gfile.readNbits(binary,b-1);
        binary[b-1] = 0;

        //Extract b-1 Msbs of the original word without the first ones and the first zero and calculate R in decimal
        for (int i = b-2; i >= 0; i--){
            if(binary[i] != 0x00)
                R+= pow(2, aux);
            aux++;
        } 
 
        if(R < pow(2, b) - m){
            //cout << "Value hass " << (A+1) + b - 1 <<" bits"<< R <<endl;
            return unfold(m*A + R);
        }
        //Extract b Msbs of the original word without the first ones and the first zero and calculate R in decimal
        else{
            binary[b-1] = Gfile.readBit();
            //cout << "Values has " << (A+1+b) << " bits"<< endl;
            R=0, aux=0;
            for (int i = b-1; i >= 0; i--){
                if(binary[i] != 0x0)
                    R+= pow(2, aux);
                aux++;
            }
            return unfold(m*A + R - (pow(2, b) - m)); 
        }
    }
    return 0;
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


int Golomb::extractPointerValue(){
    //First char indicates the value of the pointer of bitstream
    char array[8];
    Gfile.readNbits(array, 8);

    char result = array[7];
    int p = 0;

    for(int i=6; i >=4; i--){
        p++;
        if(array[i] == 0x1)
            result = result | (0x01 << p);
    }
    pt = true;
    return int(result);
}




void Golomb::close(){
    Gfile.close();
}
