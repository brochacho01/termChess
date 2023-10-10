#include <iostream>

#include <cstdio>

#include "board.h"

using namespace std;

void printBoardForWhite(piece* (&boardToPrint)[BOARDSIZE][BOARDSIZE]);
void printBoardForRed(piece* (&boardToPrint)[BOARDSIZE][BOARDSIZE]);
void printCell(piece* curPiece);
void printBound(void);
void printRowBound(void);
void colorRed(void);
void colorWhite(void);
void printList(char color, vector<int> theList, bool amILeading, int pointDiff);
bool calcLead(vector<int> myCaptured, vector<int> oppCaptured, int &myPoints, int &oppPoints);

piece* board[BOARDSIZE][BOARDSIZE];

void printMyBoard(char myColor, piece* (&boardToPrint)[BOARDSIZE][BOARDSIZE]){
    cout << endl;
    if(myColor == RED){
        printBoardForRed(boardToPrint);
    } else {
        printBoardForWhite(boardToPrint);
    }
    cout << endl;
}

void printMyBoardWithCaptures(char myColor, piece*(&boardToPrint)[BOARDSIZE][BOARDSIZE], vector<int> myCaptured, vector<int> oppCaptured){
    char oppColor;
    int myPoints = 0;
    int oppPoints = 0;
    bool amILeading = calcLead(myCaptured, oppCaptured, myPoints, oppPoints);
    int pointDiff = abs(myPoints - oppPoints);
    if(myColor == RED){
        oppColor = WHITE;
    } else {
        oppColor = RED;
    }
    cout << endl;

    if(oppCaptured.size() > 0){
        cout << "Pieces opponent has captured: ";
        printList(oppColor, oppCaptured, !amILeading, pointDiff);
        cout << endl << endl;
    }

    if(myColor == RED){
        printBoardForRed(boardToPrint);
    } else {
        printBoardForWhite(boardToPrint);
    }

    if(myCaptured.size() > 0){
        cout << endl << "Pieces I have captured: ";
        printList(myColor, myCaptured, amILeading, pointDiff);
        cout << endl;
    }
    cout << endl;
}

void printBoardForRed(piece* (&boardToPrint)[BOARDSIZE][BOARDSIZE]){
    cout << "     H   G   F   E   D   C   B   A" << endl;
    for(int i = 0; i < BOARDSIZE; i++){
        printBound();
        cout << " " << i +1  << " ";
        printRowBound();
        for(int j = 0; j < BOARDSIZE; j++){
            printCell(boardToPrint[i][j]);
        }
        cout << " " << i+1 << endl;
    }
    printBound();
    cout << "     H   G   F   E   D   C   B   A" << endl;
}

void printBoardForWhite(piece* (&boardToPrint)[BOARDSIZE][BOARDSIZE]){
    cout << "     A   B   C   D   E   F   G   H " << endl;
    for(int i = 7; i > -1; i--){
        printBound();
        cout << " " << i + 1 << " ";
        printRowBound();
        for(int j = 7; j > -1; j--){
            printCell(boardToPrint[i][j]);
        }
        cout << " " << i+1 << endl;
    }
    printBound();
    cout << "     A   B   C   D   E   F   G   H " << endl;
}

void printCell(piece* curPiece){
    cout << " ";
    if(curPiece == nullptr){
        cout << " ";
    }
    else if(curPiece->myColor == RED) {
        colorRed();
        cout << curPiece->myType;
        colorWhite();
    } else {
        cout << curPiece->myType;
    }
    cout << " |";
}

void printBound(void){
    cout << "   |---|---|---|---|---|---|---|---|" << endl;
}

void printRowBound(void){
    cout << "|";
}

void colorRed(void){
    cout << "\033[0;31m";
}

void colorWhite(void){
    cout << "\033[0;37m";
}

void createPieces(king *&playerKing, char color){
    int pawnRow = 1;
    int otherRow = 0;
    if(color == RED){
        pawnRow = 6;
        otherRow = 7;
    }
    for(int i = 0; i < BOARDSIZE; i++){
        pawn* newPawn = new pawn(color);
        board[pawnRow][i] = newPawn;
    }
    for(int i = 0; i < BOARDSIZE; i += 7){
        rook *newRook = new rook(color);
        board[otherRow][i] = newRook;
    }
    for(int i = 1; i < BOARDSIZE; i += 5){
        knight *newKnight = new knight(color);
        board[otherRow][i] = newKnight;
    }
    for(int i = 2; i < BOARDSIZE; i += 3){
        bishop *newBishop = new bishop(color);
        board[otherRow][i] = newBishop;
    }
    
    queen *newQueen = new queen(color);
    board[otherRow][4] = newQueen;

    king *newKing = new king(color);
    
    board[otherRow][3] = newKing;
    
    newKing->position[0] = otherRow;
    newKing->position[1] = 3;
    newKing->isCheck = false;
    newKing->isCheckmate = false;

    playerKing = (king*)board[otherRow][3];
}

void createSpaces(void){
    for(int i = 2; i < 6; i++){
        for(int j = 0; j < BOARDSIZE; j++){
            board[i][j] = nullptr;
        }
    }
}

void createBoard(char *myColor, king *&myKing, king *&oppKing){
    // Initialize our board
    if(*myColor == RED){
        createPieces(myKing, RED);
        createPieces(oppKing, WHITE);
    } else {
        createPieces(myKing, WHITE);
        createPieces(oppKing, RED);
    }
    createSpaces();
}

void copyBoard(piece* (&boardCopy)[BOARDSIZE][BOARDSIZE]){
    for(int i = 0; i < BOARDSIZE; i++){
        for(int j = 0; j < BOARDSIZE; j++){
            if(board[i][j] != nullptr){
                switch(board[i][j]->myType){
                    case PAWN:
                        {
                            pawn *newPawn = new pawn(board[i][j]->myColor);
                            newPawn->hasMoved = ((pawn*)board[i][j])->hasMoved;
                            newPawn->lastMoveTwo = ((pawn*)board[i][j])->lastMoveTwo;
                            boardCopy[i][j] = newPawn;
                            break;
                        }
                    case ROOK:
                        {
                            rook *newRook = new rook(board[i][j]->myColor);
                            newRook->hasMoved = ((rook*)board[i][j])->hasMoved;
                            boardCopy[i][j] = newRook;
                            break;
                        }
                    case KNIGHT:
                        {
                            knight *newKnight = new knight(board[i][j]->myColor);
                            boardCopy[i][j] = newKnight;
                            break;
                        }
                    case BISHOP:
                        {
                            bishop *newBishop = new bishop(board[i][j]->myColor);
                            boardCopy[i][j] = newBishop;
                            break;
                        }
                    case QUEEN:
                        {
                            queen *newQueen = new queen(board[i][j]->myColor);
                            boardCopy[i][j] = newQueen;
                            break;
                        }
                    case KING:
                        {
                            king *newKing = new king(board[i][j]->myColor);
                            newKing->position[0] = ((king*)board[i][j])->position[0];
                            newKing->position[1] = ((king*)board[i][j])->position[1];
                            newKing->hasMoved = ((king*)board[i][j])->hasMoved;
                            newKing->isCheck = ((king*)board[i][j])->isCheck;
                            newKing->isCheckmate = ((king*)board[i][j])->isCheckmate;

                            boardCopy[i][j] = newKing;
                            break;
                        }
                    default:
                        {
                            break;
                        }
                }
            } else {
                boardCopy[i][j] = nullptr;
            }
        }
    }
}

void cleanBoard(piece* (&myBoard)[BOARDSIZE][BOARDSIZE]){
    for(int i = 0; i < BOARDSIZE; i++){
        for(int j = 0; j < BOARDSIZE; j++){
            delete myBoard[i][j];
        }
    }
}

void printList(char color, vector<int> theList, bool amILeading, int pointDiff){
    if(color == WHITE){
        colorRed();
    }
    
    for(int curPiece : theList){
        switch(curPiece){
            case PAWNVALUE:
                cout << "P ";
                break;
            case KNIGHTVALUE:
                cout << "N ";
                break;
            case BISHOPVALUE:
                cout << "B ";
                break;
            case ROOKVALUE:
                cout << "R ";
                break;
            case QUEENVALUE:
                cout << "Q ";
                break;
            default:
                cout << "Unknown piece in captured list" << endl;
                break;
        }
    }

    if(color == WHITE){
        colorWhite();
    }

    if(amILeading && (pointDiff != 0)){
        cout << "+" << pointDiff << endl;
    }
}

// Returns true if I have more pieces captured in points than opponent
bool calcLead(vector<int> myCaptured, vector<int> oppCaptured, int &myPoints, int &oppPoints){
    for(int curPiece : myCaptured){
        if(curPiece == 4){
            curPiece = 3;
        }
        myPoints += curPiece;
    }

    for(int curPiece : oppCaptured){
        if(curPiece == 4){
            curPiece = 3;
        }
        oppPoints += curPiece;
    }

    return myPoints > oppPoints;
}