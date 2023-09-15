#include <iostream>

#include "networkUtilities.h"
#include "pieces/piece.h"
#include "board.h"

using namespace std;

void setup(char *connectType, char *color, int *fd, king *myKing, king *oppKing);


int main() {
    char connectType;
    char color;
    int fd;
    king *myKing;
    king *oppKing;
    setup(&connectType, &color, &fd, myKing, oppKing);
    printMyBoard(color);
}

void setup(char *connectType, char *color, int *fd, king *myKing, king *oppKing){
    cout << "Would you like to host or connect? (h/c) ";
    cin >> *connectType;
    tolower(*connectType);

    if(*connectType == 'h'){
        *fd = createServerAndWait();
    } else {
        *fd = connectToServer();
    }
    cout << "Connection established!" << endl;
    
    if(*connectType != 'h'){
        cout << "Woud you like to be red or white? (r/w) ";
        cin >> *color;
        tolower(*color);
        if(*color == 'r'){
            sendColor(*fd, 'w');
        } else {
            sendColor(*fd, 'r');
        }
    } else {
        receiveColor(*fd, color);
    }


    if(*connectType == 'h'){
        createBoard(color, myKing, oppKing);
        sendBoard(*fd);
    } else {
        receiveBoard(*fd, *color, myKing, oppKing);
    }
}

void gameLoop(int fd, char color, char connectType){

}