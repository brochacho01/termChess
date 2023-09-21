#include <iostream>

#include "piece.h"

using namespace std;

// TODO check for checks

bool bishop::move(int xSource, int ySource, int xDest, int yDest, bool output, piece* (&board)[8][8]){
    cout << "Called bishop move!" << endl;
    
    if(!this->validateMove(xSource, ySource, xDest, yDest, output, board)){
        cout << "Invalid Move!" << endl;
        return false;
    }

    this->placePiece(xSource, ySource, xDest, yDest, board);
    return true;
}

bool bishop::validateMove(int xSource, int ySource, int xDest, int yDest, bool output, piece* (&board)[8][8]){
    // Check to see if we're moving diagonally
    int xChange = abs(xDest - xSource);
    int yChange = abs(yDest - ySource);

    if((yChange == 0) || (xChange) == 0){
        if(output){
            cout << "Trying to move bishop non-diagonally!" << endl;
        }
        return false;
    }

    float slope = (float)xChange / (float)yChange;

    if(slope != 1){
        if(output){
            cout << "Trying to move bishop non-diagonally!" << endl;
        }
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
            if(output){
                cout << "Trying to move bishop over another piece!" << endl;
            }
            return false;
        }
    }

    // Make sure we're not trying to take our own piece
    if((board[xDest][yDest] != nullptr) && (this->myColor == board[xDest][yDest]->myColor)){
        if(output){
            cout << "Trying to take your own piece with a bishop!" << endl;
        }
        return false;
    }

    return true;
}

void bishop::placePiece(int xSource, int ySource, int xDest, int yDest, piece* (&board)[8][8]){
    bishop *curPiece = (bishop*)board[xSource][ySource];

    delete board[xDest][yDest];
    board[xSource][ySource] = nullptr;
    board[xDest][yDest] = curPiece;
}