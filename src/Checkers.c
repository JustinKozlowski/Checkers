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
        char b[9] = "\0";
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
    free(out);
}

int checkPiece(struct piece *piece){
	if(piece->x > '8' || piece->x < '1' || piece->y > '8' || piece->y < '1'){
		return 0;
	}
	// Cheecks if both even or both odd, if note, invalid piece
	if(piece->x % 0x2){
		if(!(piece->y % 0x2)){
			return 0;
		}
	}
	else{
		if(piece->y % 0x2){
			return 0;
		}
	}
	return 1;
}

char pieceXChar(struct piece *piece){
	char out = 0x1;
	for(int x = piece->x - '8'; x < 0; ++x){
		out <<= 1;
	}
	return out;
}

int checkColor(struct gameState *game, struct piece *piece){ // Check if piece is in colors grids (pawn and king)
	char x = pieceXChar(piece);
	if(checkTurn(game, COLOR1)){
		if(game->prevStates[0].red.board[piece->y - '1'] & x){
			return 1;
		}
		if(game->prevStates[0].redKings.board[piece->y - '1'] & x){
			return 1;
		}
	}
	else{
		
		if(game->prevStates[0].black.board[piece->y - '1'] & x){
			return 1;
		}
		if(game->prevStates[0].blackKings.board[piece->y - '1'] & x){
			return 1;
		}
	}
	return 0;
}

int checkTurn(struct gameState *game, char *color){
	for(int i = 0; color[i] != '\0'; ++i){
		if(color[i] != game->colorTurn.board[i]){
			return 0;
		}
	}
	return 1;
}

int checkMove(struct gameState *game, struct piece *piece, struct piece *move){
	if(!checkPiece(piece) || !checkPiece(move) || !checkColor(game, piece)){
		return 0;
	}
	return 1;
}

int main(int argc, char** argv){ // test initialization
	struct gameState *game = gameInit();
	printf("%s\n", game->prevStates[0].red.board);
	printGame(game);
}

