#include <iostream>

#include <cstdio>

#include "board.h"

using namespace std;

void printBoardForWhite(piece* (&boardToPrint)[8][8]);
void printBoardForRed(piece* (&boardToPrint)[8][8]);
void printCell(piece* curPiece);
void printBound(void);
void printRowBound(void);
void colorRed(void);
void colorWhite(void);

piece* board[8][8];

void printMyBoard(char myColor, piece* (&boardToPrint)[8][8]){
    cout << endl;
    if(myColor == 'r'){
        printBoardForRed(boardToPrint);
    } else {
        printBoardForWhite(boardToPrint);
    }
    cout << endl;
}

void printBoardForRed(piece* (&boardToPrint)[8][8]){
    for(int i = 0; i < 8; i++){
        printBound();
        printRowBound();
        for(int j = 0; j < 8; j++){
            printCell(boardToPrint[i][j]);
        }
        printf(" %d\n", i);
    }
    printBound();
    cout << "  0   1   2   3   4   5   6   7" << endl;
}

void printBoardForWhite(piece* (&boardToPrint)[8][8]){
    for(int i = 7; i > -1; i--){
        printBound();
        printRowBound();
        for(int j = 7; j > -1; j--){
            printCell(boardToPrint[i][j]);
        }
        printf(" %d\n", i);
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
        printf("%c", curPiece->myType);
        colorWhite();
    } else {
        printf("%c", curPiece->myType);
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
    printf("\033[0;31m");
}

void colorWhite(void){
    printf("\033[0;37m");
}

void createPieces(king *&playerKing, char color){
    int pawnRow = 1;
    int otherRow = 0;
    if(color == 'r'){
        pawnRow = 6;
        otherRow = 7;
    }
    for(int i = 0; i < 8; i++){
        pawn* newPawn = new pawn(color);
        board[pawnRow][i] = newPawn;
    }
    for(int i = 0; i < 8; i += 7){
        rook *newRook = new rook(color);
        board[otherRow][i] = newRook;
    }
    for(int i = 1; i < 8; i += 5){
        knight *newKnight = new knight(color);
        board[otherRow][i] = newKnight;
    }
    for(int i = 2; i < 8; i += 3){
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
        for(int j = 0; j < 8; j++){
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

void copyBoard(piece* (&boardCopy)[8][8]){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
           boardCopy[i][j] = board[i][j]; 
        }
    }
}

