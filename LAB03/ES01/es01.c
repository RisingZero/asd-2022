/* 
    LAB03 ES01 RAINERI ANDREA ANGELO S280848
    ALGORITMI E STRUTTURE DATI
 */

#include <stdio.h>

int majority(int *a, int N);

int main(int argc, char const *argv[]) {
    
    int v[] = {3, 3, 9, 4, 3, 5, 3};
    int N = 7;

    printf("%d", majority(v, N));
    return 0;
}

int majority(int *a, int N) {

    int m1, m2, q, i, cnt;
    
    if (N == 1) {
        return a[0];
    }
    
    q = N/2;
    m1 = majority(a, q);
    m2 = majority(a+q, N-q);

    if (m1 == -1 && m2 == -1) {
        return -1;
    }
    if (m1 == m2)
        return m1;
    if (m1 != -1) {
        for (i = 0, cnt = 0; i < N; i++) {
            if (a[i] == m1)
                cnt++;
        }
        if (cnt > N/2)
            return m1;
    }
    if (m2 != -1)  {
        for (i = 0, cnt = 0; i < N; i++) {
            if (a[i] == m2)
                cnt++;
        }
        if (cnt > N/2)
            return m2;
    }
    return -1;
}
