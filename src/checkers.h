#pragma once

#define BLACK 'b'
#define RED 'r'
#define BLACKKING 'B'
#define REDKING 'R'
#define EMPTY ' '
#define CHECKER ' '
#define COLOR1 "Red"
#define COLOR2 "Black"
#define KING_THRESHOLD 'a' // character to check if less than, then king
#define UP_RIGHT 'R'
#define DOWN_RIGHT 'r'
#define UP_LEFT 'L'
#define DOWN_LEFT 'l'
#define DOWN_THRESHOLD 'a' //character to check if greater than, then moving down

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
//Prints the state of the current game onto terminal
void printGame(struct gameState *game); 

//Convers binary grid in x to a char grid for printing
struct printState *boardBinary(struct grid x); 

// Checks if move is a valid move
int checkMove(struct gameState *game, struct piece *piece, struct piece *move); 

// check if is possible to be a piece
int checkPiece(struct piece *piece); 

// converts int of x to a binary repressentation of the x (00010000)
char pieceXChar(struct piece *piece); 

// checks if piece selected exists in colors turn
int checkColor(struct gameState *game, struct piece *piece); 

// checks of turn matches color turd
int checkTurn(struct gameState *game, char *color); 

//Checks if king or pawn
char checkPieceType(struct gameState *game, struct piece *piece);

//Checks direction piece will move
char checkDirection(struct gameState *game, struct piece *piece, struct piece *move);

//Adds piece to board of specified type
void add_piece(struct gameState *game, struct piece *move, char piece_type);

//Removes piece of given type from board
void remove_piece(struct gameState *game, struct piece *piece, char piece_type);

//Makes a given piece into a king
void make_king(struct gameState *game, struct piece *piece, char piece_type);

//Checks if pieceis a King
int checkKing(char piece_type);

//Moves a piecefrom one spot to another
void move_piece(struct gameState *game, struct piece *piece, struct piece *move, char piece_type);

//Jumps a piece (removes jumped and moves piece)
void jump_piece(struct gameState *game, struct piece *piece, struct piece *move, char piece_type, struct piece *jumped, char jumped_type);

//Checks if the move is a jump
int check_if_jump(struct gameState *game, struct piece *piece, struct piece *move);

//Checks if the jump is valid
struct piece *check_valid_jump(struct gameState *game, struct piece *piece, struct piece *move, char move_dir);
