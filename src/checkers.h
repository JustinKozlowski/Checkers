#pragma once

#define BLACK 'b'
#define RED 'r'
#define BLACKKING 'B'
#define REDKING 'R'
#define EMPTY ' '
#define CHECKER ' '
#define COLOR1 "Red"
#define COLOR2 "Black"

//-----GAME STRUCTURES-----//
struct grid { //store states as binary in a single char
	char board[8];
};

struct boardState { //one grid for each type of piece in game
	struct grid red;
	struct grid redKings;
	struct grid black;
	struct grid blackKings;
};

struct gameState{ //store previous 6 board states, along with meta data
	struct boardState prevStates[7];
	struct grid colorTurn;
	struct grid repetition;
	struct grid progress;
	struct grid turnNum;
};

struct piece{ // Keeps track of a location on the game board
	char x; // column for the board. 0 is left of board
	char y; // row on the board. 0 is top of board
};

//Struct used for printing gameboard//
struct printState{
	struct grid gameBoard[8];
};

//---Initialization functions---//
struct gameState *gameInit();   
void printGame(struct gameState *game); //Prints the state of the current game onto terminal
struct printState *boardBinary(struct grid x); //Convers binary grid in x to a char grid for printing
int checkMove(struct gameState *game, struct piece *piece, struct piece *move); // Checks if move is a valid move
int checkPiece(struct piece *piece); // check if is possible to be a piece
char pieceXChar(struct piece *piece); // converts int of x to a binary repressentation of the x (00010000)
int checkColor(struct gameState *game, struct piece *piece); // checks if piece selected exists in colors turn
int checkTurn(struct gameState *game, char *color); // checks of turn matches color turn

