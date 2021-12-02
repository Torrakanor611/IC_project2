// g++ testGolomb.cpp -o testGolomb

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

    cout << endl  << "Testing Enconding..." << endl;
    //Initialize Golomb object for enconding a number
    Golomb gb("t.bin", 'e', m);
    //Encode number
    int size = gb.encode(n);
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


    cout << endl << "Testing Decoding..." << endl;
    //Initialize Golomb object for decoding value
    Golomb g("t.bin", 'd', m);
    //Decode number
    int value = g.decode(size);
    //Close stream
    g.close();
    cout << "Decoded Value: " << value << endl;

    return 0;
}
