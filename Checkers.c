#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "checkers.h"

struct gameState *gameInit(){ //initializes board statean meta data
    struct gameState *game = calloc(1, sizeof(struct gameState));
    strcpy(game->colorTurn.board, COLOR1);
    strcpy(game->repetition.board, "0");
    strcpy(game->progress.board, "0");
    strcpy(game->turnNum.board, "0");
    game->prevStates[0].red.board[5] =  0xaa; 
    game->prevStates[0].red.board[6] =  0x55; 
    game->prevStates[0].red.board[7] =  0xaa; /* initializes to 
                                                  00000000
                                                  00000000
                                                  00000000
                                                  00000000
                                                  00000000
                                                  10101010
                                                  01010101
                                                  10101010*/
    game->prevStates[0].black.board[0] =  0x55; 
    game->prevStates[0].black.board[1] =  0xaa; 
    game->prevStates[0].black.board[2] =  0x55; /* initializes to 
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
    struct printState *out = calloc(1, sizeof(struct printState));
    for(int y = 0; y < 8; y++){
        char b[8] = "\0";
        for (int z = 128; z > 0; z >>= 1){ //Comares each bit in the char and appends bit to b
            strcat(b, ((x.board[y] & z) == z) ? "1" : "0");
        }
        strcpy(out->gameBoard[y].board, b); //Writes b to a row in out
    }
    return out;
}

void printGame(struct gameState *game){
    struct printState *out = calloc(1, sizeof(struct printState));
    //Parse individual layer from binary grid to char grid
    struct printState *red = boardBinary(game->prevStates[0].red);
    struct printState *redKings = boardBinary(game->prevStates[0].redKings);
    struct printState *black = boardBinary(game->prevStates[0].black);
    struct printState *blackKings = boardBinary(game->prevStates[0].blackKings);
    int emptyCell = 1; // Empty cell to create checker pattern
    for(int x = 0; x < 8; x++){ // Parse grids to singular grid
        for(int y = 0; y < 8; y++){ //parses layers and puts to out grid, assumes no overlapping pieces
            if(emptyCell){
                out->gameBoard[x].board[y] = CHECKER;
            }
            else if(red->gameBoard[x].board[y] == '1'){
                out->gameBoard[x].board[y] = RED;
            }
            else if(redKings->gameBoard[x].board[y] == '1'){
                out->gameBoard[x].board[y] = REDKING;
            }
            else if(black->gameBoard[x].board[y] == '1'){
                out->gameBoard[x].board[y] = BLACK;
            }
            else if(blackKings->gameBoard[x].board[y] == '1'){
                out->gameBoard[x].board[y] = BLACKKING;
            }
            else{
                out->gameBoard[x].board[y] = EMPTY;
            }
            emptyCell = !emptyCell;
        }
        emptyCell = !emptyCell;
    }
    for(int x = 0; x < 8; x++){ // Prints parsed gameBoard
        printf("-----------------\n");
        for(int y = 0; y < 8; y++){
            printf("|");
            printf("%c", out->gameBoard[x].board[y]);
        }
        printf("|\n");
    }
    printf("-----------------\n");
    free(red);
    free(redKings);
    free(black);
    free(blackKings);
}

int main(int argc, char* argv){ // test initialization
	struct gameState *game = gameInit();
	printf("%s\n", game->prevStates[0].red.board);
	printGame(game);
}

