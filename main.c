#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

void print_numbers(int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", i);
    }
    printf("\n");
}

int main() {
    printf("hello i am me\n");

    int sum = add(3, 4);
    printf("Sum: %d\n", sum);

    print_numbers(5);

    return 0;
}
