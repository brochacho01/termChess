#ifndef UTILITIES_H
#define UTILITIES_H

#include <arpa/inet.h>

#include "pieces/piece.h"


int sendBoard(int fd);
int receiveBoard(int fd, char myColor, king *&myKing, king *&oppKing);

void sendCaptured(int fd, int capturedVal);
void receiveCaptured(int fd, std::vector<int> &capturedList);

int sendAck(int fd);
int receiveAck(int fd);

int sendColor(int fd, char color);
int receiveColor(int fd, char *color);

void sendAction(int fd, char *action);
void receiveAction(int fd, char *action);

void sendInt(int fd, int value);
void receiveInt(int fd, int *value);

int createServerAndWait(void);
int connectToServer(void);
#endif