#ifndef BOARD
#define BOARD

#include <vector>


// Included here because function declarations need to know about the piece types
#include "pieces/piece.h"

void printMyBoard(char myColor, piece*(&boardToPrint)[BOARDSIZE][BOARDSIZE]);
void printMyBoardWithCaptures(char myColor, piece*(&boardToPrint)[BOARDSIZE][BOARDSIZE], std::vector<int> myCaptured, std::vector<int> oppCaptured);
void createBoard(char *myColor, king *&myKing, king *&oppKing);
void copyBoard(piece* (&boardCopy)[BOARDSIZE][BOARDSIZE]);
void cleanBoard(piece* (&myBoard)[BOARDSIZE][BOARDSIZE]);

extern piece* board[BOARDSIZE][BOARDSIZE];
#endif