#include <iostream>

#include "piece.h"
#include "../board.h"

using namespace std;

void copyBoard(piece* (&boardCopy)[8][8]);


// TODO VERIFY THIS ALL WORKS

// Make a deep copy of the array
// Simulate the move the player is trying to make
// Then go over the entire board and see if a piece can take the king
// We need the deep copy because move validation requires checking the board for existence of pieces in the path

// Because entries in these arrays are objects with allocated space I want to be very careful to not free any memory during this check as that would mess with the original board
bool isChecking(int xSource, int ySource, int xDest, int yDest, king* theKing){
    piece *boardCopy[8][8];
    piece *boardRevert[8][8];
    copyBoard(boardCopy);

    piece *movePiece = boardCopy[xSource][ySource];
    boardCopy[xSource][ySource] = nullptr;
    boardCopy[xDest][yDest] = nullptr;
    boardCopy[xDest][yDest] = movePiece;

    cout << "Testing the move, printing temp board after moving piece" << endl;
    printTestBoard(boardCopy);


    // Now we check to see if any piece can put the king in check
    int kingX = theKing->position[0];
    int kingY = theKing->position[1];

    cout << "Kingx: " << kingX << " kingY: " << kingY << endl;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            piece *curPiece = boardCopy[i][j];

            // Don't want to check type of nullptr
            if(curPiece == nullptr){
                continue;
            }

            // For any piece we find see if we can move it onto the square that theKing is occupying, if so, that means the king is in check and we return true
            switch(curPiece->myType){
                case PAWN:
                    if(((pawn*)curPiece)->validateMove(i, j, kingX, kingY, false, boardCopy)){
                        cout << "Check from pawn" << endl;
                        return true;
                    }
                    break;
                case ROOK:
                    if(((rook*)curPiece)->validateMove(i, j, kingX, kingY, false, boardCopy)){
                        cout << "Check from rook" << endl;
                        return true;
                    }
                    break;
                case KNIGHT:
                    if(((knight*)curPiece)->validateMove(i, j, kingX, kingY, false, boardCopy)){
                        cout << "Check from knight" << endl;
                        return true;
                    }
                    break;
                case BISHOP:
                    if(((bishop*)curPiece)->validateMove(i, j, kingX, kingY, false, boardCopy)){
                        cout << "Check from bishop" << endl;
                        return true;
                    }
                    break;
                case QUEEN:
                    if(((queen*)curPiece)->validateMove(i, j, kingX, kingY, false, boardCopy)){
                        cout << "Check from queen" << endl;
                        return true;
                    }
                    break;
                case KING:
                    if(((king*)curPiece)->validateMove(i, j, kingX, kingY, false, board)){
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

void copyBoard(piece* (&boardCopy)[8][8]){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
           boardCopy[i][j] = board[i][j]; 
        }
    }
}