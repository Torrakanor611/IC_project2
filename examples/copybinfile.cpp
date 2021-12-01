/*
 * Example: 
 * copying the contents of a binary file to another
 * both files are given by comand line arguments
 * 
 * g++ copybinfile.cpp -o copybinfile input output
*/
# include "../src/bitstream/BitStream.cpp"

int main(int argc, char** argv){
    if(argc != 3){
        printf("Error: please give input and output file!\n");
        printf("Usage: copybinfile <input_file> <output_file>");

    }

    BitStream bsin(argv[1], 'r');
    BitStream bsout(argv[2], 'w');

    while(!bsin.eof()){
        // read bit from input file
        char c = bsin.readBit();
        // write bit in outputfile
        bsout.writeBit(c);
    }
    // close both
    bsin.close();
    bsout.close();
    
    return 0;
}