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

piece* board[BOARDSIZE][BOARDSIZE];

void printMyBoard(char myColor, piece* (&boardToPrint)[BOARDSIZE][BOARDSIZE]){
    cout << endl;
    if(myColor == 'r'){
        printBoardForRed(boardToPrint);
    } else {
        printBoardForWhite(boardToPrint);
    }
    cout << endl;
}

void printBoardForRed(piece* (&boardToPrint)[BOARDSIZE][BOARDSIZE]){
    for(int i = 0; i < BOARDSIZE; i++){
        printBound();
        printRowBound();
        for(int j = 0; j < BOARDSIZE; j++){
            printCell(boardToPrint[i][j]);
        }
        cout << " " << i << endl;
    }
    printBound();
    cout << "  0   1   2   3   4   5   6   7" << endl;
}

void printBoardForWhite(piece* (&boardToPrint)[BOARDSIZE][BOARDSIZE]){
    for(int i = 7; i > -1; i--){
        printBound();
        printRowBound();
        for(int j = 7; j > -1; j--){
            printCell(boardToPrint[i][j]);
        }
        cout << " " << i << endl;
    }
    printBound();
    cout << "  7   6   5   4   3   2   1   0 " << endl;
}

void printCell(piece* curPiece){
    cout << " ";
    if(curPiece == nullptr){
        cout << " ";
    }
    else if(curPiece->myColor == 'r') {
        colorRed();
        cout << curPiece->myType;
        colorWhite();
    } else {
        cout << curPiece->myType;
    }
    cout << " |";
}

void printBound(void){
    cout << "|---|---|---|---|---|---|---|---|" << endl;
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
    if(color == 'r'){
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
    board[otherRow][3] = newQueen;

    king *newKing = new king(color);
    
    board[otherRow][4] = newKing;
    
    newKing->position[0] = otherRow;
    newKing->position[1] = 4;
    newKing->isCheck = false;
    newKing->isCheckmate = false;

    playerKing = (king*)board[otherRow][4];
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
    if(*myColor == 'r'){
        createPieces(myKing, 'r');
        createPieces(oppKing, 'w');
    } else {
        createPieces(myKing, 'w');
        createPieces(oppKing, 'r');
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
                            boardCopy[i][j] = newPawn;
                            break;
                        }
                    case ROOK:
                        {
                            rook *newRook = new rook(board[i][j]->myColor);
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
                            boardCopy[i][j] = newKing;
                            break;
                        }
                    default:
                        {
                            break;
                        }
                }
        //    boardCopy[i][j] = board[i][j]; 
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