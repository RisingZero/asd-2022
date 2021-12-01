/* 
    LAB05 ES02 RAINERI ANDREA ANGELO S280848
    ALGORITMI E STRUTTURE DATI
 */

#include <stdio.h>
#include <stdlib.h>

#define TILES_FILE "tiles.txt"
#define BOARD_FILE "board.txt"

typedef struct tile_s {
    int id, used;
    int V1, V2;
    char C1, C2;
} Tile;

typedef struct cell_s {
    int id;
    int rot;
} Cell;

void readTiles(char *file, Tile **tiles, int *N);
void readBoard(char *file, Cell ***board, int *R, int *C, Tile *tiles, int N);
void findBestConfiguration(Cell **board, int R, int C, Tile *tiles, int N);
void findBestConfigurationR(int pos, Cell **board, Cell **b_board, int R, int C, Tile *tiles, int N, int *b_point);
int calculatePoints(Cell **board, int R, int C, Tile *tiles, int N);
void showConfiguration(Cell **board, int R, int C, Tile *tiles, int N);

int main(int argc, char const *argv[])
{
    Cell **board;
    Tile *tiles;
    int R, C, N;

    readTiles(TILES_FILE, &tiles, &N);
    readBoard(BOARD_FILE, &board, &R, &C, tiles, N);

    findBestConfiguration(board, R, C, tiles, N);
    return 0;
}

void readTiles(char *file, Tile **tiles, int *N) {
    FILE *fp;
    int i;

    if (((fp = fopen(file, "r")) == NULL)) {
        printf("ERRORE apertura file input\n");
        exit(1);
    }

    fscanf(fp, "%d", N);
    *tiles = (Tile *) malloc(*N *sizeof(Tile));

    for (i = 0; i < *N; i++) {
        (*tiles)[i].id = i; (*tiles)[i].used = 0;
        fscanf(fp, "%c %d %c %d", &(*tiles)[i].C1, &(*tiles)[i].V1, &(*tiles)[i].C2, &(*tiles)[i].V2);
    }
}

void readBoard(char *file, Cell ***board, int *R, int *C, Tile *tiles, int N) {
    FILE *fp;
    int i, j;

    if ((fp = fopen(file, "r")) == NULL) {
        printf("ERRORE apertura file input\n");
        exit(1);
    }

    fscanf(fp, "%d %d", R, C);
    (*board) = (Cell **) malloc(*R * sizeof(Cell *));
    for (i = 0; i < *R; i++) {
        (*board)[i] = (Cell *) malloc(*C * sizeof(Cell));
        for (j = 0; j < *C; j++) {
            fscanf(fp, "%d/%d", &(*board)[i][j].id, &(*board)[i][j].rot);
            if ((*board)[i][j].id != -1)
                tiles[(*board)[i][j].id].id = 1;
        }
    }
}

void findBestConfiguration(Cell **board, int R, int C, Tile *tiles, int N) {
    int b_point = 0, i, j;
    Cell **b_board;

    // Create best configuration board and copy start configuration
    b_board = (Cell **) malloc(R * sizeof(Cell *));
    for (i = 0; i < R; i++) {
        b_board[i] = (Cell *) malloc(C * sizeof(Cell));
        for (j = 0; j < C; j++) {
            b_board[i][j] = board[i][j];
        }
    }

    findBestConfigurationR(0, board, b_board, R, C, tiles, N, &b_point);

    // Show found best configuration
    showConfiguration(b_board, R, C, tiles, N);
}

void findBestConfigurationR(int pos, Cell **board, Cell **b_board, int R, int C, Tile *tiles, int N, int *b_point) {
    int i, points;
    int r, c;

    if (pos >= (R-1)*(C-1)) {
        points = calculatePoints(board, R, C, tiles, N);
        printf("punti: %d\n", points);
        if (points > *b_point) {
            for (r = 0; r < R; r++) {
                for (c = 0; c < C; c++) {
                    b_board[r][c].id = board[r][c].id;
                    b_board[r][c].rot = board[r][c].rot;
                }
            }
        }
        return;
    }

    r = pos/C; c = pos%C;
    // Cell already filled by default start configuration
    if (board[r][c].id == -1 && board[r][c].rot == -1) {
        findBestConfigurationR(pos+1, board, b_board, R, C, tiles, N, b_point);
        return;
    }

    // Make choice
    for (i = 0; i < N; i++) {
        if (tiles[i].used == 0) {
            // Take element
            tiles[i].used = 1;
            board[r][c].id = tiles[i].id;
            // take no rotated
            board[r][c].rot = 0;
            findBestConfigurationR(pos+1, board, b_board, R, C, tiles, N, b_point);
            // take rotated
            board[r][c].rot = 1;
            findBestConfigurationR(pos+1, board, b_board, R, C, tiles, N, b_point);
            // backtrack
            tiles[i].used = 0;
        }
    }
}

// TODO: check function : always 0 result
int calculatePoints(Cell **board, int R, int C, Tile *tiles, int N) {
    int i, j, points = 0, partialPts, valid;
    char color;

    // Check rows
    for (i = 0; i < R; i++) {
        valid = 1; partialPts = 0;
        color = (board[i][0].rot == 0) ? tiles[board[i][0].id].C1 : tiles[board[i][0].id].C2;
        for (j = 0; j < C && valid; j++) {
            partialPts += ((board[i][j].rot == 0) ? tiles[board[i][j].id].V1 : tiles[board[i][j].id].V2);
            if (((board[i][j].rot == 0) ? tiles[board[i][j].id].C1 : tiles[board[i][j].id].C2) != color)
                valid = 0;
        }
        if (valid)
            points += partialPts;
    }

    // Check cols
    for (j = 0; j < C; j++) {
        valid = 1; partialPts = 0;
        color = (board[0][j].rot == 0) ? tiles[board[0][j].id].C2 : tiles[board[0][j].id].C1;
        for (i = 0; i < C && valid; i++) {
            partialPts += ((board[i][j].rot == 0) ? tiles[board[i][j].id].V2 : tiles[board[i][j].id].V1);
            if (((board[0][j].rot == 0) ? tiles[board[i][j].id].C2 : tiles[board[i][j].id].C1) != color)
                valid = 0;
        }
        if (valid)
            points += partialPts;
    }

    return points;
}

void showConfiguration(Cell **board, int R, int C, Tile *tiles, int N) {
    int i, j;
    
    for (i = 0; i < R; i++) {
        for (j = 0; j < C; j++) {
            printf("%d/%d ", board[i][j].id, board[i][j].rot);
        }
        printf("\n");
    }
}