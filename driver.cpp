#include <iostream>
#include <termios.h>

#include <stdio.h>

#include "networkUtilities.h"
#include "pieces/piece.h"
#include "board.h"

using namespace std;

void setup(char *connectType, char *color, int *fd, king *&myKing, king *&oppKing);
void gameLoop(int fd, char color, char connectType, king *&myKing, king *&oppKing);
int getCoordInput(char *userBuf, int type, char color);
bool validateCoords(int xSource, int ySource, int xDest, int yDest);

inline int ctoi(char c) { return c - '0'; }

// TODO
// Add way to preview players moves, i.e. they enter something, they get a preview of the board, and then get asked to confirm the move, if they do, then that move goes through, otherwise it doesn't and they enter a different move. Also make this something they can set up at the beginning of the game

// Add way for players to say if they want detailed output on move validation or not at the beginning of the game

int main() {
    char connectType;
    char color;
    int fd;
    king *myKing;
    king *oppKing;
    setup(&connectType, &color, &fd, myKing, oppKing);
    printMyBoard(color);
    gameLoop(fd, color, connectType, myKing, oppKing);
}

void setup(char *connectType, char *color, int *fd, king *&myKing, king *&oppKing){
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
        cout << "Sent color!" << endl;
    } else {
        receiveColor(*fd, color);
        cout << "Received color!" << endl;
    }

    if(*connectType == 'h'){
        createBoard(color, myKing, oppKing);
        piece *tmp = board[0][0];
        tmp->printSelf();
        sendBoard(*fd);
    } else {
        receiveBoard(*fd, *color, myKing, oppKing);
    }
    // Clear stdin for game loop
    while(getchar() != '\n');
}


void gameLoop(int fd, char color, char connectType, king *&myKing, king *&oppKing){
    if(color == 'r'){
        char userBuf[3];
        int xSource, xDest, ySource, yDest;
        bool validCoords = false;
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
                validCoords = validateCoords(xSource, ySource, xDest, yDest);
            }

            // Get the specified piece and see if it puts own player in check
            piece *curPiece = board[xSource][ySource];

            // cout << "My king position is: " << myKing->position[0] << " " << myKing->position[1] << endl;

            // Need to do some fiddly business for castling because the destination the user puts in to let us know they want to castle is not the actual destination of the king
            int check = 0;
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

            // If not, try to perform the move
            else {
                switch(curPiece->myType){
                    case PAWN:
                        ((pawn*)curPiece)->move(xSource, ySource, xDest, yDest, board);
                        break;
                    case ROOK:
                        ((rook*)curPiece)->move(xSource, ySource, xDest, yDest, board);
                        break;
                    case KNIGHT:
                        ((knight*)curPiece)->move(xSource, ySource, xDest, yDest, board);
                        break;
                    case BISHOP:
                        ((bishop*)curPiece)->move(xSource, ySource, xDest, yDest, board);
                        break;
                    case QUEEN:
                        ((queen*)curPiece)->move(xSource, ySource, xDest, yDest, board);
                        break;
                    case KING:
                        ((king*)curPiece)->move(xSource, ySource, xDest, yDest, board);
                        break;
                    default:
                        break;
                }
            }

            
            printMyBoard(color);
            isMoveIntoCheck = false;
            validCoords = false;
        }
        return;
    }

}


// TODO fill in these functions
// TODO maybe try to convert this to using cin because stdin is funky
// Type will either be 1 or 2, 1 represents entering source coord, 2 represents dest
int getCoordInput(char *userBuf, int type, char color){
	while((userBuf[0] == 'h') || (userBuf[0] == 'p')|| (userBuf[0] == 'c')){
		int inFD = fileno(stdin);
		tcflush(inFD, TCIFLUSH);
		if(type == 1){
			cout << "Enter coordinate of piece to move, type h for help, type p to print the board: ";
		} else {
			cout << "Enter the destination coordinate of piece to move, type h for help, type p to print the board: ";
		}
		// Check to see if a user tries to enter a Ctrl + D and recover properly
		char *readStatus = fgets(userBuf, 3, stdin);
		if(!readStatus){
			clearerr(stdin);
		}
		cout << endl;
		if(tolower(userBuf[0]) == 'h'){
			// help();
		} else if(tolower(userBuf[0]) == 'p'){
			printMyBoard(color);
			printf("Press ENTER to continue\n");
		} else if(tolower(userBuf[0]) == 'c'){
			// if(concede()){
				// return 0;
			// }
		}
		while(getchar() != '\n');
	}
	return 1;
}

bool validateCoords(int xSource, int ySource, int xDest, int yDest){
    if((xSource < 0) || (ySource < 0) || (xDest < 0) || yDest < 0){
        cout << "Can't use negative coordinates!" << endl;
        return false;
    }
    else if((xSource > 7) || (ySource > 7) || (xDest > 7) || (yDest > 7)){
        cout << "Can't use coordinates greater than 7!" << endl;
        return false;
    }
    else if(board[xSource][ySource] == nullptr){
        cout << "Can't move a non-existent piece!" << endl;
        return false;
    }
    return true;
}

