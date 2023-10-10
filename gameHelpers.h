#include <vector>

#include "pieces/piece.h"

using namespace std;


#ifndef HELPERS
#define HELPERS

void setup(char *connectType, char *color, bool *output, bool *preview, int *fd, king *&myKing, king *&oppKing);
bool previewMove(char myColor, int xSource, int ySource, int xDest, int yDest, bool output, king *&myKing);
void getTurnCoords(int fd, char *userBuf, char color, int *xSource, int *ySource, int *xDest, int *yDest, bool *output, bool *preview, vector<int> &myCaptured, vector<int> &oppCaptured);
int getCoordInput(int fd, char *userBuf, int type, char color, bool *output, bool *preview, vector<int> &myCaptured, vector<int> &oppCaptured);
bool validateCoords(int xSource, int ySource, int xDest, int yDest, bool output, char color);
bool movePiece(int xSource, int ySource, int xDest, int yDest, bool output, bool simulation, piece* (&board)[BOARDSIZE][BOARDSIZE]);
void settings(bool *output, bool *preview);
void help(char color);
void draw(int fd, char myColor, bool offering);
void concede(int fd, char myColor, bool offering);
void addToCaptured(vector<int> &capturedList, piece* destPiece, int *capturedVal);

#endif