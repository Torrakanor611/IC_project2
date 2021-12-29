#include "Golomb.hpp"
#include "../bitstream/BitStream.hpp"
#include <math.h>
#include <cstdlib>

using namespace std;

//Auxiliary function to convert char array from bin to int
int convertToInt(char arr[], int size){
    int res=0;
    for(int i=0;i<size;i++){
        if(arr[i] != 0x00){
            res+=pow(2, i);
        }
    }
    return res;

}
//Auxiliary function to convert int value to string in binary with numBits bits
string convertToBin(int value, int numBits){
    string aux = "";
    while (value != 0){
        aux += ( value % 2 == 0 ? '0' : '1' );
        value /= 2;
        numBits --;
    }
    while(numBits != 0){
        aux+= '0';
        numBits--;
    }
    return aux;
}

Golomb::Golomb(){}

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



int Golomb::encode(int n){
    n = fold(n);
    //cout << "Value to be decoded after Folding: " << n << endl;
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

void Golomb::encodeM(int m){
    string s = convertToBin(m, 32);
    char space[32];

    for(int i = 0; i < 32; i++){
        space[i] = s[i];
    }

    Gfile.writeNbits(space, 32);
}

int Golomb::decodeM(){
    char space[32];
    Gfile.readNbits(space, 32);
    return convertToInt(space, 32);
}

void Golomb::setM(int mi){
    this->m = mi;
    b =  ceil(log2(m));
}


int Golomb::decode(){
    int A = 0;
    int R = 0;
    // int size = 0;

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

void Golomb::encondeShamt(int shamt){
    //Shamt -> 5 bits;
    string sh = convertToBin(shamt, 5);
    char hdr[5];
    for(int i=0; i < 5; i++)
        hdr[i] = sh[i];
    Gfile.writeNbits(hdr, 5);
}

int Golomb::decodeShamt(){
    char shamt[5];
    Gfile.readNbits(shamt, 5);
    return convertToInt(shamt,5);
}

void Golomb::encodeMode(int mode){
    char x = (mode == 0) ? '0' : '1';
    Gfile.writeBit(x);
}


int Golomb::decodeMode(){
    return (int) (Gfile.readBit() & 0x01);
}




void Golomb::encodeHeaderSound(int nFrames, int sampleRate, int Channels, int format, bool lossy){
    /*
     * Header
     * Golomb m                 -> 32 bits
     * 
     * LossyOrLosses            -> 1 bit(0->Lossless, 1->Lossy)
     * Number of samples        -> 32 bits
     * Sample Rate              -> 32 bits
     * Format                   -> 32 bits
     * Channels                 -> 4 bits
     */ 
    string header;
    if(lossy)
        header = '1';
    else
        header = '0';
    header += convertToBin(nFrames, 32);
    header += convertToBin(sampleRate, 32);
    header += convertToBin(format,32);
    header += convertToBin(Channels, 4);

    char hdr[101];
    for(int i=0; i < 101; i++)
        hdr[i] = header[i];
    Gfile.writeNbits(hdr, 101);
}

void Golomb::decodeHeaderSound(int arr[]){
    //Arr[0] is codecoption (lossy or lossless)
    char option = Gfile.readBit();
    arr[0] = int(option&0x1);    
    char rd[32];
    //Arr[1] is num of samples
    Gfile.readNbits(rd, 32);
    arr[1] = convertToInt(rd,32);
    //Arr[2] is Sample Rate
    Gfile.readNbits(rd, 32);
    arr[2] = convertToInt(rd,32);
    //Arr[3] is num of samples
    Gfile.readNbits(rd, 32);
    arr[3] = convertToInt(rd,32);
    //Arr[4] is num of channels
    char nc[4];
    Gfile.readNbits(nc, 4);
    arr[4] = convertToInt(nc,4);
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