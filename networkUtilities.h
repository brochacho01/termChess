#ifndef UTILITIES_H
#define UTILITIES_H

#include <arpa/inet.h>

#include "pieces/piece.h"


int sendBoard(int fd);
int receiveBoard(int fd, char myColor, king *&myKing, king *&oppKing);
int sendAck(int fd);
int receiveAck(int fd);

int sendColor(int fd, char color);
int receiveColor(int fd, char *color);

void sendAction(int fd, char *action);
void receiveAction(int fd, char *action);

void waitForTurn(int fd, char myColor, king *&myKing, king *&oppKing);

int createServerAndWait(void);
int connectToServer(void);
#endif