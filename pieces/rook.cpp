#include <iostream>
#include "piece.h"
#include "../board.h"

using namespace std;


// TODO
// See if moving rook will place own king in check
// See if we are putting opposing king in check

bool rook::move(int xSource, int ySource, int xDest, int yDest, piece ownKing){
    cout << "Called rook move!" << endl;

    if(!this->validateMove(xSource, ySource, xDest, yDest, true)){
        cout << "Invalid Move!" << endl;
        return false;
    }
    
    this->placePiece(xSource, ySource, xDest, yDest);
    return true;
}

bool rook::validateMove(int xSource, int ySource, int xDest, int yDest, bool output){
    int xChange = 0;
    int lowPos;
    int highPos;

    // Check to see if we're moving horizontally/vertically
    if(((xSource != xDest) && (ySource != yDest)) || ((xSource == xDest) && (ySource == yDest))){
        if(output){
            cout << "Trying to move rook illegally!" << endl;
        }
        return false;
    }
    
    // Set up trackers for going low-high or high-low
    if(xSource != xDest){
        xChange = 1;
        lowPos = min(xSource, xDest);
        highPos = max(xSource, xDest);
    } else {
        lowPos = min(ySource, yDest);
        highPos = max(ySource, yDest);
    }

    // Check to see if we're trying to move over a piece
    // If we're moving along x
    if(xChange){
        for(int i = lowPos + 1; i < highPos; i++){
            if(board[i][yDest] != nullptr){
                if(output){
                    cout << "Trying to move rook over a piece!" << endl;
                }
                return false;
            }
        }
    // If we're moving along y
    } else {
        for(int i = lowPos + 1; i < highPos; i++){
            if(board[xDest][i] != nullptr){
                if(output){
                    cout << "Trying to move rook over a piece!" << endl;
                }
                return false;
            }
        }
    }

    // Check to make sure we're not trying to take our own piece
    if((board[xDest][yDest] != nullptr) && this->myColor == board[xDest][yDest]->myColor){
        if(output){
            cout << "Trying to take your own piece with a rook!" << endl;
        }
        return false;
    }

    return true;
}

void rook::placePiece(int xSource, int ySource, int xDest, int yDest){
    rook *curPiece = (rook*)board[xSource][ySource];
    curPiece->hasMoved = true;
    delete board[xDest][yDest];
    board[xSource][ySource] = nullptr;
    board[xDest][yDest] = curPiece;
}