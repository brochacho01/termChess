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

inline int ctoi(char c) { return c - '0'; }

// TODO Refactor the switch case of moving a piece into a helper method

// TODO
// Add way to preview players moves, i.e. they enter something, they get a preview of the board, and then get asked to confirm ttruehe move, if they do, then that move goes through, otherwise it doesn't and they enter a different move. Also make this something they can set up at the beginning of the game

// Add way for players to say if they want detailed output on move validation or not at the beginning of the game

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
        bool validCoords = false;
        bool validMove = false;
        bool inCheck = false;
        bool isMoveIntoCheck = false;
        while(true){
            // Get user coords and perform basic validation
            while(!validCoords){
                userBuf[0] = 'h';
                getCoordInput(userBuf, 1, color);
                xSource = ctoi(userBuf[0]);
                ySource = ctoi(userBuf[1]);
                userBuf[0] = 'h';
                getCoordInput(userBuf, 2, color);
                xDest = ctoi(userBuf[0]);
                yDest = ctoi(userBuf[1]);
                validCoords = validateCoords(xSource, ySource, xDest, yDest, output);
            }

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
            validCoords = false;
        }
        return;
    }

}