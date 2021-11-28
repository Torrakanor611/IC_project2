// g++ readbit.cpp -o readbit

#include "../src/bitstream/BitStream.cpp"

int main(){
    BitStream bs("outN.bin", 'r');

    int n = 45;
    char array[n];
    
    bs.readNbits(array, n);

    for(int i = 0; i < n; i++){
        if(i % 8 == 0 && i != 0)
            printf("\n");
        printf("%d", array[i]);
    }
    printf("\n");
    return 0;
}