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
 *  @param int[] vettore ausiliario che tiene traccia degli elementi già scelti (no ripetizioni) (dim n)
 *  @param int dimensione n dell'insieme di oggetti
 *  @param int[] vettore dove tenere traccia delle scelte e comporre la soluzione (dim k)
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
 *  @param int[] vettore dove tenere traccia delle scelte e comporre la soluzione (dim k)
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

/**
 *  * Calcolo delle permutazioni semplici
 *  @param int indice di scelta da effettuare (lv di ricorsione)
 *  @param int[] vettore contente oggetti tra cui scegliere (dim n)
 *  @param int[] vettore ausiliario che tiene traccia degli elementi già scelti (no ripetizioni) (dim n)
 *  @param int[] vettore dove tenere traccia delle scelte e comporre la soluzione (dim n)
 *  @param int dimensione n dell'insieme di oggetti
 *  @param int contatore soluzioni identificate
 *  @return int contatore soluzioni aggiornato
 */
int permutazioniSemplici(int pos, int *val, int *mark, int *sol, int n, int cnt) {
    int i;

    // Terminal condition
    if (pos >= n) {
        // print found solution
        return cnt+1;
    }

    for (i = 0; i < n; i++) {
        if (mark[i] == 0) {
            mark[i] = 1;
            sol[pos] = val[i];
            cnt = permutazioniSemplici(pos+1, val, mark, sol, n, cnt);
            mark[i] = 0;
        }
    }

    return cnt;
}

/**
 *  * Calcolo delle permutazioni con ripetizione
 *  @param int indice di scelta da effettuare (lv di ricorsione)
 *  @param int[] vettore contente gli oggetti distinti tra cui scegliere (dim n_dist)
 *  @param int[] vettore ausiliario che tiene traccia delle ricorrenze degli oggetti e delle scelte già effettuate (dim n_dist)
 *  @param int[] vettore dove tenere traccia delle scelte e comporre la soluzione (dim n)
 *  @param int dimensione n della soluzione
 *  @param int contatore soluzioni identificate
 *  @return int contatore soluzioni aggiornato
 */
int permutazioniRipetute(int pos, int *val_dist, int *mark, int n_dist, int *sol, int n, int cnt) {
    int i;

    // Terminal condition
    if (pos >= n) {
        // print found solution
        return cnt+1;
    }

    for (i = 0; i < n_dist; i++) {
        if (mark[i] > 0) {
            mark[i]--;
            sol[pos] = val_dist[i];
            cnt = permutazioniRipetute(pos+1, val_dist, mark, n_dist, sol, n, cnt);
            mark[i]++;
        }
    }

    return cnt;
}
