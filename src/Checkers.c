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

//move logic
int checkEmpty(struct gameState *game, struct piece *piece){
	char x = pieceXChar(piece);
	if(game->prevStates[0].red.board[piece->y - '1'] & x){
		return 0;
	}
	if(game->prevStates[0].redKings.board[piece->y - '1'] & x){
		return 0;
	}
	if(game->prevStates[0].black.board[piece->y - '1'] & x){
		return 0;
	}
	if(game->prevStates[0].blackKings.board[piece->y - '1'] & x){
		return 0;
	}
	return 1;
}

int checkMove(struct gameState *game, struct piece *piece, struct piece *move){
	if(!checkPiece(piece) || !checkPiece(move) || !checkColor(game, piece)){
		return 0;
	}
	char piece_type = checkPieceType(game, piece);
	if(!checkEmpty(game, move)){ //makes sure move goes to empty space
		return 0;
	}
	char move_direction = checkDirection(game, piece, move);
	if(move_direction > DOWN_THRESHOLD & piece_type > KING_THRESHOLD){ //checks if not king and trying to move down
		return 0;
	}
	//checks if move is further than 1 spot away`
	if(check_if_jump(game, piece, move)){
		//checks if it is legal jump
		struct piece *jumped = check_valid_jump(game, piece, move, move_direction);
		if(jumped != NULL){
			//handle jump logic
			char jumped_type = checkPieceType(game, jumped);
			jump_piece(game, piece, move, piece_type, jumped, jumped_type);
			free(jumped);
		}
	}
	else{
		move_piece(game, piece, move, piece_type);
	}
	//check if piece moved to spot which promotes to king
	if(!checkKing(piece_type)){
		if(checkTurn(game, COLOR1)){
			if(move->y == '8'){
				make_king(game, piece, piece_type);
			}
		}
		else{
			if(move->y == '1'){
				make_king(game, piece, piece_type);
			}
		}
	}
	return 1;
}

void jump_piece(struct gameState *game, struct piece *piece, struct piece *move, char piece_type, struct piece *jumped, char jumped_type){
	remove_piece(game, piece, piece_type);
	remove_piece(game, jumped, jumped_type);
	add_piece(game, move, piece_type);

}

struct piece *check_valid_jump(struct gameState *game, struct piece *piece, struct piece *move, char move_dir){
	struct piece *compare_piece = calloc(1, sizeof(struct piece));
	if(move_dir == UP_RIGHT){
		compare_piece->x = piece->x + 2;
		compare_piece->y = piece->y + 2;
	}
	if(move_dir == DOWN_RIGHT){
		compare_piece->x = piece->x + 2;
		compare_piece->y = piece->y - 2;
	}
	if(move_dir == UP_LEFT){
		compare_piece->x = piece->x - 2;
		compare_piece->y = piece->y + 2;
	}
	if(move_dir == DOWN_LEFT){
		compare_piece->x = piece->x - 2;
		compare_piece->y = piece->y - 2;
	}
	//checks if moving piece is going to a valid spot after jumping
	if(!((compare_piece->x == move->x) && (compare_piece->y == move->y))){
		return NULL;
	}
	if(move_dir == UP_RIGHT){
		compare_piece->x--;
		compare_piece->y--;
	}
	if(move_dir == DOWN_RIGHT){
		compare_piece->x--;
		compare_piece->y++;
	}
	if(move_dir == UP_LEFT){
		compare_piece->x++;
		compare_piece->y--;
	}
	if(move_dir == DOWN_LEFT){
		compare_piece->x++;
		compare_piece->y++;
	}
	//check if jumped piece is other color
	char comp_x = pieceXChar(compare_piece);
	if(checkTurn(game, COLOR2)){
		if(game->prevStates[0].red.board[compare_piece->y - '1'] & comp_x){
			return compare_piece;
		}
		if(game->prevStates[0].redKings.board[compare_piece->y - '1'] & comp_x){
			return compare_piece;
		}
	}
	else{
		
		if(game->prevStates[0].black.board[compare_piece->y - '1'] & comp_x){
			return compare_piece;
		}
		if(game->prevStates[0].blackKings.board[compare_piece->y - '1'] & comp_x){
			return compare_piece;
		}
	}
	return NULL;
}

void move_piece(struct gameState *game, struct piece *piece, struct piece *move, char piece_type){
	remove_piece(game, piece, piece_type);
	add_piece(game,move, piece_type);
}


void make_king(struct gameState *game, struct piece *piece, char piece_type){
	remove_piece(game, piece, piece_type);
	if(piece_type == RED){
		add_piece(game, piece, REDKING);
	}
	else{
		add_piece(game, piece, BLACKKING);
	}
}

void remove_piece(struct gameState *game, struct piece *piece, char piece_type){
	char piece_x = pieceXChar(piece);
	char original = game->prevStates[0].black.board[piece->y - '1']; //need to use pointer to grid of the piece
	if(piece_type == RED){
		game->prevStates[0].red.board[piece->y - '1'] = game->prevStates[0].red.board[piece->y - '1'] ^ piece_x;
	}
	else if(piece_type = REDKING){
		game->prevStates[0].redKings.board[piece->y - '1'] = game->prevStates[0].redKings.board[piece->y - '1'] ^ piece_x;
	}
	else if(piece_type == BLACK){
		game->prevStates[0].black.board[piece->y - '1'] = game->prevStates[0].black.board[piece->y - '1'] ^ piece_x;
	}
	else if(piece_type == BLACKKING){
		game->prevStates[0].blackKings.board[piece->y - '1'] = game->prevStates[0].blackKings.board[piece->y - '1'] ^ piece_x;
	}
}

void add_piece(struct gameState *game, struct piece *piece, char piece_type){
	char piece_x = pieceXChar(piece);
	char original = game->prevStates[0].black.board[piece->y - '1']; //need to use pointer to grid of the piece
	if(piece_type == RED){
		game->prevStates[0].red.board[piece->y - '1'] = game->prevStates[0].red.board[piece->y - '1'] | piece_x;
	}
	else if(piece_type = REDKING){
		game->prevStates[0].redKings.board[piece->y - '1'] = game->prevStates[0].redKings.board[piece->y - '1'] | piece_x;
	}
	else if(piece_type == BLACK){
		game->prevStates[0].black.board[piece->y - '1'] = game->prevStates[0].black.board[piece->y - '1'] | piece_x;
	}
	else if(piece_type == BLACKKING){
		game->prevStates[0].blackKings.board[piece->y - '1'] = game->prevStates[0].blackKings.board[piece->y - '1'] | piece_x;
	}
}

int check_if_jump(struct gameState *game, struct piece *piece, struct piece *move){
	int xdist = piece->x - move->x;
	int ydist = piece->y - move->y;
	if(ydist*ydist*xdist*xdist > 1){ //is 1 if direct move, >1 if jump
		return 1;
	}
	return 0;
}

char checkDirection(struct gameState *game, struct piece *piece, struct piece *move){
	if(piece->y < move->y){
		if(piece->x < move->x){
			return UP_RIGHT;
		}
		else{
			return UP_LEFT;
		}
	}
	else{
		if(piece->x < move->x){
			return DOWN_RIGHT;
		}
		else{
			return DOWN_LEFT;
		}
	}
}

char checkPieceType(struct gameState *game, struct piece *piece){
	char xChar = pieceXChar(piece);
	for(int i = 0; i < 8; i++){
		if(game->prevStates[0].black.board[piece->y - '1'] & xChar){
			return BLACK;
		}
		if(game->prevStates[0].blackKings.board[piece->y - '1'] & xChar){
			return BLACKKING;
		}
		if(game->prevStates[0].red.board[piece->y - '1'] & xChar){
			return RED;
		}
		if(game->prevStates[0].redKings.board[piece->y - '1'] & xChar){
			return REDKING;
		}
	}
}

int checkKing(char piece_type){
	if(!(piece_type ^ REDKING & piece_type ^ BLACKKING)){ //Compares Kings characters to type
		return 1;
	}
	return 0;
}

int main(int argc, char** argv){ // test initialization
	struct gameState *game = gameInit();
	printGame(game);
	return 1;
}

