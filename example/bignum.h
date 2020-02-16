#include <stdio.h>

#ifndef BIGNUM_H
#define BIGNUM_H
    #define PLUS 0
    #define MINUS 1
    struct bignum_struct {
        char *revdigits; // digits are stored in reverse order
        int sign;
        int size;
        int decimal;
    };
    typedef struct bignum_struct bignum;
    extern bignum bignum_default;
    char* mallocWithError(int length);
    void freeMallocSpace(bignum *bi);
    int isZero(bignum *bi);
    int isOne(bignum *bi);
    void generateEmpty(bignum *biempty);
    int generateZero(bignum *one);
    int generateOne(bignum *one);
    int initialize_bignum(bignum *bi, char *str);
    int copyBignum(bignum *copyFrom, bignum *copyTo, int noOfMFDtoCopy, int doRounding);
    void fixLeadingZero(bignum *bi);
    int fixTrailingZero(bignum *bi);
    int sadd(bignum *b1, bignum *b2, bignum *ans);
    int ssub(bignum *b1, bignum *b2, bignum *ans);
    int smultiply(bignum *b1, bignum *b2, bignum *ans);
    int multiplyBy10Pow(bignum *from, bignum *to, int power);
    int sdivide(bignum *b1, bignum *b2, bignum *ans, int precision, int roundOffLastDigit);
    int generateReciprocal (bignum *b1, bignum *ans, int precision, int roundOffLastDigit);
    int usquareRoot(bignum *b1, bignum *ans, int precision, int roundOffLastDigit);
    int absolute(bignum *bi, bignum *ans);
    int spower(bignum *b1, bignum *b2, bignum *ans, int precision, int roundingOff);
    void string_bignum(bignum *bi, char *output);
    void print_bignum(bignum *bi);
#endif