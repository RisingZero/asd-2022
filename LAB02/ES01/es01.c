/* 
    LAB02 ES01 RAINERI ANDREA ANGELO S280848
    ALGORITMI E STRUTTURE DATI
 */

#include <stdio.h>

int gcd(int x, int y);

int main(void) {

    int a, b, res, continua = 1;
    do {
        printf("Inserisci due numeri tra i quali calcolare il gcd: ");
        if (scanf("%d %d", &a, &b) == 2) {
            res = gcd(a,b);
            printf("Il gcd tra %d e %d: %d\n", a, b, res);
        } else {
            continua = 0;
        }
    } while (continua);

    //TODO: fix terminazione programma
    return 0;
}

int gcd(int x, int y) {
    int a, b, temp;

    // Swap if x > y
    a = x; b = y;
    if (b > a) {
        temp = a;
        a = b;
        b = temp;
    }

    if (a%b == 0) {
        return b;
    } else if (a%2 == 0 && b%2 == 0) {
        return 2*gcd(a/2, b/2);
    } else if (a%2 == 0) {
        return gcd(a/2, b);
    } else if (b%2 == 0) {
        return gcd(a, b/2);
    } else {
        return gcd((a-b)/2, b);
    }

}
