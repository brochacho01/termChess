#include <iostream>

#include <termios.h>
#include <stdio.h>
#include <sys/poll.h>

#include "networkUtilities.h"
#include "pieces/piece.h"
#include "board.h"
#include "gameHelpers.h"

using namespace std;

void gameLoop(int fd, char color, char connectType, bool output, bool preview, king *&myKing, king *&oppKing);

// TODO allow users to alter their settings
// TODO allow users to propose a draw

int main() {
    char connectType;
    char color;
    bool output;
    bool preview;
    int fd;
    king *myKing;
    king *oppKing;
    setup(&connectType, &color, &output, &preview, &fd, myKing, oppKing);
    printMyBoard(color);
    gameLoop(fd, color, connectType, output, preview, myKing, oppKing);
}


void gameLoop(int fd, char color, char connectType, bool output, bool preview, king *&myKing, king *&oppKing){
    if(color == 'r'){
        char userBuf[3];
        int xSource, xDest, ySource, yDest;
        bool validMove = false;
        bool inCheck = false;
        bool isMoveIntoCheck = false;
        while(true){
            // Get user coords and perform basic validation
            getTurnCoords(userBuf, color, &xSource, &ySource, &xDest, &yDest, output);

            // Get the specified piece and see if it puts own player in check
            piece *curPiece = board[xSource][ySource];

            // Need to do some fiddly business for castling because the destination the user puts in to let us know they want to castle is not the actual destination of the king
            if((curPiece->myType == KING) && (board[xDest][yDest] != nullptr) && (board[xDest][yDest]->myType == ROOK) && (curPiece->myColor == board[xDest][yDest]->myColor)){
                int trueYDest;
                if(yDest - ySource > 0) {
                    isMoveIntoCheck = isChecking(xSource, ySource, xDest, 6, (king*)curPiece);
                } else {
                    isMoveIntoCheck = isChecking(xSource, ySource, xDest, 2, (king*)curPiece);
                }
            } else {
                isMoveIntoCheck = isChecking(xSource, ySource, xDest, yDest, myKing);
            }

            // curPiece->printSelf();

            if(isMoveIntoCheck && (curPiece->myColor == color)){
                cout << "Can't move yourself into check!" << endl;
                cout << "Can't perform that move!" << endl;
            }

            // If user has requested to preview their moves
            else if(preview){
                validMove = previewMove(xSource, ySource, xDest, yDest, output, myKing);
            }

            // If not, try to perform the move
            else {
                validMove = movePiece(xSource, ySource, xDest, yDest, output, false, board);
            }

            // Check to see if we're putting the opposing king in check
            // Technically this isn't needed
            inCheck = isChecking(xSource, ySource, xDest, yDest, oppKing);
            if(validMove && inCheck){
                oppKing->isCheck = true;
            } else if(validMove && !inCheck){
                oppKing->isCheck = false;
            }
            
            printMyBoard(color);
            isMoveIntoCheck = false;
            validMove = false;
            inCheck = false;
        }
        return;
    }

}