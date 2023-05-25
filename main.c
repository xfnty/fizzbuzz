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

void typical_fizzbuzz(int n) {
    if (n % 3 == 0) {
        printf("Fizz");
    }
    else if (n % 5 == 0) {
        printf("Buzz");
    }
    else if (n % 15 == 0) {
        printf("FizzBuzz");
    }
    else {
        printf("%d", n);
    }
}


void single_if_fizzbuzz(int n) {
    unsigned char flags = ((n % 3 == 0) | ((n % 5 == 0) << 1)) & 0b00000011;
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
            printf("%d", n);
    }
    printf("\n");
}


typedef bool (*checker_f)(int n);
bool times_3(int n) { return n % 3 == 0; }
bool times_5(int n) { return n % 3 == 0; }

void variable_msg_fizzbuzz(int n) {
    const char* messages[] = { "Fizz", "Buzz" };
    checker_f checkers[] = { times_3, times_5 };

    bool printed_flag = false;
    for (int i = 0; i < sizeof(checkers)/sizeof(checkers[0]); i++) {
        if (checkers[i](n)) {
            printf("%s", messages[i]);
            printed_flag = true;
        }
    }

    if (!printed_flag)
        printf("%d", n);

    printf("\n");
}


typedef bool (*msg_f)(int n);
bool msg3(int n) { if (n % 3 == 0) { printf("Fizz"); return true; } return false; }
bool msg5(int n) { if (n % 5 == 0) { printf("Buzz"); return true; } return false; }

void variable_msg_v2_fizzbuzz(int n) {
    msg_f messages[] = { msg3, msg5 };

    bool printed_flag = false;
    for (int i = 0; i < sizeof(messages)/sizeof(messages[0]); i++)
        if (messages[i](n))
            printed_flag = true;

    if (!printed_flag)
        printf("%d", n);

    printf("\n");
}


void single_printf_fizzbuzz(int n) {
    const char* messages[] = {
        "%d\n", "Fizz\n", "Buzz\n", "FizzBuzz\n"
    };

    printf(messages[((n % 3 == 0) | ((n % 5 == 0) << 1)) & 0b00000011], n);
}


// Source post: https://dev.to/shensd/taking-a-look-at-an-odd-fizzbuzz-solution-3hca
// Author: wunkolo@github.com
void literally_single_printf_fizzbuzz(int n) {
    size_t i = n; // Could be in the argument list
    printf("%zu\n\0\0\0\0Fizz\n\0\0\0Buzz\n\0\0\0FizzBuzz\n" 
        + (((0x1241843 >> ((i % 15) * 2)) & 0b11) * 8), i);
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
        for (int i = 1; i < m + 1; i++) {
            fizzbuzzes[n-1].func(i);
        }
    );

    return 0;
}
