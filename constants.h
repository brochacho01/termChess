#ifndef CONSTANTS
#define CONSTANTS

// Networking constants
constexpr int RECEIVE_BUFFER_SIZE = 200;
constexpr int MESSAGE_BUFFER_SIZE = 200;
constexpr const char ACKNOWLEDGE_MESSAGE[20] = "ACKNOWLEDGE_MESSAGE";
constexpr char SEND_MESSAGE_AFFIRMATION = 'y';
constexpr char STATUS_MESSAGE_SUCCESS[23] = "STATUS_MESSAGE_SUCCESS";
constexpr char STATUS_MESSAGE_FAIL[20] = "STATUS_MESSAGE_FAIL";
constexpr int SO_REUSEPORT_OPTION_VALUE = 1;
constexpr char HOST_IP_ADDRESS[8] = "0.0.0.0";
constexpr char CONNCT_IP_ADDRESS[10] = "127.0.0.1";
constexpr int SERVER_PORT = 1234;

// Piece constants
constexpr char PAWN = 'P';
constexpr char ROOK = 'R';
constexpr char KNIGHT = 'N';
constexpr char BISHOP = 'B';
constexpr char QUEEN = 'Q';
constexpr char KING = 'K';
constexpr char EMPTY = '0';

// Board constants
constexpr int BOARDSIZE = 8;

// Constatns for kings
constexpr int LONGCASTLEDEST = 5;
constexpr int SHORTCASTLEDEST = 1;

#endif