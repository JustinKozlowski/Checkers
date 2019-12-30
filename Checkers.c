#include <stdint.h>
#include <stdlib.h>
#include <string.h>


struct grid {
    char board[8];
};

struct boardState {
    struct grid red;
    struct grid redKings;
    struct grid black;
    struct grid blackKing;
};

struct gameState{
    struct boardState prevStates[7];
    struct grid colorTurn;
    struct grid repetition;
    struct grid progress;
    struct grid turnNum;
};

struct printState{
    struct grid gameBoard[8];
}

struct gameState gameInit();
void printState(struct gameState game);

struct gameState gameInit(){
    struct gameState *game = calloc(1, sizeof(gameState));
    strcpy(game->colorTurn.board, "red");
    strcpy(game->repetition.board, "0");
    strcpy(game->progress.board, "0");
    strcpy(game->turnNum.board, "0");
    strcpy(game->prevStates[0].red, "\0\0\0\0\0ªUª"); /* initializes to 
                                                  00000000
                                                  00000000
                                                  00000000
                                                  00000000
                                                  00000000
                                                  10101010
                                                  01010101
                                                  10101010*/
    strcpy(game->prevStates[0].black, "UªU"); /* initializes to 
                                                  01010101
                                                  10101010
                                                  01010101
                                                  00000000
                                                  00000000
                                                  00000000
                                                  00000000
                                                  00000000*/
    return game;
}

struct printState *boardBinary(struct grid x) {
    struct grid out [8];
    for(int y = 0; y < 8, y++){
        static char b[8];
        for (int z = 128; z > 0; z >>= 1){
            strcat(b, ((x[y] & z) == z) ? "1" : "0");
        }
        out[y] = b;
    }
    return out;
}

void printSate(struct gameState game){
    struct printState *out = calloc(1, sizeof(printState));

}