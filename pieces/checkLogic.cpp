#include <iostream>

#include "piece.h"
#include "../board.h"

using namespace std;

// Make a deep copy of the array
// Simulate the move the player is trying to make
// Then go over the entire board and see if a piece can take the king

// Because entries in these arrays are objects with allocated space I want to be very careful to not free any memory during this check as that would mess with the original board

// Need to check to see if the piece we're grabbing is our king so that we can update temporary coordinates properly
bool isChecking(int xSource, int ySource, int xDest, int yDest, king* theKing){
    int oldKingX;
    int oldKingY;
    piece *boardCopy[8][8];
    bool isCheck = false;
    copyBoard(boardCopy);

    piece *movePiece = boardCopy[xSource][ySource];
    if(movePiece == theKing){
        oldKingX = theKing->position[0];
        oldKingY = theKing->position[1];
        theKing->position[0] = xDest;
        theKing->position[1] = yDest;
    }

    boardCopy[xSource][ySource] = nullptr;
    boardCopy[xDest][yDest] = nullptr;
    boardCopy[xDest][yDest] = movePiece;

    // cout << "Testing the move, printing temp board after moving piece" << endl;
    // cout << "xSource: " << xSource << " ySource: " << ySource << " xDest: " << xDest << " yDest: " << yDest << endl;
    // printTestBoard(boardCopy);


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

            // Make sure piece is of opposite color
            if(curPiece->myColor == theKing->myColor){
                continue;
            }

            // For any piece we find see if we can move it onto the square that theKing is occupying, if so, that means the king is in check and we return true
            switch(curPiece->myType){
                case PAWN:
                    if(((pawn*)curPiece)->validateMove(i, j, kingX, kingY, false, boardCopy)){
                        cout << "Check from pawn" << endl;
                        isCheck = true;
                    }
                    break;
                case ROOK:
                    if(((rook*)curPiece)->validateMove(i, j, kingX, kingY, false, boardCopy)){
                        cout << "Check from rook" << endl;
                        isCheck = true;
                    }
                    break;
                case KNIGHT:
                    if(((knight*)curPiece)->validateMove(i, j, kingX, kingY, false, boardCopy)){
                        cout << "Check from knight" << endl;
                        isCheck = true;
                    }
                    break;
                case BISHOP:
                    if(((bishop*)curPiece)->validateMove(i, j, kingX, kingY, false, boardCopy)){
                        cout << "Check from bishop" << endl;
                        isCheck = true;
                    }
                    break;
                case QUEEN:
                    if(((queen*)curPiece)->validateMove(i, j, kingX, kingY, false, boardCopy)){
                        cout << "Check from queen" << endl;
                        isCheck = true;
                    }
                    break;
                case KING:
                    if(((king*)curPiece)->validateMove(i, j, kingX, kingY, false, boardCopy)){
                        cout << "Check from king" << endl;
                        isCheck = true;
                    }
                    break;
                default:
                    break;
            }

            if(isCheck){
                // If we changed king coords we need to reset them
                if(movePiece == theKing){
                    theKing->position[0] = oldKingX;
                    theKing->position[1] = oldKingY;
                }
                return true;
            }
        }
    }

    // IF we changed king coords we need to reset them
    if(movePiece == theKing){
                    theKing->position[0] = oldKingX;
                    theKing->position[1] = oldKingY;
    }

    return false;
}