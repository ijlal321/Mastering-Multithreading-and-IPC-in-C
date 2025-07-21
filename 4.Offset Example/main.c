#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


// Q. Given a struct, find offset of a field using a macro

// sol 
#define findOffset(structName, field)  (size_t)&((structName* )0) -> field

typedef struct {
    char name [30];
    int age;
    double gender;
    unsigned int worth;
} Person;


int main()
{
    printf("Offset of a: %zu\n", findOffset( Person, name));  // 0
    printf("Offset of a: %zu\n", findOffset( Person, age));  // 32
    printf("Offset of a: %zu\n", findOffset( Person, gender));  // 40
    printf("Offset of a: %zu\n", sizeof(int));  // 4
}

/* 
    vv imp
    why we saw 32, not 30 ? why we say 40, not 36 ?
    compiler does something called padding ? why ?
    if int is 4 byte, then it must be stored at an address divisible by 4 (like 0, 4, 8, ...), 
        thus after 30 byte of char, we need 2 byte of padding.
    same as in, after int, we are at address 36, but double is 8 byte, and 36 not divisible by 
    8. thus we put 4 byte padding.

    its all is for sompiler. for easy memory addressing, fetching etc.
    it was compiler all along.
*/