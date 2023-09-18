#include <iostream>

#include "piece.h"
#include "../board.h"

using namespace std;


bool pawn::move(int xSource, int ySource, int xDest, int yDest, piece ownKing){
    cout << "Called pawn move!" << endl;
    cout << "xSource: " << xSource << " ySource: " << ySource << " xDest: " << xDest << " yDest: " << yDest << endl;
    
    if(!this->validateMove(xSource, ySource, xDest, yDest, ownKing)){
        cout << "Invalid Move!" << endl;
        return false;
    }

    this->placePiece(xSource, ySource, xDest, yDest);
    return true;
}

// TODO take logic for en-passant
bool pawn::validateMove(int xSource, int ySource, int xDest, int yDest, piece ownKing){
    int lowPos;
    int highPos;

    // Make sure we're not moving horizontally
    if(xSource == xDest){
        cout << "Trying to move a pawn horizontally!" << endl;
        return false;
    }

    // Make sure we're not moving backwards
    if((this->myColor == 'r') && (xDest > xSource)){
        cout << "Trying to move pawn backwards!" << endl;
        return false;
    } else if((this->myColor == 'w') && (xSource > xDest)){
        cout << "Trying to move pawn backwards!" << endl;
        return false;
    }

    // Check to see if we're not moving straight
    if(ySource != yDest){
        // This means we're allegedly trying to take a piece
        // Check if we're still only moving one and if dest is opp piece
        if(abs(ySource - yDest) != 1){
            cout << "Trying to move pawn illegally!" << endl;
            return false;
        }
        if(board[xDest][yDest] == nullptr){
            cout << "Trying to take nonexistent piece!" << endl;
            return false;
        }
        if(this->myColor == board[xDest][yDest]->myColor){
            cout << "Trying to take your own piece with a pawn!" << endl;
            return false;
        }
        // We have confirmed this is a valid take
        return true;
    }
    
    // If we are moving straight
    // If we're trying to move forward to spaces make sure we're allowed to
    if((this->hasMoved == true) && abs((xSource - xDest) > 1)){
        cout << "Selected pawn cannot move two spaces" << endl;
        return false;
    }

    // when we're moving forward make sure we're not moving through or over any pieces
    lowPos = min(xSource, xDest) + 1;
    highPos = max(xSource, xDest);
    for(int i = lowPos; i <= highPos; i++){
        if((board[i][ySource] != nullptr) && (board[i][ySource] != board[xSource][ySource])){
            cout << "Trying to move pawn over another piece!" << endl;
            return false;
        }
    }

    return true;
}

// TODO if pawn reaches far side need the upgrade logic
void pawn::placePiece(int xSource, int ySource, int xDest, int yDest){
    pawn *curPiece = (pawn*)board[xSource][ySource];
    curPiece->hasMoved = true;
    if(abs(xSource - xDest) == 2){
        curPiece->lastMoveTwo = true;
    } else {
        curPiece->lastMoveTwo = false;
    }
    delete board[xDest][yDest];
    board[xSource][ySource] = nullptr;
    board[xDest][yDest] = curPiece;
    
    // Check to see if the pawn has made it to the far side of the board
    if(((curPiece->myColor == 'r') && (xDest == 0)) || ((curPiece->myColor == 'w') && (xDest == 7))){
        char upgradePiece;
        char curColor = curPiece->myColor;
        bool validChoice = false;
        delete board[xDest][yDest];
        while(!validChoice){
            cout << "What piece would you like to upgrade the pawn to? (r/n/b/q) ";
            
            cin >> upgradePiece;
            switch(upgradePiece){
                case ROOK:
                {
                    rook *newRook = new rook(curColor);
                    board[xDest][yDest] = newRook;
                    validChoice = true;
                    break;
                }
                case KNIGHT:
                {
                    knight *newKnight = new knight(curColor);
                    board[xDest][yDest] = newKnight;
                    validChoice = true;
                    break;
                }
                case BISHOP:
                {
                    bishop *newBishop = new bishop(curColor);
                    board[xDest][yDest] = newBishop;
                    validChoice = true;
                    break;
                }
                case QUEEN:
                {
                    queen *newQueen = new queen(curColor);
                    board[xDest][yDest] = newQueen;
                    validChoice = true;
                    break;
                }
                default:
                    cout << "Unrecognized piece name, please try again!" << endl;
                    break;
            }
        }
        while(getchar() != '\n');
        
    }
}