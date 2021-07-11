#include <stdio.h>
#include <iostream>
#include <stdlib.h>
int main(int ac, char **av)
{
    int x = 1;
    printf("%llu %d\n", uint64_t(&x), x);
    new (&x) int(2);
    printf("%llu %d\n", uint64_t(&x), x);

    int *y = new int(3);
    printf("%llu %d\n", uint64_t(y), *y);
    int* z = (int*)malloc(sizeof(int));
    *z = 4;
    printf("%llu %d\n", uint64_t(z), *z);
    int* z1 = (int*)alloca(sizeof(int));
    *z1 = 5;
    printf("%llu %d\n", uint64_t(z1), *z1);
    return 0;
}