#include <stdint.h>
#include <stdio.h>
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
};

struct gameState *gameInit();
void printGame(struct gameState *game);
struct printState *boardBinary(struct grid x);

struct gameState *gameInit(){ //initializes board statean meta data
    struct gameState *game = calloc(1, sizeof(struct gameState));
    strcpy(game->colorTurn.board, "red");
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
	printf("%s", b);
	printf("%x", x.board[y] & 0xff);
        for (int z = 128; z > 0; z >>= 1){
            strcat(b, ((x.board[y] & z) == z) ? "1" : "0");
        }
	//printf("%s\n", b);
        strcpy(out->gameBoard[y].board, b);
	//printf("%s\n", out->gameBoard[y].board);
    }
    printf("\n");
    return out;
}

void printGame(struct gameState *game){
    struct printState *out = calloc(1, sizeof(struct printState));
    printf("red:");
    struct printState *red = boardBinary(game->prevStates[0].red);
    printf("redKings:");
    struct printState *redKings = boardBinary(game->prevStates[0].redKings);
    printf("black:");
    struct printState *black = boardBinary(game->prevStates[0].black);
    printf("blackKings:");
    struct printState *blackKings = boardBinary(game->prevStates[0].blackKings);
    int emptyCell = 1;
    for(int x = 0; x < 8; x++){ // Parse grids to singular grid
        for(int y = 0; y < 8; y++){
	    //printf("red char: %c\n", red->gameBoard[x].board[y]);
            if(emptyCell){
                out->gameBoard[x].board[y] = ' ';
            }
            else if(red->gameBoard[x].board[y] == '1'){
                out->gameBoard[x].board[y] = 'r';
            }
            else if(redKings->gameBoard[x].board[y] == '1'){
                out->gameBoard[x].board[y] = 'R';
            }
            else if(black->gameBoard[x].board[y] == '1'){
                out->gameBoard[x].board[y] = 'b';
            }
            else if(blackKings->gameBoard[x].board[y] == '1'){
                out->gameBoard[x].board[y] = 'B';
            }
            else{
                out->gameBoard[x].board[y] = ' ';
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
}

int main(int argc, char* argv){
	struct gameState *game = gameInit();
	printf("%s\n", game->prevStates[0].red.board);
	printGame(game);
}
