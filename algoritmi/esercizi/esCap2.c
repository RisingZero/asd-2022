#include <stdio.h>

int sommaCifre(int n) {
    int x;

    if (n < 10)
        return n;

    x = n%10 + sommaCifre(n/10);
    return x;
}

int divisibile(int n, int m) {
    int res = n - m;

    if (res == 0) return 1;
    if (res < 0) return -1;
    return divisibile(res, m);
}

void reversePrint(int A[], int N) {

    if (N == 0)
        return;

    reversePrint(A+1, N-1);
    printf("%d ", A[0]);
}

int main(int argc, char const *argv[])
{
    printf("%d\n", sommaCifre(234));
    printf("%d\n", divisibile(234, 2));

    int V[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    reversePrint(V, 9); printf("\n");
    return 0;
}
