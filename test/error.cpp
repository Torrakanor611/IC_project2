/*
 * Example test throw exception
 *
 * g++ error.cpp -o error
*/

#include "../src/bitstream/BitStream.cpp"

int main(){
    try{
       BitStream bs2("file_not_exists.bin", 'r'); // wrong mode
    }catch(const std::runtime_error& e)
    {
        printf("runtime_error caught!\n");
        std::cerr << e.what() << '\n';
    }


    try{
       BitStream bs2("out.bin", 's'); // wrong mode
    }catch(const std::runtime_error& e)
    {
        printf("runtime_error caught!\n");
        std::cerr << e.what() << '\n';
    }

    // --------------------

    BitStream bs("out.bin", 'w');

    // try read in 'w' mode
    try{
       char c = bs.readBit(); 
    }catch(const std::runtime_error& e)
    {
        printf("runtime_error caught!\n");
        std::cerr << e.what() << '\n';
    }

    try{
        char array[8];
        bs.readNbits(array, 8); 
    }catch(const std::runtime_error& e)
    {
        printf("runtime_error caught!\n");
        std::cerr << e.what() << '\n';
    }
    bs.close();


    // -------------------

    BitStream bs1("out.bin", 'r');

    // try read in 'w' mode
    try{
        bs1.writeBit('0'); 
    }catch(const std::runtime_error& e)
    {
        printf("runtime_error caught!\n");
        std::cerr << e.what() << '\n';
    }

    try{
        int n = 8;
        char array[n] = {'0', '0', '1', '1', '0', '1', '0', '1'};
        bs1.writeNbits(array, n); 
    }catch(const std::runtime_error& e)
    {
        printf("runtime_error caught!\n");
        std::cerr << e.what() << '\n';
    }
    bs.close();

    return 0;
}


    