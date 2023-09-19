#include <iostream>

#include "piece.h"
#include "../board.h"

using namespace std;

void copyBoard(piece* (&boardCopy)[8][8]);

// Make a deep copy of the array
// Simulate the move the player is trying to make
// Then go over the entire board and see if a piece can take the king
// We need the deep copy because move validation requires checking the board for existence of pieces in the path

// Because entries in these arrays are objects with allocated space I want to be very careful to not free any memory during this check as that would mess with the original board
bool isChecking(int xSource, int ySource, int xDest, int yDest, king* theKing){
    piece *boardCopy[8][8];
    copyBoard(boardCopy);

    piece *movePiece = boardCopy[xSource][ySource];
    boardCopy[xSource][ySource] = nullptr;
    boardCopy[xDest][yDest] = nullptr;
    boardCopy[xDest][yDest] = movePiece;

    // Now we check to see if any piece can put the king in check
    int kingX = theKing->position[0];
    int kingY = theKing->position[1];

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            piece *curPiece = boardCopy[i][j];

            // Don't want to check type of nullptr
            if(curPiece == nullptr){
                continue;
            }

            switch(curPiece->myType){
                case PAWN:
                    if(((pawn*)curPiece)->validateMove(xSource, ySource, xDest, yDest, false)){
                        return true;
                    }
                    break;
                case ROOK:
                    break;
                case KNIGHT:
                    break;
                case BISHOP:
                    break;
                case QUEEN:
                    break;
                case KING:
                    break;
                default:
                    break;
            }
        }
    }

    return false;
}

void copyBoard(piece* (&boardCopy)[8][8]){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
           boardCopy[i][j] = board[i][j]; 
        }
    }
}