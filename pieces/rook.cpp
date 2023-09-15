#include <iostream>
#include "piece.h"
#include "../board.h"

using namespace std;

// TODO for rook moving, currently code doesn't check if piece is going from positive to negative, only works if going positive to positive
bool rook::move(int xSource, int ySource, int xDest, int yDest, piece ownKing){
    int xChange = 0;
    int yChange = 0;
    
    // Check to see if we're moving horizontally/vertically
    if(((xSource != xDest) && (ySource != yDest)) || ((xSource == xDest) && (ySource == yDest))){
        cout << "Trying to move rook illegally!" << endl;
        return false;
    }
    // Check to see if we're staying on the board
    if((xDest < 0) || (xDest > 7) || (yDest < 0) || (yDest > 7)){
        cout << "Trying to move rook off the board!" << endl;
        return false;
    }
    // Check to see that we're not trying to jump any pieces or take our own piece
    
    if(xSource != xDest){
        xChange = 1;
    }
    if(xChange){
        for(int i = xSource; i <= xDest; i++){
            if((board[yDest][i] != nullptr) && (i != xDest)){
                cout << "Trying to move rook over a piece!" << endl;
                return false;
            }
            else if((i == xDest) && (this->myColor == board[yDest][xDest]->myColor)){
                cout << "Trying to take your own piece with a rook!" << endl;
                return false;
            }
        }
    } else {
        for(int i = ySource; i <= ySource; i++){
            if((board[i][xDest] != nullptr) && (i != yDest)){
                cout << "Trying to move rook over a piece!" << endl;
                return false;
            }
            else if((i == yDest) && (this->myColor == board[yDest][xDest]->myColor)){
                cout << "Trying to take your own piece with a rook!" << endl;
                return false;
            }
        }
    }
    // See if moving rook will place own king in check
    // See if we are putting opposing king in check
    return true;
}