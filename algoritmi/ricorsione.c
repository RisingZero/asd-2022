/* 
    ASD 2021-2022
    ALGORITMI RICORSIVI
 */

#include <stdio.h>
#define N_dim 10

/**
 *  * MAX FUNCTION WRAPPER: 
 *  * Finds the max number inside the array
 *  @param int[] int array to search for max
 *  @param int dim of the array
 *  @return int max number inside the array
 */
int max(int A[], int N) {
    return maxR(A, 0, N-1);
}

/** 
 *  * FIND MAX OF ARRAY (recursive)
 *  @param int[] int array to search for max
 *  @param int left index of the subvector
 *  @param int right index of the subvector
 *  @return int max of the array 
 */
int maxR(int A[], int l, int r) {
    int m1, m2, q = (l+r)/2;

    // Terminal condition: max of array of dim 1 is the element itself
    if (l >= r) {
        return A[l];
    }

    // Recursive calls on subvectors
    m1 = maxR(A, l, q);
    m2 = maxR(A, q+1, r);

    // Combination of results
    if (m1 <= m2)
        return m2;
    else
        return m1;
}

/**
 *  * Binary Search
 *  @param int[] array to search for element
 *  @param int left index of the subvector
 *  @param int right index of the subvector
 *  @param int element to search
 *  @return int boolean indicating the success of the search
 */
int binarySearch(int array[], int l, int r, int k) {
    int q = (l+r)/2;

    // Terminal condition: array of dim 1, compare element with key
    if (l == r) {
        if (array[l] == k)
            return 1;
        else
            return 0;
    }

    // Recursive calls
    if (array[q] == k) {
        return 1;
    } else if (array[q] > k) {
        return binarySearch(array, l, q-1, k);
    } else {
        return binarySearch(array, q+1, r, k);
    }
}

/**
 *  * Hanoi Towers algorithm: moves N disks from src to dest
 *  @param int number of disks to move
 *  @param int src tower
 *  @param int dest tower
 *  @return --
 */
void hanoi(int N, int src, int dest) {
    int aux = 3 - (src+dest);

    // Terminal condition: 1 disk to move from src to dest
    if (N == 1) {
        printf("src %d -> dest %d\n", src, dest);
        return;
    }

    // Recursive calls
    hanoi(N-1, src, aux);
    printf("src %d -> dest %d\n", src, dest);
    hanoi(N-1, aux, dest);
}

int main(int argc, char const *argv[])
{
    int V[N_dim] = {1, 34, 2, 45, 6, 7, 8, 98, 124, 2};
    printf("Max: %d\n", max(V, N_dim));
    printf("%d\n", binarySearch(V, 0, N_dim-1, 124));
    
    hanoi(3, 0, 2);

    return 0;
}
