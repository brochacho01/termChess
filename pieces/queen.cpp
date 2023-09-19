#include <iostream>

#include "piece.h"
#include "../board.h"

using namespace std;

// TODO check for checks

bool queen::move(int xSource, int ySource, int xDest, int yDest, piece ownKing){
    cout << "Called queen move!" << endl;

    if(!this->validateMove(xSource, ySource, xDest, yDest, ownKing)){
        cout << "Invalid Move!" << endl;
        return false;
    }

    this->placePiece(xSource, ySource, xDest, yDest);
    return true;
}

bool queen::validateMove(int xSource, int ySource, int xDest, int yDest, piece ownKing){
    // Need to check if we're moving diagonally or horizontally/vertically
    int slope;
    int xChange = abs(xDest - xSource);
    int yChange = abs(yDest - ySource);

    if((xChange == 0) && (yChange == 0)){
        cout << "Trying to move queen to space that it is currently on!" << endl;
        return false;
    }

    if((xChange == 0) || (yChange == 0)){
        slope = 0;
    }


    // If slope is 0 we are not moving diagonally
    if(!slope){
        if(!validateStraight(xSource, ySource, xDest, yDest)){
            return false;
        }
    // Else, we are moving diagonally 
    } else {
        if(!validateDiagonal(xSource, ySource, xDest, yDest, xChange, yChange)){
            return false;
        }
    }

    // Make sure we're not trying to take our own piece
    if((board[xDest][yDest] != nullptr) && (this->myColor == board[xDest][yDest]->myColor)){
        cout << "Trying to take your own piece with a queen!" << endl;
        return false;
    }

    return true;
}

bool queen::validateStraight(int xSource, int ySource, int xDest, int yDest){
    int xChange = 0;
    int lowPos;
    int highPos;

    // Set up trackers for going low-high or high-low
    if(xSource != xDest){
        xChange = 1;
        lowPos = min(xSource, xDest);
        highPos = max(xSource, xDest);
    } else {
        lowPos = min(ySource, yDest);
        highPos = max(ySource, yDest);
    }

    // Check to see if we're moving over a piece
    // If we're moving along x
    if(xChange){
        for(int i = lowPos + 1; i < highPos; i++){
            if(board[i][yDest] != nullptr){
                cout << "Trying to move queen over a piece!" << endl;
                return false;
            }
        }
    // If we're moving along y
    } else {
        for(int i = lowPos + 1; i < highPos; i++){
            if(board[xDest][i] != nullptr){
                cout << "Trying to move queen over a piece!" << endl;
                return false;
            }
        }
    }

    return true;
}

bool queen::validateDiagonal(int xSource, int ySource, int xDest, int yDest, int xChange, int yChange){
    int slope = yChange / xChange;

    if(slope != 1){
        cout << "Trying to move queen diagonally with an illegal slope!" << endl;
        return false;
    }

    // Set up bitmasks for direction checks
    int iIncrementMask = ((xDest - xSource) >> 31) | 1;
    int jIncrementMask = ((yDest - ySource) >> 31) | 1;

    int i = xSource + iIncrementMask;
    int j = ySource + jIncrementMask;

    // Make sure we're not trying to move through a piece
    for(i,j; i != xDest; i += iIncrementMask, j += jIncrementMask){
        if(board[i][j] != nullptr){
            cout << "Trying to move queen over another piece!" << endl;
            return false;
        }
    }

    return true;
}

void queen::placePiece(int xSource, int ySource, int xDest, int yDest){
    queen *curPiece = (queen*)board[xSource][ySource];

    delete board[xDest][yDest];
    board[xSource][ySource] = nullptr;
    board[xDest][yDest] = curPiece;
}