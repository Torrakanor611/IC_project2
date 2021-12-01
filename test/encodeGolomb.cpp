// g++ encodeGolomb.cpp -o encodeGolomb

#include "../src/golomb/Golomb.cpp"
#include "util/util.cpp"

void printAsBinary(char byte[]);
void printAsChar(char byte[]);


int main(int argc, char* argv[]){
    //Get m and n
    int m, n;
    cout << "Insert m: ";
    cin >> m;
    cout << "Insert n: ";
    cin >> n;

    //Initialize Golomb object for enconding a number
    Golomb gb("t.bin", 'e', 14);
    //Encode number
    int size = gb.encode(22);
    cout << "Encoded value has " << size << " Bits" << endl;
    //Close stream
    gb.close();

    
    //BitStream to read result
    BitStream bs("t.bin", 'r');
    char byte[size+1];    
    bs.readNbits(byte, size+1);
    //Print in Binary
    printAsBinaryMsbtoLsb(byte, size);
    //Close BitStream
    bs.close();
}
