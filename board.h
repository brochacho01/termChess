#ifndef BOARD
#define BOARD

// Included here because function declarations need to know about the piece types
#include "pieces/piece.h"

void printMyBoard(char myColor, piece*(&boardToPrint)[8][8]);
void createBoard(char *myColor, king *&myKing, king *&oppKing);
void copyBoard(piece* (&boardCopy)[8][8]);

extern piece* board[8][8];
#endif