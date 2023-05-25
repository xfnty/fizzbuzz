#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#ifdef WIN32
#define PATH_DIV '\\'
#else
#define PATH_DIV '/'
#endif

#define BENCHMARK(code)\
    do {\
        clock_t start = clock();\
        {code}\
        printf("----- Finished in %f seconds -----\n", (float)(clock() - start) / CLOCKS_PER_SEC);\
    } while(0);


typedef struct fizzbuzz_s {
    const char* name;
    void (*func)(int n);
} fizzbuzz_t;


void typical_fizzbuzz(int m) {
    for (size_t i = 1; i < m; i++) {
        if (i % 3 == 0) {
            printf("Fizz");
        }
        else if (i % 5 == 0) {
            printf("Buzz");
        }
        else if (i % 15 == 0) {
            printf("FizzBuzz");
        }
        else {
            printf("%zu", i);
        }
        printf("\n");
    }
}


void single_if_fizzbuzz(int m) {
    for (size_t i = 1; i < m; i++) {
        unsigned char flags = ((i % 3 == 0) | ((i % 5 == 0) << 1)) & 0b00000011;
        switch (flags) {
            case 0b00000001:
                printf("Fizz");
                break;
            case 0b00000010:
                printf("Buzz");
                break;
            case 0b00000011:
                printf("FizzBuzz");
                break;
            default:
                printf("%zu", i);
        }
        printf("\n");
    }
}


// Source: https://github.com/Keith-S-Thompson/fizzbuzz-c/blob/master/fizzbuzz020.c
void duffs_fizzbuzz(int m) {
    int i = 1;
    switch (i % 15) {
        case 0:  do { puts("FizzBuzz"); i++;
        case 1:       printf("%d\n",    i++);
        case 2:       printf("%d\n",    i++);
        case 3:       puts("Fizz");     i++;
        case 4:       printf("%d\n",    i++);
        case 5:       puts("Buzz");     i++;
        case 6:       puts("Fizz");     i++;
        case 7:       printf("%d\n",    i++);
        case 8:       printf("%d\n",    i++);
        case 9:       puts("Fizz");     i++;
        case 10:      puts("Buzz");     i++;
        case 11:      printf("%d\n",    i++);
        case 12:      puts("Fizz");     i++;
        case 13:      printf("%d\n",    i++);
        case 14:      printf("%d\n",    i++);
                 } while(i < m);
    }
    printf("*I don't really understand this*\n");
}


typedef bool (*checker_f)(size_t i);
bool times_3(size_t i) { return i % 3 == 0; }
bool times_5(size_t i) { return i % 5 == 0; }
void variable_msg_fizzbuzz(int m) {
    const char* messages[] = { "Fizz", "Buzz" };
    checker_f checkers[] = { times_3, times_5 };

    for (size_t i = 1; i < m; i++) {
        bool printed_flag = false;

        for (int j = 0; j < sizeof(checkers)/sizeof(checkers[0]); j++) {
            if (checkers[j](i)) {
                printf("%s", messages[j]);
                printed_flag = true;
            }
        }

        if (!printed_flag)
            printf("%zu", i);

        printf("\n");
    }
}


typedef bool (*msg_f)(int n);
bool msg3(int n) { if (n % 3 == 0) { printf("Fizz"); return true; } return false; }
bool msg5(int n) { if (n % 5 == 0) { printf("Buzz"); return true; } return false; }

void variable_msg_v2_fizzbuzz(int m) {
    msg_f messages[] = { msg3, msg5 };

    for (size_t i = 1; i < m; i++) {
        bool printed_flag = false;
        for (int j = 0; j < sizeof(messages)/sizeof(messages[0]); j++)
            if (messages[j](i))
                printed_flag = true;

        if (!printed_flag)
            printf("%zu", i);

        printf("\n");
    }
}


void single_printf_fizzbuzz(int m) {
    const char* messages[] = {
        "%zu\n", "Fizz\n", "Buzz\n", "FizzBuzz\n"
    };

    for (size_t i = 1; i < m; i++) {
        printf(messages[((i % 3 == 0) | ((i % 5 == 0) << 1)) & 0b00000011], i);
    }
}


// Source post: https://dev.to/shensd/taking-a-look-at-an-odd-fizzbuzz-solution-3hca
// Author: wunkolo@github.com
void literally_single_printf_fizzbuzz(int m) {
    for (size_t i = 1; i < m; i++) {
        printf("%zu\n\0\0\0\0Fizz\n\0\0\0Buzz\n\0\0\0FizzBuzz\n" 
            + (((0x1241843 >> ((i % 15) * 2)) & 0b11) * 8), i);
    }
}


int main(int argc, char const *argv[])
{
    const fizzbuzz_t fizzbuzzes[] = {
        {"typical", typical_fizzbuzz},
        {"single if (bitflag switch)", single_if_fizzbuzz},
        {"variable message", variable_msg_fizzbuzz},
        {"variable message v2", variable_msg_v2_fizzbuzz},
        {"single printf", single_printf_fizzbuzz},
        {"literally single printf", literally_single_printf_fizzbuzz},
        {"Duff's device", duffs_fizzbuzz},
    };

    if (argc < 3) {
        printf("usage: %s N M\n", strrchr(argv[0], PATH_DIV) + 1);
        printf("where N is the algorithm number and M is the highest number being reached.\n");
        printf("Available algorithms are:\n");
        for (int i = 0; i < sizeof(fizzbuzzes)/sizeof(fizzbuzzes[0]); i++)
            printf("  %d: %s\n", i+1, fizzbuzzes[i].name);
        return 0;
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    if (n < 1 || n > sizeof(fizzbuzzes)/sizeof(fizzbuzzes[0])) {
        printf("N is out of range.");
        return -1;
    }
    if (m < 0) {
        printf("M should not be negative.");
        return -1;
    }

    BENCHMARK(
        fizzbuzzes[n-1].func(m);
    );

    return 0;
}
