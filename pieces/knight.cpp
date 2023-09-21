#include <iostream>

#include "piece.h"


using namespace std;

// TODO check logic
bool knight::move(int xSource, int ySource, int xDest, int yDest, bool output, piece* (&board)[8][8]){
    cout << "Called knight move!" << endl;
    if(!this->validateMove(xSource, ySource, xDest, yDest, output, board)){
        cout << "Invalid Move!" << endl;
        return false;
    }

    this->placePiece(xSource, ySource, xDest, yDest, board);
    return true;
}

bool knight::validateMove(int xSource, int ySource, int xDest, int yDest, bool output, piece* (&board)[8][8]){
    // First check that the coordinates are for a valid knight move
    int xChange = abs(xSource - xDest);
    int yChange = abs(ySource - yDest);

    if(((xChange != 2) || (yChange != 1)) && ((xChange != 1) || (yChange != 2))){
        if(output){
            cout << "Trying to move knight illegally!" << endl;
        }
        return false;
    }

    // Check to make sure we're not trying to take our own piece
    if((board[xDest][yDest] != nullptr) && (this->myColor == board[xDest][yDest]->myColor)){
        if(output){
            cout << "Trying to take your own piece with a knight!" << endl;
        }
        return false;
    }

    return true;
}

void knight::placePiece(int xSource, int ySource, int xDest, int yDest, piece* (&board)[8][8]){
    knight *curPiece = (knight*)board[xSource][ySource];

    delete board[xDest][yDest];
    board[xSource][ySource] = nullptr;
    board[xDest][yDest] = curPiece;
}