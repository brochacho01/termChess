#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <bits/stdc++.h>

#include "networkUtilities.h"
#include "pieces/piece.h"
#include "board.h"

int sendPawn(int fd, pawn* curPiece);
int sendRook(int fd, rook* curPiece);
int sendKnight(int fd, knight* curPiece);
int sendBishop(int fd, bishop* curPiece);
int sendQueen(int fd, queen* curPiece);
int sendKing(int fd, king* curPiece);
int receivePawn(int fd, pawn *newPawn);
int receiveRook(int fd, rook *newRook);
int receiveKnight(int fd, knight *newKnight);
int receiveBishop(int fd, bishop *newBishop);
int receiveQueen(int fd, queen *newQueen);
int receiveKing(int fd, king *newKing);

int create_server_socket(struct sockaddr_in* sock_addr, sa_family_t sa_family, const char * ip_addr, in_port_t port, int type);
int connect_server(struct sockaddr_in* sock_addr, sa_family_t sa_family, const char * ip_addr, in_port_t port, int type);
void init_sock_addr_in(struct sockaddr_in* sock_addr, sa_family_t sa_family, const char * ip_addr, in_port_t port);


using namespace std;


int create_server_socket(struct sockaddr_in* sock_addr, sa_family_t sa_family, const char * ip_addr, in_port_t port, int type)
{
  int fd = socket(sa_family, type, 0);
  int optval = SO_REUSEPORT_OPTION_VALUE;
  setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(int));

  init_sock_addr_in(sock_addr, sa_family, ip_addr, port);

  int result = bind(fd, (struct sockaddr *)sock_addr, sizeof(struct sockaddr_in));
  if(result == -1){
    cout << "Error on bind, returning" << endl;
    return -1;
  }
  result = listen(fd, 10);

  return fd;
}

void init_sock_addr_in(struct sockaddr_in* sock_addr, sa_family_t sa_family, const char * ip_addr, 
in_port_t port){
  memset(sock_addr, 0, sizeof(struct sockaddr_in));
  sock_addr->sin_family = sa_family;
  sock_addr->sin_addr.s_addr = inet_addr(ip_addr);
  sock_addr->sin_port = htons(port);
}

/* Handle creating a server and waiting for client to connect, or connect to preexisting socket */
int createServerAndWait(void){
  struct sockaddr_in server_endpoint;
  struct sockaddr_in client_endpoint;
  int client_sock_desc = -1;
  int server_sock_desc = create_server_socket(&server_endpoint, AF_INET, HOST_IP_ADDRESS, 
  SERVER_PORT,SOCK_STREAM);
  cout << "Listening on Network Interface: " << HOST_IP_ADDRESS << " Network Port: " << SERVER_PORT << endl;
  int connected = 0;
  while(!connected){
    cout << "Waiting for client connection..." << endl;
    socklen_t sock_len = sizeof(struct sockaddr_in);
    client_sock_desc = accept(server_sock_desc, (struct sockaddr *)&client_endpoint, &sock_len);
    char *client_ip = inet_ntoa(client_endpoint.sin_addr);
    cout << "Accepted connection: " << client_ip << "L" << ntohs(client_endpoint.sin_port) << endl;
    connected = 1;
  }
  return client_sock_desc;
}

int connectToServer(void){
  struct sockaddr_in server_endpoint;
  int sock_desc = connect_server(&server_endpoint, AF_INET, HOST_IP_ADDRESS, SERVER_PORT, 
  SOCK_STREAM);
  return sock_desc;
}

int connect_server(struct sockaddr_in* sock_addr, sa_family_t sa_family, const char * ip_addr, in_port_t port, int type){
  int fd = socket(sa_family, type, 0);
  init_sock_addr_in(sock_addr, sa_family, ip_addr, port);
  int result = connect(fd, (struct sockaddr *)sock_addr, sizeof(struct sockaddr_in));
  if(result == -1){
    cout << "Error on connect, closing" << endl;
    return -1;
  }
  return fd;
}


/* Helper functions for the connections to communicate */

int sendAck(int fd){
  write(fd, ACKNOWLEDGE_MESSAGE, strlen(ACKNOWLEDGE_MESSAGE));
  return 0;
}

int receiveAck(int fd){
  char myBuf[RECEIVE_BUFFER_SIZE];
  read(fd, myBuf, strlen(ACKNOWLEDGE_MESSAGE));
  return 0;
}

int sendColor(int fd, char color){
  write(fd, &color, sizeof(char));
  receiveAck(fd);
  return 0;
}

int receiveColor(int fd, char *color){
  read(fd, color, sizeof(char));
  sendAck(fd);
  return 0;
}

// Because pieces have a variable size, for each piece we will need to send two messages, first one telling us what the piece is, the second being the actual piece
int sendBoard(int fd){
  for(int i = 0; i < BOARDSIZE; i++){
    for(int j = 0; j < BOARDSIZE; j++){
      piece *curPiece = board[i][j];
      char pieceType;
      if(curPiece == nullptr){
        pieceType = EMPTY; 
      } else {
        pieceType = curPiece->myType;
      }
      int written = write(fd, &pieceType, sizeof(char));
      receiveAck(fd);
      switch(pieceType) {
        case PAWN:
          sendPawn(fd, (pawn*)curPiece);
          break;
        case ROOK:
          sendRook(fd, (rook*)curPiece);
          break;
        case KNIGHT:
          sendKnight(fd, (knight*)curPiece);
          break;
        case BISHOP:
          sendBishop(fd, (bishop*)curPiece);
          break;
        case QUEEN:
          sendQueen(fd, (queen*)curPiece);
          break;
        case KING:
          sendKing(fd, (king*)curPiece);
          break;
        case EMPTY:
          break;
        default:
          cout << "Unknown piece identified when sending board: " << pieceType << "i: " << i << "j " << j << endl;
          return -1;
      }
    }
  }
  return 0;
}

int receiveBoard(int fd, char myColor, king *&myKing, king *&oppKing){
  for(int i = 0; i < BOARDSIZE; i++){
    for(int j = 0; j < BOARDSIZE; j++){
      // Need to clean up piece on current spot
      delete board[i][j];

      char pieceType;
      read(fd, &pieceType, sizeof(char));
      sendAck(fd);
      switch(pieceType){
        case PAWN:
        {
          pawn *newPawn = new pawn(myColor);
          receivePawn(fd, newPawn);
          board[i][j] = newPawn; 
          break;
        }
        case ROOK:
        {
          rook *newRook = new rook(myColor);
          receiveRook(fd, newRook);
          board[i][j] = newRook;
          break;
        }
        case KNIGHT:
        {
          knight *newKnight = new knight(myColor);
          receiveKnight(fd, newKnight);
          board[i][j] = newKnight;
          break;
        }
        case BISHOP:
        {
          bishop *newBishop = new bishop(myColor);
          receiveBishop(fd, newBishop);
          board[i][j] = newBishop; 
          break;
        }
        case QUEEN:
        {
          queen *newQueen = new queen(myColor);
          receiveQueen(fd, newQueen);
          board[i][j] = newQueen;
          break;
        }
        case KING:
        {
          king *newKing = new king(myColor);
          receiveKing(fd, newKing);
          board[i][j] = newKing;
          
          // Update player kings
          if(newKing->myColor == myColor){
            myKing = newKing;
          } else {
            oppKing = newKing;
          }
          break;
        }
        case EMPTY:
          board[i][j] = nullptr;
          break;
        default:
          cout << "Unknown piece identified when receiving board: " << pieceType << " i: " << i << " j " << j << endl;
          return -1;
      }
    }
  }
  return 0;
}

/* Heplers for sending and receiving individual pieces */
int sendPawn(int fd, pawn *myPawn){
  int written = write(fd, myPawn, PAWNSIZE);
  receiveAck(fd);
  return 0;
}

int sendRook(int fd, rook *myRook){
  write(fd, (myRook), ROOKSIZE);
  receiveAck(fd);
  return 0;
}

int sendKnight(int fd, knight *myKnight){
  write(fd, (myKnight), KNIGHTSIZE);
  receiveAck(fd);
  return 0;
}

int sendBishop(int fd, bishop *myBishop){
  write(fd, (myBishop), BISHOPSIZE);
  receiveAck(fd);
  return 0;
}

int sendQueen(int fd, queen *myQueen){
  write(fd, (myQueen), QUEENSIZE);
  receiveAck(fd);
  return 0;
}

int sendKing(int fd, king *myKing){
  int written = write(fd, (myKing), KINGSIZE);
  receiveAck(fd);
  return 0;
}

int receivePawn(int fd, pawn *newPawn){
  read(fd, (newPawn), sizeof(PAWNSIZE));
  sendAck(fd);
  return 0;
}

int receiveRook(int fd, rook *newRook){
  read(fd, (newRook), sizeof(ROOKSIZE));
  sendAck(fd);
  return 0;
}

int receiveKnight(int fd, knight *newKnight){
  read(fd, (newKnight), KNIGHTSIZE);
  sendAck(fd);
  return 0;
}

int receiveBishop(int fd, bishop *newBishop){
  read(fd, (newBishop), BISHOPSIZE);
  sendAck(fd);
  return 0;
}

int receiveQueen(int fd, queen *newQueen){
  read(fd, (newQueen), QUEENSIZE);
  sendAck(fd);
  return 0;
}

int receiveKing(int fd, king *newKing){
  read(fd, (newKing), KINGSIZE);
  sendAck(fd);
  return 0;
}

void sendAction(int fd, char *action){
  write(fd, action, sizeof(char));
  receiveAck(fd);
}

void receiveAction(int fd, char *action){
  read(fd, action, sizeof(char));
  sendAck(fd);
}

// This is essentially a wrapper however I don't want to directly be calling sendInt
void sendCaptured(int fd, int capturedVal){
  // Catch edge case of empty list
  sendInt(fd, capturedVal);
}

// Receive every piece opponent is sending, and if item differs from my list, update it
void receiveCaptured(int fd, vector<int> &capturedList){
  int receivedVal;
  receiveInt(fd, &receivedVal);

  if(receivedVal != 0){
    capturedList.push_back(receivedVal);
  }
  
  sort(capturedList.begin(), capturedList.end(), greater<int>());
}

void sendInt(int fd, int value){
  write(fd, &value, sizeof(int));
  receiveAck(fd);
}

void receiveInt(int fd, int *value){
  read(fd, value, sizeof(int));
  sendAck(fd);
}