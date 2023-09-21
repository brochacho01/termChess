#include <iostream>

#include <termios.h>

#include "networkUtilities.h"
#include "board.h"
#include "gameHelpers.h"
#include "pieces/piece.h"

using namespace std;

inline int ctoi(char c) { return c - '0'; }

void setup(char *connectType, char *color, bool *output, bool *preview, int *fd, king *&myKing, king *&oppKing){
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

    char outputDecision;
    cout << "Would you like detailed output of move validation? (y/n) ";
    cin >> outputDecision;
    // Erring on the side of caution to make sure users really don't want extra output
    if(outputDecision == 'n'){
        *output = false;
    } else {
        *output = true;
    }

    char previewDecision;
    cout << "Would you like to preview each of your moves and then confirm them? (y/n) ";
    cin >> previewDecision;
    // Because this is a lot of extra output going to err towards no
    if(previewDecision == 'y'){
        *preview = true;
    } else {
        *preview = false;
    }

    // Clear stdin for game loop
    while(getchar() != '\n');

}

// Copies the board, simulates the move, asks the user for confirmation, if so, performs the move on the real board

// Need to make sure we're not updating statuses when previewing
bool previewMove(int xSource, int ySource, int xDest, int yDest, bool output, king *&myKing){
    piece *boardCopy[8][8];
    char confirmation;
    bool validMove = false;
    copyBoard(boardCopy);

    piece *curPiece = board[xSource][ySource];

    // Simulate the move
    validMove = movePiece(xSource, ySource, xDest, yDest, output, true, boardCopy);

    // If the move they tried wasn't valid simply return
    if(!validMove){
        return false;
    }

    cout << "-----PREVIEW OF MOVE-----" << endl;

    // Display the board to the user
    printTestBoard(boardCopy);

    cout << "Would you like to perform this move? (y/n) ";
    cin >> confirmation;

    // Err on the side of non-confirmation of move
    // If they say yes then perform the move on the actual board, otherwise prompt user again for a move
    if(confirmation == 'y'){
        validMove = movePiece(xSource, ySource, xDest, yDest, false, false, board);

        return true;
    }

    return false;

}

void getTurnCoords(char *userBuf, char color, int *xSource, int *ySource, int *xDest, int *yDest, bool output){
    bool validCoords = false;
    while(!validCoords){
        userBuf[0] = 'h';
        getCoordInput(userBuf, 1, color);
        *xSource = ctoi(userBuf[0]);
        *ySource = ctoi(userBuf[1]);
        userBuf[0] = 'h';
        getCoordInput(userBuf, 2, color);
        *xDest = ctoi(userBuf[0]);
        *yDest = ctoi(userBuf[1]);
        validCoords = validateCoords(*xSource, *ySource, *xDest, *yDest, output);
    }
}

// TODO fill in these functions
// TODO maybe try to convert this to using cin because stdin is funky
// TODO add ability to change output/preview settings
// TODO add ability to propose a draw

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

		cin >> userBuf[0];
        cin >> userBuf[1];

        // Check to see if a user tries to enter a Ctrl + D and recover properly
        if(cin.eof()){
            cin.clear();
            clearerr(stdin);
            cout << endl <<  "Press ENTER to continue" << endl;
        }
		// cout << endl;
		if(tolower(userBuf[0]) == 'h'){
			// help();
		} else if(tolower(userBuf[0]) == 'p'){
			printMyBoard(color);
			printf("Press ENTER to continue\n");
		} else if(tolower(userBuf[0]) == 'c'){
			// if(concede()){
				// return 0;
			// }
		} else if(tolower(userBuf[0]) == 's'){
            // 
        } else if(tolower(userBuf[0]) == 'd'){
            // 
        }
		while(getchar() != '\n');
	}
	return 1;
}

bool validateCoords(int xSource, int ySource, int xDest, int yDest, bool output){
    if((xSource < 0) || (ySource < 0) || (xDest < 0) || yDest < 0){
        if(output){
            cout << "Can't use negative coordinates!" << endl;
        }
        return false;
    }
    else if((xSource > 7) || (ySource > 7) || (xDest > 7) || (yDest > 7)){
        if(output){
            cout << "Can't use coordinates greater than 7!" << endl;
        }
        return false;
    }
    else if(board[xSource][ySource] == nullptr){
        if(output){
            cout << "Can't move a non-existent piece!" << endl;
        }
        return false;
    }
    return true;
}

bool movePiece(int xSource, int ySource, int xDest, int yDest, bool output, bool simulation, piece* (&board)[8][8]){
    bool validMove = false;
    piece *curPiece = board[xSource][ySource];

    switch(curPiece->myType){
        case PAWN:
            validMove = ((pawn*)curPiece)->move(xSource, ySource, xDest, yDest, output, simulation, board);
            break;
        case ROOK:
            validMove = ((rook*)curPiece)->move(xSource, ySource, xDest, yDest, output, simulation, board);
            break;
        case KNIGHT:
            validMove = ((knight*)curPiece)->move(xSource, ySource, xDest, yDest, output, board);
            break;
        case BISHOP:
            validMove = ((bishop*)curPiece)->move(xSource, ySource, xDest, yDest, output, board);
            break;
        case QUEEN:
            validMove = ((queen*)curPiece)->move(xSource, ySource, xDest, yDest, output, board);
            break;
        case KING:
            validMove = ((king*)curPiece)->move(xSource, ySource, xDest, yDest, output, simulation, board);
            break;
        default:
            break;
    }

    return validMove;
}