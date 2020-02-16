#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bignum.h"
#include "complexnum.h"
#include <complex.h>

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif

#include <time.h>

#ifndef MAXSIZE
#define MAXSIZE 500001
#endif

#define MAX_INSTRUCTIONS 1000

void printErrorAndExitProgram(int e) {
    if (e == 0) {
        return;
    } else if (e == 1) {
        printf("error: insufficient storage");
    } else if (e == 2) {
        printf("error: string format wrong: -(first alphabet) or . (1 repeatition) or 0 to 9");
    } else if (e == 3) {
        printf("error: bignum can't be initialized from empty string");
    } else if (e == 4) {
        printf("error: division by 0 is not allowed");
    } else if (e == 5) {
        printf("error: currently power only works with integers in power");
    } else if (e == 6) {
        printf("error: only integers factorial can be calculated");
    } else if (e == 7) {
        printf("error: number given more than MAXSIZE");
    } else if (e == 8) {
        printf("error: string format is not correct");
    }
    exit(1);
}

void parseSpace(char *cmd, char **argv) {
    int len = strlen(cmd);
    if (cmd[len - 1] == '\n') {
        len--;
        cmd[len] = '\0'; // so as to avoid getting \n
    }

    int bracket = 0;
    for (int i = 0; i < len; i++) {
        if (cmd[i] == ' ') {
            if (bracket) cmd[i] = '\n'; // will restore later
        } else if (cmd[i] == '(' || cmd[i] == ')') {
            bracket = !bracket;
        }
    }

    char *str = strtok(cmd, " ");
    int i = 0;
    while (str != NULL) {
        argv[i] = str;
        str = strtok(NULL, " ");
        i += 1;
    }
    argv[i] = NULL;
}

void execute(char *tokens[], char *output, int precision, int roundingOff) {
    int e = 0;
    complexnum c1 = complexnum_default;
    bignum ansbignum = bignum_default;

    if (strcmp(tokens[0], "ABS") == 0) {
        e = initialize_complexnum(&c1, tokens[1]);
        printErrorAndExitProgram(e);
        e = complex_absolute(&c1, &ansbignum, precision, roundingOff, 1);
        printErrorAndExitProgram(e);
        if (ansbignum.decimal > (precision)) {
            bignum temp2 = bignum_default;
            e = copyBignum(&ansbignum, &temp2, ansbignum.size - ansbignum.decimal + precision, roundingOff);
            printErrorAndExitProgram(e);
            e = copyBignum(&temp2, &ansbignum, -1, 0);
            printErrorAndExitProgram(e);
            freeMallocSpace(&temp2);
        }
        string_bignum(&ansbignum, output);
        freeMallocSpaceInComplex(&c1);
        freeMallocSpace(&ansbignum);
        return;
    }

    complexnum c2 = complexnum_default;
    complexnum ans = complexnum_default;

    if (strcmp(tokens[0], "ADD") == 0) {
        e = initialize_complexnum(&c1, tokens[1]);
        printErrorAndExitProgram(e);
        e = initialize_complexnum(&c2, tokens[2]);
        printErrorAndExitProgram(e);
        e = complex_add(&c1, &c2, &ans);
    } else if (strcmp(tokens[0], "SUB") == 0) {
        e = initialize_complexnum(&c1, tokens[1]);
        printErrorAndExitProgram(e);
        e = initialize_complexnum(&c2, tokens[2]);
        printErrorAndExitProgram(e);
        e = complex_sub(&c1, &c2, &ans);
    } else if (strcmp(tokens[0], "PROD") == 0) {
        e = initialize_complexnum(&c1, tokens[1]);
        printErrorAndExitProgram(e);
        e = initialize_complexnum(&c2, tokens[2]);
        printErrorAndExitProgram(e);
        e = complex_multiply(&c1, &c2, &ans);
    } else if (strcmp(tokens[0], "QUOT") == 0) {
        e = initialize_complexnum(&c1, tokens[1]);
        printErrorAndExitProgram(e);
        e = initialize_complexnum(&c2, tokens[2]);
        printErrorAndExitProgram(e);
        e = complex_divide(&c1, &c2, &ans, precision, roundingOff);
    }
    printErrorAndExitProgram(e);
    bignum temp2 = bignum_default;
    if (ans.real.decimal > (precision)) {
        e = copyBignum(&(ans.real), &temp2, ans.real.size - ans.real.decimal + precision, roundingOff);
        printErrorAndExitProgram(e);
        e = copyBignum(&temp2, &(ans.real), -1, 0);
        printErrorAndExitProgram(e);
    }
    if (ans.imaginary.decimal > (precision)) {
        e = copyBignum(&(ans.imaginary), &temp2, ans.imaginary.size - ans.imaginary.decimal + precision, roundingOff);
        printErrorAndExitProgram(e);
        e = copyBignum(&temp2, &(ans.imaginary), -1, 0);
        printErrorAndExitProgram(e);
    }
    freeMallocSpace(&temp2);
    fixLeadingZeroInComplex(&ans);
    e = fixTrailingZeroInComplex(&ans);
    printErrorAndExitProgram(e);
    string_complex(&ans, output);
    freeMallocSpaceInComplex(&c1);
    freeMallocSpaceInComplex(&c2);
    freeMallocSpaceInComplex(&ans);
}

void intializeComplexDefaultLibrary(long double complex *c, char *str) {
    *c = 0.0 + 0.0 * I;
    char seperated[2][MAXSIZE];
    int error = parseComplexNoStr(str, seperated);
    if (error == 0) {
        *c += strtold(seperated[0], NULL);
        *c += (strtold(seperated[1], NULL)) * I;
    } else if (error == 1) {
        printf("error: number given more than %d", MAXSIZE);
        exit(1);
    } else if (error == 2) {
        printf("error: string format is not correct");
        exit(1);
    }
}

void executeUsingDefaultLibrary(char *tokens[], char *output, int precision, int roundingOff) {
    long double complex c1 = 0.0 + 0.0 * I;
    long double ansd;
    if (strcmp(tokens[0], "ABS") == 0) {
        intializeComplexDefaultLibrary(&c1, tokens[1]);

        // printf("real %Lf \n", creall(c1));
        // printf("imaginary %Lf \n", cimagl(c1));

        c1 = c1 * conj(c1);

        ansd = csqrtl(creall(c1));

        snprintf(output, MAXSIZE, "By Default Operation is %.2Lf \n", ansd);
        return;
    }
    long double complex c2 = 0.0 - 0.0 * I;
    long double complex ans = 0.0 - 0.0 * I;
    if (strcmp(tokens[0], "ADD") == 0) {
        intializeComplexDefaultLibrary(&c1, tokens[1]);

        // printf("real %Lf \n", creall(c1));
        // printf("imaginary %Lf \n", cimagl(c1));

        intializeComplexDefaultLibrary(&c2, tokens[2]);
        ans = c1 + c2;
    } else if (strcmp(tokens[0], "SUB") == 0) {
        intializeComplexDefaultLibrary(&c1, tokens[1]);
        intializeComplexDefaultLibrary(&c2, tokens[2]);
        ans = c1 - c2;
    } else if (strcmp(tokens[0], "PROD") == 0) {
        intializeComplexDefaultLibrary(&c1, tokens[1]);
        intializeComplexDefaultLibrary(&c2, tokens[2]);
        ans = c1 * c2;
    } else if (strcmp(tokens[0], "QUOT") == 0) {
        intializeComplexDefaultLibrary(&c1, tokens[1]);
        intializeComplexDefaultLibrary(&c2, tokens[2]);
        ans = c1 / c2;
    }
    snprintf(output, MAXSIZE, "By Default Operation is (%.2Lf,%.2Lf) \n", creall(ans), cimagl(ans));
}

int main(int argc, char *argv[]) {

    if (argc < 4) {
        printf("error: pass arguments of input file and output file");
        exit(1);
    }

    // for measuring time taken
    struct timespec start, end;
    double timeTakenByMyLib[MAX_INSTRUCTIONS];
    double timeTakenByDefaultLib[MAX_INSTRUCTIONS];
    int t = 0;


    FILE *outputFilePointer;
    char dataToBeWritten[MAXSIZE];
    char dataToBeWrittenByDefault[MAXSIZE];
    outputFilePointer = fopen(argv[2], "w");

    FILE *inputFilePointer;
    char dataToBeRead[MAXSIZE];
    char *tokens[30];
    inputFilePointer = fopen(
            argv[1],
            "r");
    if (inputFilePointer == NULL || outputFilePointer == NULL) {
        printf("error: input file failed to open");
    } else {
        while (fgets(dataToBeRead, 50000, inputFilePointer) != NULL) {
            // Print the dataToBeRead
            // printf("%s", dataToBeRead);
            parseSpace(dataToBeRead, tokens);

            clock_gettime(CLOCK_MONOTONIC, &start);
            execute(tokens, dataToBeWritten, 20, 0);
            clock_gettime(CLOCK_MONOTONIC, &end);
            timeTakenByMyLib[t] = (end.tv_sec - start.tv_sec) * 1e9;
            timeTakenByMyLib[t] = (timeTakenByMyLib[t] + (end.tv_nsec - start.tv_nsec)) * 1e-9;


            clock_gettime(CLOCK_MONOTONIC, &start);
            executeUsingDefaultLibrary(tokens, dataToBeWrittenByDefault, 20, 0);
            clock_gettime(CLOCK_MONOTONIC, &end);
            timeTakenByDefaultLib[t] = (end.tv_sec - start.tv_sec) * 1e9;
            timeTakenByDefaultLib[t] = (timeTakenByDefaultLib[t] + (end.tv_nsec - start.tv_nsec)) * 1e-9;
            t += 1;

            printf("%s", dataToBeWrittenByDefault);
            printf("%s", dataToBeWritten);
            fputs(dataToBeWrittenByDefault, outputFilePointer);
            fputs(dataToBeWritten, outputFilePointer);
        }
        fclose(inputFilePointer);
        fclose(outputFilePointer);

        FILE *temp1 = fopen("data.temp", "w");
        FILE *gnuplotPipe = popen("gnuplot -persistent", "w");

        char secondCommand[1000] = "set output '";
        strcat(secondCommand, argv[3]);
        strcat(secondCommand, "';");

        char *commandsForGnuplot[] = {"set terminal png size 500,500;", secondCommand, "set border 2+4+8", "set boxwidth 0.8",
                                      "set style data histograms", "set style fill solid 1.0", "set xzeroaxis lt 0",
                                      "set ylabel 'Execution Time (ns)'",
                                      "set title 'Comparision of my bignum library with default library'",
                                      "plot 'data.temp' using 2:xtic(1) title 'Default Complex lib' lt rgb '#40FF00', '' using 3 title 'Using my bignum' lt rgb '#406090' ",
                                      NULL};
        int i;
        for (i = 0; i < t; i++) {
            fprintf(temp1, "%d %f %f \n", i, timeTakenByDefaultLib[i],  timeTakenByMyLib[i]); //Write the data to a temporary1 file
            // fprintf(temp2, "%d %f \n", i, ); //Write the data to a temporary2 file
        }
        for (i = 0; commandsForGnuplot[i] != NULL; i++) {
            fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
        }
        fclose(temp1);
        fclose(gnuplotPipe);
    }
    return 0;
}
