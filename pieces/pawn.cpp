#include <iostream>

#include "piece.h"

using namespace std;


bool pawn::move(int xSource, int ySource, int xDest, int yDest, bool output, bool simulation, piece* (&board)[8][8]){
    cout << "Called pawn move!" << endl;
    
    if(!this->validateMove(xSource, ySource, xDest, yDest, output, board)){
        cout << "Invalid Move!" << endl;
        return false;
    }

    this->placePiece(xSource, ySource, xDest, yDest, simulation, board);
    return true;
}

bool pawn::validateMove(int xSource, int ySource, int xDest, int yDest, bool output, piece* (&board)[8][8]){
    int lowPos;
    int highPos;

    int xChange = abs(xSource - xDest);
    int yChange = abs(ySource - yDest);

    if(xChange + yChange > 2){
        if(output){
            cout << "Trying to move a pawn illegally!" << endl;
        }
        return false;
    }

    // Make sure we're not moving horizontally
    if(xSource == xDest){
        if(output){
            cout << "Trying to move a pawn horizontally!" << endl;
        }
        return false;
    }

    // Make sure we're not moving backwards
    if((this->myColor == 'r') && (xDest > xSource)){
        if(output){
            cout << "Trying to move pawn backwards!" << endl;
        }
        return false;
    } else if((this->myColor == 'w') && (xSource > xDest)){
        if(output){
            cout << "Trying to move pawn backwards!" << endl;
        }
        return false;
    }

    // Check to see if we're not moving straight
    if(ySource != yDest){
        // This means we're allegedly trying to take a piece
        // Check if we're still only moving one and if dest is opp piece
        if(yChange != 1){
            if(output){
                cout << "Trying to move pawn illegally!" << endl;
            }
            return false;
        }
        // Here is where we check for en passant
        if(board[xDest][yDest] == nullptr){
            bool isPassant = checkPassant(xSource, ySource, xDest, yDest, output, board);
            if(isPassant){
                return true;
            }
            else if(output && !isPassant){
                cout << "Trying to perform an invalid en passant!" << endl;
            } else if(output){
                cout << "Trying to take nonexistent piece!" << endl;
            }
            return false;
        }
        if(this->myColor == board[xDest][yDest]->myColor){
            if(output){
                cout << "Trying to take your own piece with a pawn!" << endl;
            }
            return false;
        }
        // We have confirmed this is a valid take
        return true;
    }
    
    // If we are moving straight
    // If we're trying to move forward to spaces make sure we're allowed to
    if((this->hasMoved == true) && (xChange > 1)){
        if(output){
            cout << "Selected pawn cannot move two spaces" << endl;
        }
        return false;
    }

    // when we're moving forward make sure we're not moving through or over any pieces
    lowPos = min(xSource, xDest) + 1;
    highPos = max(xSource, xDest);
    for(int i = lowPos; i <= highPos; i++){
        if((board[i][ySource] != nullptr) && (board[i][ySource] != board[xSource][ySource])){
            if(output){
                cout << "Trying to move pawn over another piece!" << endl;
            }
            return false;
        }
    }

    return true;
}

// TODO refactor to use this instead of curPiece
void pawn::placePiece(int xSource, int ySource, int xDest, int yDest, bool simulation, piece* (&board)[8][8]){
    piece *destPiece = board[xDest][yDest];
    if(!simulation){
        this->hasMoved = true;
        if(abs(xSource - xDest) == 2){
            this->lastMoveTwo = true;
        } else {
            this->lastMoveTwo = false;
        }
    }

    // Need to do some fancy footwork if we're performing an en passant
    bool isPassant = checkPassant(xSource, ySource, xDest, yDest, false, board);
    if(isPassant){
        int checkX;
        if(this->myColor == 'r'){
            checkX = 1;
        } else {
            checkX = -1;
        }
        delete board[xDest + checkX][yDest];
        board[xDest + checkX][yDest]= nullptr;
    } else {
        delete board[xDest][yDest];
    }

    board[xSource][ySource] = nullptr;
    board[xDest][yDest] = this;
    
    // Check to see if the pawn has made it to the far side of the board
    if(((this->myColor == 'r') && (xDest == 0)) || ((this->myColor == 'w') && (xDest == 7))){
        char upgradePiece;
        char curColor = this->myColor;
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

// red y goes from + to -
// white y goes from - to +
bool pawn::checkPassant(int xSource, int ySource, int xDest, int yDest, bool output, piece* (&board)[8][8]){
    // Check which direction we are going
    int checkX;
    if(this->myColor == 'r'){
        checkX = 1;
    } else {
        checkX = -1;
    }

    // Check to see if we are moving past a pawn and if it's en passantable
    pawn *passantablePawn = (pawn*)board[xDest + checkX][yDest];

    
    if(passantablePawn == nullptr){
        return false;
    }

    if((passantablePawn->lastMoveTwo) && (this->myColor != passantablePawn->myColor)){
        return true;
    }

    return false;
}