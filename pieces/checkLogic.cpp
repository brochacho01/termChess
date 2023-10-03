#include <iostream>

#include "piece.h"
#include "../board.h"

using namespace std;

// Make a deep copy of the array
// Simulate the move the player is trying to make
// Then go over the entire board and see if a piece can take the king

// Because entries in these arrays are objects with allocated space I want to be very careful to not free any memory during this check as that would mess with the original board

// Need to check to see if the piece we're grabbing is our king so that we can update temporary coordinates properly
bool isMoveChecking(int xSource, int ySource, int xDest, int yDest, king* theKing){
    int oldKingX;
    int oldKingY;
    bool isKingCheck;
    piece *boardCopy[BOARDSIZE][BOARDSIZE];
    copyBoard(boardCopy);

    piece *movePiece = boardCopy[xSource][ySource];

    if((movePiece->myType == KING) && (movePiece->myColor == theKing->myColor)){
        oldKingX = theKing->position[0];
        oldKingY = theKing->position[1];
        theKing->position[0] = xDest;
        theKing->position[1] = yDest;
    }

    // Move the piece
    boardCopy[xSource][ySource] = nullptr;
    boardCopy[xDest][yDest] = movePiece;

    // Now we check to see if any piece can put the king in check
    int kingX = theKing->position[0];
    int kingY = theKing->position[1];

    isKingCheck = isCheck(theKing, boardCopy);

    // If we changed king coords we need to reset them
    if((movePiece->myType == 'k') && (movePiece->myColor == theKing->myColor)){
                    theKing->position[0] = oldKingX;
                    theKing->position[1] = oldKingY;
    }

    return isKingCheck;
}


bool isCheck(king* theKing, piece* (&theBoard)[BOARDSIZE][BOARDSIZE]){
    int kingX = theKing->position[0];
    int kingY = theKing->position[1];
    for(int i = 0; i < BOARDSIZE; i++){
        for(int j = 0; j < BOARDSIZE; j++){
            piece *curPiece = theBoard[i][j];

            // Don't want to check type of nullptr
            if(curPiece == nullptr){
                continue;
            }

            // Make sure piece is of opposite color
            if(curPiece->myColor == theKing->myColor){
                continue;
            }

            // For any piece we find see if we can move it onto the square that theKing is occupying, if so, that means the king is in check and we return true
            switch(curPiece->myType){
                case PAWN:
                    if(((pawn*)curPiece)->validateMove(i, j, kingX, kingY, false, theBoard)){
                        cout << "Check from pawn" << endl;
                        return true;
                    }
                    break;
                case ROOK:
                    if(((rook*)curPiece)->validateMove(i, j, kingX, kingY, false, theBoard)){
                        cout << "Check from rook" << endl;
                        return true;
                    }
                    break;
                case KNIGHT:
                    if(((knight*)curPiece)->validateMove(i, j, kingX, kingY, false, theBoard)){
                        cout << "Check from knight" << endl;
                        return true;
                    }
                    break;
                case BISHOP:
                    if(((bishop*)curPiece)->validateMove(i, j, kingX, kingY, false, theBoard)){
                        cout << "Check from bishop" << endl;
                        return true;
                    }
                    break;
                case QUEEN:
                    if(((queen*)curPiece)->validateMove(i, j, kingX, kingY, false, theBoard)){
                        cout << "Check from queen" << endl;
                        return true;
                    }
                    break;
                case KING:
                    if(((king*)curPiece)->validateMove(i, j, kingX, kingY, false, theBoard)){
                        cout << "Check from king" << endl;
                        return true;
                    }
                    break;
                default:
                    break;
            }
        }
    }
    return false;
}