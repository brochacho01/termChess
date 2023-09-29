#include <iostream>

#include "piece.h"

using namespace std;


bool king::move(int xSource, int ySource, int xDest, int yDest, bool output, bool simulation, piece* (&board)[BOARDSIZE][BOARDSIZE]){
    if(!this->validateMove(xSource, ySource, xDest, yDest, output, board)){
        cout << "Invalid Move!" << endl;
        return false;
    }
    this->placePiece(xSource, ySource, xDest, yDest, simulation, board);
    return true;
}

bool king::validateMove(int xSource, int ySource, int xDest, int yDest, bool output, piece* (&board)[BOARDSIZE][BOARDSIZE]){
    // Check to see if we're trying to castle
    if((board[xDest][yDest] != nullptr) && (this->myColor == board[xDest][yDest]->myColor) && (board[xDest][yDest]->myType == ROOK)){
        // Validate the attempted castle
        if(!validateCastle(xSource, ySource, xDest, yDest, output, board)){
            if(output){
                cout << "Invalid Castle!" << endl;
            }
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
        if(output){
            cout << "Can't move a king that way!" << endl;
        }
        return false;
    }

    // Make sure we're not trying to take our own piece
    if((board[xDest][yDest]) && (this->myColor == board[xDest][yDest]->myColor)){
        if(output){
            cout << "Trying to take your own piece with a king!" << endl;
        }
        return false;
    }

    return true;
}

bool king::validateCastle(int xSource, int ySource, int xDest, int yDest, bool output, piece* (&board)[BOARDSIZE][BOARDSIZE]){
    king *myKing = (king*)board[xSource][ySource];
    rook *myRook = (rook*)board[xDest][yDest];
    // Make sure neither king or rook have moved
    if(myKing->hasMoved){
        if(output){
            cout << "Cannot castle because your King has moved!" << endl;
        }
        return false;
    }

    if(myRook->hasMoved){
        if(output){
            cout << "Cannot castle because your Rook has moved!" << endl;
        }
        return false;
    }

    // Can't be in check and castle
    if(myKing->isCheck){
        if(output){
            cout << "Can't castle while in check!" << endl;
        }
        return false;
    }

    // Because we can go + or - need a mask
    int yIncrementMask = ((yDest - ySource) >> 31) | 1;

    // Check to see if there's any pieces between king and rook
    // Because neither piece can have moved we just need to check along one axis
    for(int i = ySource + yIncrementMask; i != yDest; i+= yIncrementMask){
        if(board[xSource][i] != nullptr){
            if(output){
                cout << "Cannot castle through a piece!" << endl;
            }
            return false;
        }
    }

    // Must also validate we're not castling through check
    int trueYDest;
    if(yIncrementMask > 0){
        trueYDest = LONGCASTLEDEST;
    } else {
        trueYDest = SHORTCASTLEDEST;
    }
    // Want to validate we're not in check from our starting position to our destination position
    for(int i = ySource; i != trueYDest + yIncrementMask; i += yIncrementMask){
        if(isChecking(xSource, ySource, xSource, i, this)){
            cout << "Cannot castle through check!" << endl;
            return false;
        }
    }
    if(output){
        cout << "Valid castle!" << endl;
    }

    return true;
}

// Need to update kingCoords on move
void king::placePiece(int xSource, int ySource, int xDest, int yDest, bool simulation, piece* (&board)[BOARDSIZE][BOARDSIZE]){
    board[xSource][ySource] = nullptr;

    // Check to see if we're placing for a castle, keep in mind we would have already validated this move
    if((board[xDest][yDest] != nullptr) && (board[xDest][yDest]->myType == ROOK) && (this->myColor == board[xDest][yDest]->myColor)){
        rook *myRook = (rook*)board[xDest][yDest];
        board[xDest][yDest] = nullptr;
        if(yDest - ySource > 0){
            board[xDest][LONGCASTLEDEST] = this;
            board[xDest][4] = myRook;
            this->position[1] = LONGCASTLEDEST;
        } else {
            board[xDest][SHORTCASTLEDEST] = this;
            board[xDest][2] = myRook;
            this->position[1] = SHORTCASTLEDEST;
        }
        
        // If we are merely simulating a move we don't want to update these statuses
        if(!simulation){
            myRook->hasMoved = true;
            this->hasMoved = true;
        }
    } else {
        if(!simulation){
            this->hasMoved = true;
        }
        delete board[xDest][yDest];
        board[xDest][yDest] = this;
        this->position[0] = xDest;
        this->position[1] = yDest;
    }
}

void king::printMySelf(void){
    cout << "Position[0] is: " << this->position[0] << endl;
    cout << "Position[1] is: " << this->position[1] << endl;
    cout << "isCheck is: " << this->isCheck << endl;
}