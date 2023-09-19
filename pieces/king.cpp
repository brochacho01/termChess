#include <iostream>

#include "piece.h"
#include "../board.h"

using namespace std;

// RULES FOR CASTLING
// King can't have moved
// Rook can't have moved
// Can't be any pieces between king or rook
// King can't move through check when castling

bool king::move(int xSource, int ySource, int xDest, int yDest, piece ownKing){
    cout << "Called king move!" << endl;

    if(!this->validateMove(xSource, ySource, xDest, yDest)){
        cout << "Invalid Move!" << endl;
        return false;
    }

    this->placePiece(xSource, ySource, xDest, yDest);
    return true;
}

bool king::validateMove(int xSource, int ySource, int xDest, int yDest){
    // Check to see if we're trying to castle
    if((board[xDest][yDest] != nullptr) && (this->myColor == board[xDest][yDest]->myColor) && (board[xDest][yDest]->myType == ROOK)){
        // Validate the attempted castle
        if(!validateCastle(xSource, ySource, xDest, yDest)){
            cout << "Invalid Castle!" << endl;
            return false;
        } else {
        // We have a valid castle and we have verified the move so we can early return true here
            return true;
        }
    }

    int xChange = abs(xDest - xSource);
    int yChange = abs(yDest - ySource);

    // Just make sure we're staying within the king square
    if((xChange + yChange > 2) || (xChange + yChange == 0)){
        cout << "Can't move a king that way!" << endl;
        return false;
    }

    // Make sure we're not trying to take our own piece
    if((board[xDest][yDest]) && (this->myColor == board[xDest][yDest]->myColor)){
        cout << "Trying to take your own piece with a king!" << endl;
        return false;
    }

    return true;
}

// TODO check for moving through check
bool king::validateCastle(int xSource, int ySource, int xDest, int yDest){
    king *myKing = (king*)board[xSource][ySource];
    rook *myRook = (rook*)board[xSource][ySource];

    // Make sure neither king or rook have moved
    if(myKing->hasMoved){
        cout << "Cannot castle because your King has moved!" << endl;
        return false;
    }

    if(myRook->hasMoved){
        cout << "Cannot castle because your Rook has moved!" << endl;
        return false;
    }

    // Check to see if there's any pieces between king and rook
    // Because neither piece can have moved we just need to check along one axis
    for(int i = ySource + 1; i < yDest; i++){
        if(board[xSource][i] != nullptr){
            cout << "Cannot castle through a piece!" << endl;
            return false;
        }
    }

    return true;
}

// TODO don't delete rook when castling?
// Or always delete the piece but check distances to see if it was a castle move and just create a new rook in the proper place
void king::placePiece(int xSource, int ySource, int xDest, int yDest){
    king *curPiece = (king*)board[xSource][ySource];
    curPiece->hasMoved = true;
    delete board[xDest][yDest];
    board[xSource][ySource] = nullptr;
    board[xDest][yDest] = curPiece;
}

void king::printSelf(void){
    cout << "Position[0] is: " << this->position[0] << endl;
    cout << "Position[1] is: " << this->position[1] << endl;
    cout << "isCheck is: " << this->isCheck << endl;
    cout << "isCheckMate is: " << this->isCheckmate << endl;
}