#include <stdint.h>
#include <stdlib.h>
#include <string.h>


struct grid { //store states as binary in a single char
    char board[8];
};

struct boardState { //one grid for each type of piece in game
    struct grid red;
    struct grid redKings;
    struct grid black;
    struct grid blackKings;
};

struct gameState{ //store previous 6 board states, along with metat data
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
void printGame(struct gameState game);
struct printState *boardBinary(struct grid x);

struct gameState gameInit(){ //initializes board statean meta data
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

struct printState *boardBinary(struct grid x) { // Convers char to binary grid
    struct printState *out = calloc(1, sizeof(printState));
    for(int y = 0; y < 8, y++){
        static char b[8];
        for (int z = 128; z > 0; z >>= 1){
            strcat(b, ((x[y] & z) == z) ? "1" : "0");
        }
        out->gameBoard[y] = b;
    }
    return out;
}

void printGame(struct gameState game){
    struct printState *out = calloc(1, sizeof(printState));
    struct printState *red = calloc(1, sizeof(printState));
    struct printState *redKings = calloc(1, sizeof(printState));
    struct printState *black = calloc(1, sizeof(printState));
    struct printState *blackKings = calloc(1, sizeof(printState));
    red = boardBinary(game->prevStates[0].red);
    redKings = boardBinary(game->prevStates[0].redKings);
    black = boardBinary(game->prevStates[0].black);
    blackKings = boardBinary(game->prevStates[0].blackKings);
    int emptyCell = 1;
    for(int x = 0; x < 8; x++){ // Parse grids to singular grid
        for(int y = 0; y < 8; y++){
            if(emptyCell){
                out->gameBoard[x][y] = 'X';
            }
            else if(red->gameBoard[x][y]){
                out->gameBoard[x][y] = 'r';
            }
            else if(redKings->gameBoard[x][y]){
                out->gameBoard[x][y] = 'R';
            }
            else if(black->gameBoard[x][y]){
                out->gameBoard[x][y] = 'b';
            }
            else if(blackKings->gameBoard[x][y]){
                out->gameBoard[x][y] = 'B';
            }
            else{
                out->gameBoard[x][y] = ' ';
            }
            emptyCell = !emptyCell;
        }
    }


}