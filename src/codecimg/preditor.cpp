#include "preditor.hpp"

/* Function declarations */
int max(int num1, int num2);
int min(int num1, int num2);

/**
 * x = img(j, i)
 * a = img(j - 1, i)
 * b = img(j, i - 1)
 * c = img(j - 1, i - 1)
**/
int preditorJLS(int a, int b, int c){
    if (c >= max(a, b))
        return min(a, b);
    else if (c < min(a, b))
        return max(a, b);
    else
        return a + b - c;
}

int preditor1(int a){
    return a;
}

int preditor4(int a, int b, int c){
    return a + b - c;
}

/**
 * Find maximum between two numbers.
 */
int max(int num1, int num2)
{
    return (num1 > num2 ) ? num1 : num2;
}

/**
 * Find minimum between two numbers.
 */
int min(int num1, int num2) 
{
    return (num1 > num2 ) ? num2 : num1;
}
    