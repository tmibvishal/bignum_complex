# Bignum Complex Library for C
**A simple Complex library using Arbitrary Precision Arithmetic in C**


### Description

Complex Bignum supports following

- conjugate, addition, subtraction, multiply and division, absolute

There is a library I have used called arbprecision.

I created arbprecision in an assignment for my course COP290 Design Practices at IIT Delhi in which I had to implement "bignum", you can find that here.
https://github.com/tmibvishal/simple-bignum

arbprecision supports the following:

- addition, subtraction, multiplication and division, factorial, reciprocal.
- Square Root, abs, power(exp)

It also works for negative numbers.

### How to use in your project
Suppose you have a *main.c* file in your project directory that you are working on.
Copy *bignum.c* *bignum.h* *complexnum.c* *complexnum.h* into your project directory. And then include .h files in your main.c file
```c
#include "bignum.h"
#include "complexnum.h"
```
```
$ gcc -o mainfile main.c bignum.c complexnum.c
$ ./mainfile
```
An example program is in the example folder. You can check it.

### function definations

- complexnum structure

    ```c
    struct complexnum_struct {
    bignum real;
    bignum imaginary;
    } complexnum_default = {NULL, PLUS, 0, 0, NULL, PLUS, 0, 0};
    ```

- declaring the complex variables

    ```c
    complexnum c1 = complexnum_default;
    intializeComplexDefaultLibrary(&c1, "(3,4)");
    complexnum c2 = complexnum_default;
    intializeComplexDefaultLibrary(&c2, "(-6.01,8)");
    bignum ans = bignum_default;
    ```

- complex_add stores the **ADD** result in ans.

    ```c
    int complex_add(complexnum *c1, complexnum *c2, complexnum *ans);
    ```

- complex_sub stores the **SUB** result in ans.

    ```c
    int complex_sub(complexnum *c1, complexnum *c2, complexnum *ans);
    ```

- complex_absolute stores the **ABSOLUTE** result in ans. Absolute of a complex number x+iy is √(x<sup>2</sup>+y<sup>2</sup>). You can pass *alsoTakeSqRoot* as 0, if you just want to calculate x<sup>2</sup>+y<sup>2</sup>

    ```c
    int complex_absolute(complexnum *c1, bignum* ans, int precision, int roundOff, int alsoTakeSqRoot);
    ```

    precision is the decimal digits upto which you should get the result and roundOff means that whether you should round off the least significant digit which getting ans truncated upto precision.

- complex_multiply stores the **MULT** result in ans.

    ```c
    int complex_multiply(complexnum *c1, complexnum *c2, complexnum *ans);
    ```

- complex_conjugate stores the **CONJUGATE** result in ans. Conjugate of a complex number x+iy is x-iy.
    
    ```c
    int complex_conjugate (complexnum *c1, complexnum *ans);
    ```

- complex_divide makes store **DIVISION** value in ans. i.e c1/c2 is stored in ans.

    ```c
    int complex_divide(complexnum *c1, complexnum *c2, complexnum *ans, int precision, int roundOff);
    ```

    Here also precision and roundOff means same as of absolute

- fixLeadingZeroInComplex removes leading zero from x and y bignum of complex number x+iy.

    ```c
    void fixLeadingZeroInComplex(complexnum *c1);
    ```

    If you use print_complex to print the complex and you see something like (001.3, 01.2) use fixLeadingZeroInComplex, then after printing you will see (1.3, 1.2)

- fixTrailingZeroInComplex removes trailing zero from x and y bignum of complex number x+iy. 

    ```c
    int fixTrailingZeroInComplex(complexnum *c1);
    ```

    If you use print_complex to print the complex and you see something like (1.300, 100.0) use fixLeadingZeroInComplex, then after printing you will see (1.3, 100)

- freeMallocSpaceInComplex is used to free the space allocated dynamically using malloc for complex numbers.

    ```c
    void freeMallocSpaceInComplex(complexnum *c1);
    ```

    if you create a complexnum variable using ```complexnum c1 = complexnum_default;```
    and then initialized it like ```intializeComplexDefaultLibrary(&c1, "(3,4)");```
    then you also need to free the malloc space allocared using ```freeMallocSpaceInComplex(&c1);```

- Can be used if you want to print the complex number x+ib in form (x,y) in the string called output.

    ```c
    void string_complex(complexnum *ci, char *output);
    ```

    To print the complex number, you can do this
    ```c
    char str[5000];
    string_complex(&ci, str);
    printf("%s", str);
    ```

- print_complex is used to directly print the complex number x+iy in form (x,y)

    ```c
    void print_complex(complexnum *ci);
    ```
    
### Errors
Some of the functions can return error code depending on which you have to give the output. Here are all the error codes that can be returned.

| Error Code | Error String | What can cause this error  |
| ------------- |-------------| -----|
| 0      | insufficient storage | malloc can cause this |
| 1      | string format wrong for bignum| "-" is not first alphabet, "." not 1 repeatition, other digits not "0 to 9". initializing complexnum, bignum can cause this|
| 3 | bignum can't be initialized from empty string | initializing complexnum, bignum can cause this |
| 4 | division by 0 is not allowed | complex_divide can cause this |
| 5 | currently power only works with integers in power | calculating power for bignum can cause this
| 6 | only integers factorial can be calculated | calculating bignum factorial for non integers can cause this
| 7 | number given more than #MAXSIZE | complexnum initialization from string can cause this
| 8 | string format is not correct | complexnum initialization from string not of format (a,b) can cause this

### Performace
How well this complex library using bignum is slower than default complex library in "complex.h". Using long double complex using default complex lib can also work but it will be accurate only upto 19 decimal places.
In example folder I have a example program where I have compared both and this is the graph I got

![Comparsion of complex implementation using mybignum vs default complex lib](https://i.imgur.com/dkDWKOF.png)

It works nice for add, sub for both large and small complex numbers
It works nice for small product numbers
but abs (x=3) and for divide (x = 5) even for small complex number it can be slow due to high precision (I have used precision upto 20 decimal).
Also for product of large complex numbers it can be slow (x = 8).
See example folder for my example implementation using my complexnum functions.

### FAQ:
**What is Arbitrary-Precision arithmetic**  
Arbitrary-Precision arithmetic, also known as "bignum" or simply "long arithmetic" is a set of data structures and algorithms which allows to process much greater numbers than can be fit in standard data types.
