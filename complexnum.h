#include "bignum.h"

#ifndef MAXSIZE
#define MAXSIZE 500001
#endif

#ifndef COMPLEXNUM_H
#define COMPLEXNUM_H
    struct complexnum_struct {
        bignum real;
        bignum imaginary;
    };
    typedef struct complexnum_struct complexnum;
    extern complexnum complexnum_default;
    int parseComplexNoStr(char* str, char seperated[][MAXSIZE]);
    int initialize_complexnum(complexnum *ci, char *str);
    int complex_add(complexnum *c1, complexnum *c2, complexnum *ans);
    int complex_sub(complexnum *c1, complexnum *c2, complexnum *ans);
    int complex_absolute(complexnum *c1, bignum* ans, int precision, int roundOff, int alsoTakeSqRoot);
    int complex_multiply(complexnum *c1, complexnum *c2, complexnum *ans);
    int complex_conjugate (complexnum *c1, complexnum *ans);
    int complex_divide(complexnum *c1, complexnum *c2, complexnum *ans, int precision, int roundOff);
    void fixLeadingZeroInComplex(complexnum *c1);
    int fixTrailingZeroInComplex(complexnum *c1);
    void freeMallocSpaceInComplex(complexnum *c1);
    void string_complex(complexnum *ci, char *output);
    void print_complex(complexnum *ci);
#endif