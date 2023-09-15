#include <iostream>
#include "piece.h"
#include "../board.h"

using namespace std;


void pawn::move(int xSource, int ySource, int xDest, int yDest, piece ownKing){
    cout << "Called move from pawn!" << endl;
    piece *curPiece = board[ySource][xSource];
    piece *destPiece = board[yDest][xDest];
    
    if(!validateMove(xSource, ySource, xDest, yDest, ownKing)){
        cout << "Invalid Move!" << endl;
        return;
    }

    placePiece(xSource, ySource, xDest, yDest);
}

bool pawn::validateMove(int xSource, int ySource, int xDest, int yDest, piece ownKing){
    // Check to see if we're not moving straight
    if(xSource != xDest){
        cout << "Trying to move a pawn illegally! ";
        return false;
    }
    else if((this->hasMoved == true) && abs((ySource - yDest) > 1)){
        cout << "Selected pawn cannot move two spaces" << endl;
    }

    return true;
}

void pawn::placePiece(int xSource, int ySource, int xDest, int yDest){
    piece *curPiece = board[ySource][xSource];
    piece *destPiece = board[yDest][xDest];
    cout << xDest << yDest << endl;
    delete board[yDest][xDest];
    board[ySource][xSource] = nullptr;
    board[yDest][xDest] = curPiece;
}