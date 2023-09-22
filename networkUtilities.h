#ifndef UTILITIES_H
#define UTILITIES_H

#include <arpa/inet.h>
#include "pieces/piece.h"

#define RECEIVE_BUFFER_SIZE 200
#define MESSAGE_BUFFER_SIZE 200
#define ACKNOWLEDGE_MESSAGE "ACKNOWLEDGE_MESSAGE"
#define SEND_MESSAGE_AFFIRMATION  "y"
#define STATUS_MESSAGE_SUCCESS "STATUS_MESSAGE_SUCCESS"
#define STATUS_MESSAGE_FAIL "STATUS_MESSAGE_FAIL"
#define SO_REUSEPORT_OPTION_VALUE 1
#define HOST_IP_ADDRESS "0.0.0.0"
#define CONNECT_IP_ADDRESS "127.0.0.1"
#define SERVER_PORT 1234


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