#include <iostream>
#include <csignal>

#include <termios.h>
#include <stdio.h>
#include <sys/poll.h>

#include "networkUtilities.h"
#include "pieces/piece.h"
#include "board.h"
#include "gameHelpers.h"

using namespace std;

void gameLoop(int fd, char myColor, char connectType, bool *output, bool *preview, king *&myKing, king *&oppKing);
void doTurn(int fd, char myColor, bool output, bool *preview, king *&myKing, king *&oppKing, int (&passantCoords)[2]);
void waitForTurn(int fd, char myColor, king *&myKing, king *&oppKing);
void welcome(void);
void sigHandler(int sigNum);


int main() {
    char connectType;
    char myColor;
    bool output;
    bool preview;
    int fd;
    king *myKing;
    king *oppKing;

    // Create an action handler as users tend to panic Ctrl + C upon poor input, want to make sure to recover
	// Properly if it happens during input
    signal(SIGINT, sigHandler);

    welcome();
    setup(&connectType, &myColor, &output, &preview, &fd, myKing, oppKing);
    printMyBoard(myColor, board);
    gameLoop(fd, myColor, connectType, &output, &preview, myKing, oppKing);
}

void gameLoop(int fd, char myColor, char connectType, bool *output, bool *preview, king *&myKing, king *&oppKing){
    // White goes first
    char curTurnColor = 'w';
    int passantCoords[2] = { -1, -1 };
    while(true){
        if(myColor == curTurnColor){
            doTurn(fd, myColor, output, preview, myKing, oppKing, passantCoords);
        } else {
            cout << "Not my turn!" << endl;
            waitForTurn(fd, myColor, myKing, oppKing);
            printMyBoard(myColor, board);
        }
        if(curTurnColor == 'w'){
            curTurnColor = 'r';
        } else {
            curTurnColor = 'w';
        }
    }
}

void doTurn(int fd, char myColor, bool output, bool *preview, king *&myKing, king *&oppKing, int (&passantCoords)[2]){
    char userBuf[3];
    int xSource, xDest, ySource, yDest;
    bool validMove = false;
    bool inCheck = false;
    bool isMoveIntoCheck = false;

    // At the beginning of a turn see if our passant coords exist, if they do update the piece and unset the coords
    if((passantCoords[0]) != -1 && (board[passantCoords[0]][passantCoords[1]] != nullptr)){
        pawn *thePawn = (pawn*)board[passantCoords[0]][passantCoords[1]];
        thePawn->lastMoveTwo = false;
        passantCoords[0] = -1;
        passantCoords[1] = -1;
    }

    while(!validMove){
        // Get user coords and perform basic validation
        getTurnCoords(fd, userBuf, myColor, &xSource, &ySource, &xDest, &yDest, &output, preview);

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

        if(isMoveIntoCheck && (curPiece->myColor == myColor)){
            cout << "Can't move yourself into check!" << endl;
            cout << "Can't perform that move!" << endl;
        }

        // If user has requested to preview their moves
        else if(*preview){
            validMove = previewMove(myColor, xSource, ySource, xDest, yDest, output, myKing);
        }

        // If not, try to perform the move
        else {
            validMove = movePiece(xSource, ySource, xDest, yDest, output, false, board);
        }

        // Check to see if we're putting the opposing king in check
        // This ends up being used in castling logic
        inCheck = isChecking(xSource, ySource, xDest, yDest, oppKing);
        if(validMove && inCheck){
            oppKing->isCheck = true;
        } else if(validMove && !inCheck){
            oppKing->isCheck = false;
        }

        // Check to see if we moved a pawn two spaces thus making it possible to en passant
        if((curPiece->myType == PAWN) && (validMove) && (((pawn*)curPiece)->lastMoveTwo == true)){
            passantCoords[0] = xDest;
            passantCoords[1] = yDest;
        }
        
        printMyBoard(myColor, board);
    }

    char move = 'm';
    sendAction(fd, &move);
    sendBoard(fd);
    return;
}

void waitForTurn(int fd, char myColor, king *&myKing, king *&oppKing){
  char action;
  bool receivedMove = false;
  while(!receivedMove){
    receiveAction(fd, &action);

    switch(action){
      case 'm':
        receiveBoard(fd, myColor, myKing, oppKing);
        receivedMove = true;
        break;
      case 'd':
        draw(fd, myColor, false);
        break;
      case 'c':
        concede(fd, myColor, false);
      default:
        cout << "Unknown turn action" << endl;
        break;
    }
  }
}

void welcome(void){


cout <<" _                      _____  _   _" << endl;               
cout << "| |                    /  __ \\| | | |" << endl;              
cout << "| |_ ___ _ __ _ __ ___ | /  \\/| |_| | ___  ___ ___" << endl; 
cout << "| __/ _ \\ '__| '_ ` _ \\| |    |  _  |/ _ \\/ __/ __|" << endl;
cout << "| ||  __/ |  | | | | | | \\__/\\| | | |  __/\\__ \\__ \\" << endl;
cout << " \\__\\___|_|  |_| |_| |_|\\____/\\_| |_/\\___||___/___/" << endl;

cout << endl << endl;
}

// Make sure that users don't panic Ctrl + C.
void sigHandler(int sigNum){
	if(sigNum == SIGQUIT){
		cleanBoard(board);
        exit(0);
	} else {
        return;
    }
}