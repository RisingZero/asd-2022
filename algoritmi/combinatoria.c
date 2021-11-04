/* 
    ASD 2021-2022
    ALGORITMI PER MODELLI DI CALCOLO COMBINATORIO
 */

typedef struct livello_s {
    int num_scelte;
    int *scelte;
} Livello;

/**
 *  * Principio moltiplicativo del calcolo combinatorio:
 *  * CALCOLO NUMERO SOLUZIONI
 *  @param int indice di scelta da effettuare (lv di ricorsione)
 *  @param Livello[] strttura dati contente info sui livelli di scelta (dim n)
 *  @param int[] vettore tenere traccia delle scelte e comporre la soluzione (dim n)
 *  @param int dimensione della soluzione cercata
 *  @param int contatore soluzioni identificate
 *  @return int contatore soluzioni aggiornato
 */
int principioMoltiplicativo(int pos, Livello *val, int *sol, int n, int cnt) {
    int i;

    // Terminal condition
    if (pos >= n) {
        // print found solution
        return cnt+1;
    }

    for (i = 0; i < val[pos].num_scelte; i++) {
        sol[pos] = val[pos].scelte[i];
        cnt = principioMoltiplicativo(pos+1, val, sol, n, cnt);
    }

    return cnt;
}