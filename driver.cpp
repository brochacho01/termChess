#include <iostream>
#include <csignal>
#include <vector>

#include <termios.h>
#include <stdio.h>
#include <sys/poll.h>

#include "networkUtilities.h"
#include "pieces/piece.h"
#include "board.h"
#include "gameHelpers.h"

using namespace std;

void gameLoop(int fd, char myColor, char connectType, bool *output, bool *preview, king *&myKing, king *&oppKing, vector<int> &myCaptured, vector<int> &oppCaptured);
void doTurn(int fd, char myColor, bool *output, bool *preview, king *&myKing, king *&oppKing, int (&passantCoords)[2], vector<int> &myCaptured, vector<int> &oppCaptured);
void waitForTurn(int fd, char myColor, king *&myKing, king *&oppKing, vector<int> &myCaptured, vector<int> &oppCaptured);
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
    vector<int> myCaptured;
    vector<int> oppCaptured;


    // Create an action handler as users tend to panic Ctrl + C upon poor input, want to make sure to recover
	// Properly if it happens during input
    signal(SIGINT, sigHandler);

    welcome();
    setup(&connectType, &myColor, &output, &preview, &fd, myKing, oppKing);
    printMyBoardWithCaptures(myColor, board, myCaptured, oppCaptured);
    gameLoop(fd, myColor, connectType, &output, &preview, myKing, oppKing, myCaptured, oppCaptured);
}

void gameLoop(int fd, char myColor, char connectType, bool *output, bool *preview, king *&myKing, king *&oppKing, vector<int> &myCaptured, vector<int> &oppCaptured){
    // White goes first
    char curTurnColor = WHITE;
    int passantCoords[2] = { -1, -1 };
    while(true){
        if(myColor == curTurnColor){
            doTurn(fd, myColor, output, preview, myKing, oppKing, passantCoords, myCaptured, oppCaptured);
        } else {
            cout << "Not my turn!" << endl;
            waitForTurn(fd, myColor, myKing, oppKing, myCaptured, oppCaptured);
            printMyBoardWithCaptures(myColor, board, myCaptured, oppCaptured);
        }
        if(curTurnColor == WHITE){
            curTurnColor = RED;
        } else {
            curTurnColor = WHITE;
        }
    }
}

void doTurn(int fd, char myColor, bool *output, bool *preview, king *&myKing, king *&oppKing, int (&passantCoords)[2], vector<int> &myCaptured, vector<int> &oppCaptured){
    char userBuf[3];
    int xSource, xDest, ySource, yDest;
    bool validMove = false;
    bool inCheck = false;
    bool isMoveIntoCheck = false;
    bool isCastle = false;
    piece *destPiece = nullptr;
    int capturedVal = 0;

    // At the beginning of a turn see if our passant coords exist, if they do update the piece and unset the coords
    if((passantCoords[0]) != -1 && (board[passantCoords[0]][passantCoords[1]] != nullptr)){
        pawn *thePawn = (pawn*)board[passantCoords[0]][passantCoords[1]];
        thePawn->lastMoveTwo = false;
        passantCoords[0] = -1;
        passantCoords[1] = -1;
    }

    // Check to see if we're in check
    bool prevTurnCheck = isCheck(myKing, board);
    if(prevTurnCheck){
        cout << endl;
    }

    while(!validMove){
        // Get user coords and perform basic validation
        getTurnCoords(fd, userBuf, myColor, &xSource, &ySource, &xDest, &yDest, output, preview, myCaptured, oppCaptured);

        // Get the specified piece and see if it puts own player in check
        piece *curPiece = board[xSource][ySource];

        // Need to do some fiddly business for castling because the destination the user puts in to let us know they want to castle is not the actual destination of the king
        if((curPiece->myType == KING) && (board[xDest][yDest] != nullptr) && (board[xDest][yDest]->myType == ROOK) && (curPiece->myColor == board[xDest][yDest]->myColor)){
            if(yDest - ySource > 0) {
                isMoveIntoCheck = isMoveChecking(xSource, ySource, xDest, LONGCASTLEDEST, (king*)curPiece);
            } else {
                isMoveIntoCheck = isMoveChecking(xSource, ySource, xDest, SHORTCASTLEDEST, (king*)curPiece);
            }

            // Need to know if we're castling
            if(!isMoveIntoCheck){
                isCastle = true;
            }

        } else {
            isMoveIntoCheck = isMoveChecking(xSource, ySource, xDest, yDest, myKing);
        }

        if(isMoveIntoCheck && (curPiece->myColor == myColor)){
            cout << "Can't move yourself into check!" << endl;
            cout << "Can't perform that move!" << endl;
        }

        // If user has requested to preview their moves
        else if(*preview){
            // Tracking if a piece gets captured
            if(board[xDest][yDest] != nullptr){
                destPiece = new piece(board[xDest][yDest]->myColor, board[xDest][yDest]->myType);
            }
            validMove = previewMove(myColor, xSource, ySource, xDest, yDest, output, myKing);
        }

        // If not, try to perform the move
        else {
            // Tracking if a piece gets captured
            if(board[xDest][yDest] != nullptr){
                destPiece = new piece(board[xDest][yDest]->myColor, board[xDest][yDest]->myType);
            } else {
                cout << "Null destPiece" << endl;
            }
            validMove = movePiece(xSource, ySource, xDest, yDest, output, false, board);
        }

        // Check to see if we're putting the opposing king in check
        // This ends up being used in castling logic
        if(validMove){
            if(isCastle){
                inCheck = isMoveChecking(myKing->position[0], myKing->position[1], myKing->position[0], myKing->position[1], oppKing);            
            } else {
                inCheck = isMoveChecking(xDest, yDest, xDest, yDest, oppKing);
            }
        }

        if(validMove && inCheck){
            oppKing->isCheck = true;
        } else if(validMove && !inCheck){
            oppKing->isCheck = false;
        }

        // Update pieces captured
        if(validMove && (destPiece != nullptr) && (destPiece->myColor != myColor)){
            addToCaptured(myCaptured, destPiece, &capturedVal);
        }

        // Check to see if we moved a pawn two spaces thus making it possible to en passant
        if((curPiece->myType == PAWN) && (validMove) && (((pawn*)curPiece)->lastMoveTwo == true)){
            passantCoords[0] = xDest;
            passantCoords[1] = yDest;
        }
        
        printMyBoardWithCaptures(myColor, board, myCaptured, oppCaptured);
        isCastle = false;
        if(destPiece != nullptr){
            delete destPiece;
            destPiece = nullptr;
        }
    }

    char move = 'm';
    sendAction(fd, &move);
    sendBoard(fd); 
    sendCaptured(fd, capturedVal);
    capturedVal = 0;
    return;
}

void waitForTurn(int fd, char myColor, king *&myKing, king *&oppKing, vector<int> &myCaptured, vector<int> &oppCaptured){
  char action;
  bool receivedMove = false;
  while(!receivedMove){
    receiveAction(fd, &action);

    switch(action){
      case 'm':
        receiveBoard(fd, myColor, myKing, oppKing);
        receiveCaptured(fd, oppCaptured);
        receivedMove = true;
        break;
      case 'd':
        draw(fd, myColor, false);
        break;
      case 'c':
        concede(fd, myColor, false);
      default:
        cout << "Unknown turn action" << endl;
        cout << action << endl;
        exit(0);
        break;
    }
  }
}
                                                  
void welcome(void){

cout << " _                      _____ _" << endl;                   
cout << "| |                    /  __ \\ |" << endl;                  
cout << "| |_ ___ _ __ _ __ ___ | /  \\/ |__   ___  ___ ___" << endl; 
cout << "| __/ _ \\ '__| '_ ` _ \\| |   | '_ \\ / _ \\/ __/ __|" << endl;
cout << "| ||  __/ |  | | | | | | \\__/\\ | | |  __/\\__ \\__ \\" << endl;
cout << " \\__\\___|_|  |_| |_| |_|\\____/_| |_|\\___||___/___/" << endl;

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