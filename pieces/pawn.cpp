#include <iostream>
#include "piece.h"
#include "../board.h"

using namespace std;


bool pawn::move(int xSource, int ySource, int xDest, int yDest, piece ownKing){
    piece *curPiece = board[ySource][xSource];
    piece *destPiece = board[yDest][xDest];
    
    if(!validateMove(xSource, ySource, xDest, yDest, ownKing)){
        cout << "Invalid Move!" << endl;
        return false;
    }

    placePiece(xSource, ySource, xDest, yDest);
    return true;
}

bool pawn::validateMove(int xSource, int ySource, int xDest, int yDest, piece ownKing){
    // Check to see if we're not moving straight
    if(abs(xSource - xDest) > 1){
        // This means we're allegedly trying to take a piece
        // Check if we're still only moving one and if dest is opp piece
        if(abs(xSource - xDest) != 1){
            cout << "Trying to move pawn illegally!" << endl;
            return false;
        }
        if(this->myColor == board[yDest][xDest]->myColor){
            cout << "Trying to take your own piece with a pawn!" << endl;
        }
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