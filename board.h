#ifndef BOARD
#define BOARD

// Included here because function declarations need to know about the piece types
#include "pieces/piece.h"

void printMyBoard(char myColor, piece*(&boardToPrint)[BOARDSIZE][BOARDSIZE]);
void createBoard(char *myColor, king *&myKing, king *&oppKing);
void copyBoard(piece* (&boardCopy)[BOARDSIZE][BOARDSIZE]);
void cleanBoard(piece* (&myBoard)[BOARDSIZE][BOARDSIZE]);

extern piece* board[BOARDSIZE][BOARDSIZE];
#endif