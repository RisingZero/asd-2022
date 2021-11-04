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
 *  @param Livello[] struttura dati contente info sui livelli di scelta (dim n)
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

/**
 *  * Calcolo delle disposizioni semplici
 *  @param int indice di scelta da effettuare (lv di ricorsione)
 *  @param int[] vettore contente oggetti tra cui scegliere (dim n)
 *  @param int[] vettore ausiliari che tiene traccia degli elementi già scelti (no ripetizioni) (dim n)
 *  @param int dimensione n dell'insieme di oggetti
 *  @param int[] vettore tenere traccia delle scelte e comporre la soluzione (dim k)
 *  @param int dimensione k della soluzione cercata
 *  @param int contatore soluzioni identificate
 *  @return int contatore soluzioni aggiornato
 */
int disposizioniSemplici(int pos, int *val, int *mark, int n, int *sol, int k, int cnt) {
    int i;

    // Terminal condition
    if (pos >= k) {
        // print found solution
        return cnt+1;
    }

    for (i = 0; i < n; i++){
        if (mark[i] == 0) {
            mark[i] = 1;
            sol[pos] = val[i];
            cnt = disposizioniSemplici(pos+1, val, mark, n, sol, k, cnt);
            mark[i] = 0;
        }
    }

    return cnt;
}

/**
 *  * Calcolo delle disposizioni ripetute
 *  @param int indice di scelta da effettuare (lv di ricorsione)
 *  @param int[] vettore contente oggetti tra cui scegliere (dim n)
 *  @param int dimensione n dell'insieme di oggetti
 *  @param int[] vettore tenere traccia delle scelte e comporre la soluzione (dim k)
 *  @param int dimensione k della soluzione cercata
 *  @param int contatore soluzioni identificate
 *  @return int contatore soluzioni aggiornato
 */
int disposizioniRipetute(int pos, int *val, int n, int *sol, int k, int cnt) {
    int i;

    // Terminal condition
    if (pos >= k) {
        // print found solution
        return cnt+1;
    }

    for (i = 0; i < n; i++) {
        sol[pos] = val[i];
        cnt = disposizioniRipetute(pos+1, val, n, sol, k, cnt);
    }

    return cnt;
}