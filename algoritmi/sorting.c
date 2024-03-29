/* 
    ASD 2021-2022
    ALGORITMI DI ORDINAMENTO
 */

#include <stdio.h>
#define N_dim 10

/**
 *  * MERGE SORT WRAPPER: 
 *  * Hides the recursive implementation of the algorithm to the client
 *  @param Item[] array to order (size N)
 *  @param Item[] auxiliary array used in merge function (size N)
 *  @param int size of arrays (number of items to order)
 *  @return --
 */
void MergeSort(int A[], int B[], int N) {
    int l=0, r=N-1;
    MergeSortR(A, B, l, r);
}

/**
 *  * MERGE SORT (recursive call)
 *  @param Item[] array of items to order
 *  @param Item[] auxiliary array used in merge function
 *  @param int left index of the subvector to order
 *  @param int right index of the subvector to order
 *  @return --
 */
void MergeSortR(int A[], int B[], int l, int r) {
    int q = (l+r)/2;

    // Terminal condition: subvector of dim <=1 already ordered
    if (l >= r)
        return;
    
    // Recursive calls: 2 subproblems of dim = dim/2
    MergeSortR(A, B, l, q);
    MergeSortR(A, B, q+1, r);

    // Recombination
    Merge(A, B, l, q, r);
}

/** 
 *  * MERGE FUNCTION: 
 *  * Merges two ordered vectors of dim N/2 into one ordered vector of dim N
 *  @param Item[] array containing the subvectors (dim N)
 *  @param Item[] auxiliary array (dim N)
 *  @param int left index of the first subvector
 *  @param int rigth index of the first subvector
 *  @param int rigth index of the second subvector
 *  @return --
 */
void Merge(int A[], int B[], int l, int q, int r) {
    int i, j, k;
    i = l; j = q+1;

    // Merge inside B
    for (k = l; k <= r; k++) {
        if (i > q)
            B[k] = A[j++];
        else if (j > r)
            B[k] = A[i++];
        else if (A[i] <= A[j])
            B[k] = A[i++];
        else
            B[k] = A[j++];
    }

    // Copy the new order into A
    for (k = l; k <= r; k++)
        A[k] = B[k];
}

/**
 *  * QUICK SORT WRAPPER: 
 *  * Hides the recursive implementation of the algorithm to the client
 *  @param Item[] array to order (size N)
 *  @param int size N of the array (number of items to order)
 *  @return --
 */
void QuickSort(int A[], int N) {
    int l=0, r=N-1;
    QuickSortR(A, l, r);
}

/**
 *  * QUICK SORT (recursive call)
 *  @param Item[] array of items to order
 *  @param int left index of the subvector to order
 *  @param int right index of the subvector to order
 *  @return --
 */
void QuickSortR(int A[], int l, int r) {
    int q;
    
    // Terminal condition: subvector of dim <=1 already ordered
    if (l >= r)
        return;

    // Divide
    q = partition(A, l, r);

    // Recursive calls on subproblems
    QuickSortR(A, l, q-1);
    QuickSortR(A, q+1, r);
}
/**
 *  * Partition function called by Quick Sort, divides the vector into two subvectors with a pivot element in its final position q
 *  @param Item[] array of items to divide
 *  @param int left index of the subvector to divide
 *  @param int right index of the subvector to divide
 * @return int index of the pivot element the vector is divided by
 */
int partition(int A[], int l, int r) {
    int i = l-1, j = r;
    int tmp;
    int x = A[r];

    for (; ;) {
        while(A[++i] < x);
        while(A[--j] > x)
            if (j == l)
                break;
        if (i >= j)
            break;

        // Swap to position elements into the two subvectors
        tmp = A[i];
        A[i] = A[j];
        A[j] = tmp;
    }
    // Positioning trhe pivot element in its final position
    tmp = A[i];
    A[i] = A[r];
    A[r] = tmp;
    
    return i;
}

int main(int argc, char const *argv[])
{   
    int V[N_dim]= {3, 65, 32, 1, 8, 6, 9, 123, 54, 2};
    /*   
    int aux[N_dim];
    MergeSort(V, aux, N_dim);
    */
    /* 
   QuickSort(V, N_dim);
    */
    for (int i=0; i<N_dim; i++) {
        printf("%d ", V[i]);
    }
    return 0;
}
