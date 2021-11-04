/* 
    ASD 2021-2022
    ALGORITMI RICORSIVI
 */


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