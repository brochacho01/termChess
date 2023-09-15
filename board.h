#ifndef BOARD
#define BOARD

#include "pieces/piece.h"

void printMyBoard(char myColor);
void createBoard(char *myColor, king *&myKing, king *&oppKing);

extern piece* board[8][8];
#endif