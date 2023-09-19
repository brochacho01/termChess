#include <iostream>

#include "piece.h"
#include "../board.h"

using namespace std;

// TODO check for checks

bool bishop::move(int xSource, int ySource, int xDest, int yDest, piece ownKing){
    cout << "Called bishop move!" << endl;
    
    if(!this->validateMove(xSource, ySource, xDest, yDest, ownKing)){
        cout << "Invalid Move!" << endl;
        return false;
    }

    this->placePiece(xSource, ySource, xDest, yDest);
    return true;
}

bool bishop::validateMove(int xSource, int ySource, int xDest, int yDest, piece ownKing){
    // Check to see if we're moving diagonally
    int xChange = abs(xDest - xSource);
    int yChange = abs(yDest - ySource);

    if((yChange == 0) || (xChange) == 0){
        cout << "Trying to move bishop non-diagonally!" << endl;
        return false;
    }

    int slope = yChange / xChange;

    if(slope != 1){
        cout << "Trying to move bishop non-diagonally!" << endl;
        return false;
    }

    // Set up bitmasks for direction checks
    int iIncrementMask = ((xDest - xSource) >> 31) | 1;
    int jIncrementMask = ((yDest - ySource) >> 31) | 1;

    int i = xSource + iIncrementMask;
    int j = ySource + jIncrementMask;

    // Make sure we're not trying to move through a piece
    for(i,j; i != xDest; i+= iIncrementMask, j+= jIncrementMask){
        if(board[i][j] != nullptr){
            cout << "Trying to move bishop over another piece!" << endl;
            return false;
        }
    }

    // Make sure we're not trying to take our own piece
    if((board[xDest][yDest] != nullptr) && (this->myColor == board[xDest][yDest]->myColor)){
        cout << "Trying to take your own piece with a bishop!" << endl;
        return false;
    }

    return true;
}

void bishop::placePiece(int xSource, int ySource, int xDest, int yDest){
    bishop *curPiece = (bishop*)board[xSource][ySource];

    delete board[xDest][yDest];
    board[xSource][ySource] = nullptr;
    board[xDest][yDest] = curPiece;
}