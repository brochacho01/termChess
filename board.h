#ifndef BOARD
#define BOARD

#include "pieces/piece.h"

void printMyBoard(char myColor);
void createBoard(char *myColor, king *&myKing, king *&oppKing);
void printTestBoard(piece* (&boardCopy)[8][8]);
void copyBoard(piece* (&boardCopy)[8][8]);

extern piece* board[8][8];
#endif