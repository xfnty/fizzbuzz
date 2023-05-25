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
        printf("Finished in %f seconds\n", (float)(clock() - start) / CLOCKS_PER_SEC);\
    } while(0);


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


int main(int argc, char const *argv[])
{
    if (argc <= 1) {
        printf("usage: %s N\n", strrchr(argv[0], PATH_DIV) + 1);
        return 0;
    }

    int m = atoi(argv[1]);

    BENCHMARK(
        for (int i = 1; i < m + 1; i++) {
            // typical_fizzbuzz(i);
            // single_if_fizzbuzz(i);
            // variable_msg_fizzbuzz(i);
            variable_msg_v2_fizzbuzz(i);
        }
    );

    return 0;
}
