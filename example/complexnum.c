#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bignum.h"

#ifndef MAXSIZE
#define MAXSIZE 500001
#endif

struct complexnum_struct {
    bignum real;
    bignum imaginary;
} complexnum_default = {NULL, PLUS, 0, 0, NULL, PLUS, 0, 0};

typedef struct complexnum_struct complexnum;

int parseComplexNoStr(char* str, char seperated[][MAXSIZE]) {
    int len = strlen(str);
    int a=-1,b=-1,c=-1,d=-1;
    for (int i=0; i<len; i++) {
        if (str[i] == '(') a = i+1;
        else if (str[i] == ',') { b = i-1; c=i+1; }
        else if (str[i] == ')') d = i-1;
    }

    // i converted ' ' to '\n' initially
    for (int i=a; i<len; i++) {
        if (str[i] == '\n') a++;
        else break;
    }

    for (int i=b; i>=0; i--) {
        if (str[i] == '\n') b--;
        else break;
    }

    for (int i=c; i<len; i++) {
        if (str[i] == '\n' ) c++;
        else break;
    }

    for (int i=d; i>=0; i--) {
        if (str[i] == '\n' ) d--;
        else break;
    }

    if (a!=-1 && b!=-1 && c!=-1 && d!=-1) {
        if (b-a < (MAXSIZE-1) && d-c < (MAXSIZE-1)) {
            if (a == b+1) seperated[0][0] = '\0';
            else {strncpy(seperated[0], str + a, b - a + 1); seperated[0][b - a + 1] = '\0';}
            if (c == d+1) seperated[1][0] = '\0';
            else {strncpy(seperated[1], str + c, d - c + 1); seperated[1][d - c + 1] = '\0';}
            return 0;
        } else {
            return 7;
        }
    } else {
        return 8;
    }
}

int initialize_complexnum(complexnum *ci, char *str) {
    char seperated[2][MAXSIZE];
    int e = 0;
    e = parseComplexNoStr(str, seperated);
    if (e) return e;
    e = initialize_bignum(&(ci->real), seperated[0]);
    if (e) return e;
    e = initialize_bignum(&(ci->imaginary), seperated[1]);
    if (e) return e;
    return 0;
}

int complex_add(complexnum *c1, complexnum *c2, complexnum *ans) { // addition for complexnum
    int e = 0;
    e = sadd(&(c1->real), &(c2->real), &(ans->real));
    if (e) return e;
    e = sadd(&(c1->imaginary), &(c2->imaginary), &(ans->imaginary));
    if (e) return e;
    return 0;
}

int complex_sub(complexnum *c1, complexnum *c2, complexnum *ans) { // signed subtraction for complexnum
    int e = 0;
    e = ssub(&(c1->real), &(c2->real), &(ans->real));
    if (e) return e;
    e = ssub(&(c1->imaginary), &(c2->imaginary), &(ans->imaginary));
    if (e) return e;
    return 0;
}

int complex_absolute(complexnum *c1, bignum* ans, int precision, int roundOff, int alsoTakeSqRoot) { // calculate mod for complexnum
    int e = 0;
    bignum temp1 = bignum_default;
    bignum temp2 = bignum_default;
    e = smultiply(&(c1->real), &(c1->real), &temp1);
    if (e) return e;
    e = smultiply(&(c1->imaginary), &(c1->imaginary), &temp2);
    if (e) return e;
    e = sadd(&temp1, &temp2, ans);
    if (e) return e;
    if (alsoTakeSqRoot == 1) {
        e = usquareRoot(ans, ans, precision, roundOff);
        if (e) return e;
    }
    return 0;
}

int complex_multiply(complexnum *c1, complexnum *c2, complexnum *ans) { // signed product for complexnum
    // if c1 = r1 + i1 * i
    // if c2 = r2 + i2 * i
    // then c1 * c2 = (r1r2 - i1i2) + (r1i2 + r2i1) * i
    int e = 0;
    bignum temp1 = bignum_default;
    bignum temp2 = bignum_default;
    complexnum anstemp = complexnum_default;
    // make duplicate of anstemp because c1 ans can be same or c2 ans can be same
    e = smultiply(&(c1->real), &(c2->real), &temp1);
    if (e) return e;
    e = smultiply(&(c1->imaginary), &(c2->imaginary), &temp2);
    if (e) return e;
    e = ssub(&temp1, &temp2, &(anstemp.real));
    if (e) return e;
    e = smultiply(&(c1->real), &(c2->imaginary), &temp1);
    if (e) return e;
    e = smultiply(&(c2->real), &(c1->imaginary), &temp2);
    if (e) return e;
    e = sadd(&temp1, &temp2, &(anstemp.imaginary));
    if (e) return e;
    ans->real = anstemp.real;
    ans->imaginary = anstemp.imaginary;
    return 0;
}

int complex_conjugate (complexnum *c1, complexnum *ans) {
    int e = 0;
    e = copyBignum(&(c1->real), &(ans->real), -1, 0);
    if (e) return e;
    e = copyBignum(&(c1->imaginary), &(ans->imaginary), -1, 0);
    if (e) return e;
    int sign = ans->imaginary.sign;
    ans->imaginary.sign = sign == PLUS ? MINUS : PLUS;
    return 0;
}

int complex_divide(complexnum *c1, complexnum *c2, complexnum *ans, int precision, int roundOff) { // quotient for complexnum
    // c1 / c2 = (c1 * conjugate (c2)) / (c2 * conjugate (c2)) = (c1 * conjugate (c2)) / abs (c2)
    int e = 0;
    e = complex_conjugate(c2, ans);
    if (e) return e;
    e = complex_multiply(c1, ans, ans);
    if (e) return e;
    bignum denominator = bignum_default;
    e = complex_absolute(c2, &denominator, precision, roundOff, 0);
    if (e) return e;
    e = sdivide(&(ans->real), &denominator, &(ans->real), precision, roundOff);
    if (e) return e;
    e = sdivide(&(ans->imaginary), &denominator, &(ans->imaginary), precision ,roundOff);
    if (e) return e;
    return 0;
}

void fixLeadingZeroInComplex(complexnum *c1) {
    fixLeadingZero(&(c1->real));
    fixLeadingZero(&(c1->imaginary));
}

int fixTrailingZeroInComplex(complexnum *c1) {
    int e = 0;
    e = fixTrailingZero(&(c1->real));
    if (e) return e;
    e = fixTrailingZero(&(c1->imaginary));
    if (e) return e;
    return 0;
}

void freeMallocSpaceInComplex(complexnum *c1) {
    freeMallocSpace(&(c1->real));
    freeMallocSpace(&(c1->imaginary));
}

void string_complex(complexnum *ci, char *output) {
    int c = 0;
    char temp[MAXSIZE];
    string_bignum(&(ci->real), temp);
    int templength = strlen(temp);
    output[c] = '(';
    c += 1;
    for (int i=0; i<templength; i++) {
        if (temp[i] != '\n') {
            output[c] = temp[i];
            c += 1;
        }
    }
    output[c] = ',';
    c += 1;
    string_bignum(&(ci->imaginary), temp);
    templength = strlen(temp);
    for (int i=0; i<templength; i++) {
        if (temp[i] != '\n') {
            output[c] = temp[i];
            c += 1;
        }
    }
    output[c] = ')';
    c += 1;
    output[c] = '\n';
    c += 1;
    output[c] = '\0';
}

void print_complex(complexnum *ci) {
    char str[MAXSIZE];
    string_complex(ci, str);
    printf("%s", str);
}