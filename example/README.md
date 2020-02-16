# Example Program using Bignum Complex Library for C

### Description
This is a sample program in which I have a input.txt file having some queries and answers for those queries are stored in output.txt
You need gnuplot installed to be able to run this sample program.
Try
```
$ gnuplot
```
If it is not intalled, install it first

**Query Format:** Each line of input file corresponds to a query which is in the format: \<Operator\>\<SPACE\>(R1,C1)\<SPACE\>(R2,C2)
Where Ri= Real part and Ci=Imaginary part of the complex number.


here input.txt contains operators of type
```
ADD, SUB, PROD, QUOT, ABS
```
example input.txt file
```c
ADD (3,4) (4,9)
SUB (-6,8) (4,-4)
ABS (9,-7)
PROD (4,8) (7,4)
QUOT (2,3) (3,4)
```
then output.txt file will be
```c
(7,13)
(-10,12)
11.40175425099137979136
(-4,72)
(0.72,0.04)
```

### Running the programming
If you have terminal on the current directory, use
```
$ gcc -o mainfile main.c bignum.c complexnum.c
$ ./mainfile input.txt output.txt output.png
```

### Performace
How well this complex library using bignum is slower than default complex library in "complex.h". Using long double complex using default complex lib can also work but it will be accurate only upto 19 decimal places.
For the input.txt given in this folder file given this is the plot

![Comparsion of complex implementation using mybignum vs default complex lib](https://i.imgur.com/dkDWKOF.png)

It works nice for add, sub for both large and small complex numbers
It works nice for small product numbers
but abs (x=3) and for divide (x = 5) even for small complex number it can be slow due to high precision (I have used precision upto 20 decimal).
Also for product of large complex numbers it can be slow (x = 8).
See example folder for my example implementation using my complexnum functions.