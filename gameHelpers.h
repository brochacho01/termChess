#ifndef HELPERS
#define HELPERS

void setup(char *connectType, char *color, bool *output, bool *preview, int *fd, king *&myKing, king *&oppKing);
bool previewMove(int xSource, int ySource, int xDest, int yDest, bool output, king *&myKing);
void getTurnCoords(char *userBuf, char color, int *xSource, int *ySource, int *xDest, int *yDest, bool *output, bool *preview);
int getCoordInput(char *userBuf, int type, char color, bool *output, bool *preview);
bool validateCoords(int xSource, int ySource, int xDest, int yDest, bool output);
bool movePiece(int xSource, int ySource, int xDest, int yDest, bool output, bool simulation, piece* (&board)[8][8]);
void settings(bool *output, bool *preview);
void help(char color);

#endif