#ifndef PIECE
#define PIECE

#include "../constants.h"


class piece {
    public:
        char myColor;
        char myType;

        piece(char color, char type){
            myColor = color;
            myType = type;
        }

        piece(){}

        bool move(int xSource, 
                int ySource, 
                int xDest, 
                int yDest, 
                piece ownKing);

        void printSelf(void);
};

class pawn : public piece {
    public:
        bool hasMoved;
        bool lastMoveTwo;

        pawn(char color) : piece(color, PAWN){
            hasMoved = false;
            lastMoveTwo = false;
        }

        bool move(int xSource,
                int ySource,
                int xDest,
                int yDest,
                bool output,
                bool simulation,
                piece* (&board)[BOARDSIZE][BOARDSIZE]);

        bool validateMove(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        bool output,
                        piece* (&board)[BOARDSIZE][BOARDSIZE]);

        void placePiece(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        bool simulation,
                        piece* (&board)[BOARDSIZE][BOARDSIZE]);

        bool checkPassant(int xSource,
                    int ySource,
                    int xDest,
                    int yDest,
                    bool output,
                    piece* (&board)[BOARDSIZE][BOARDSIZE]);
};

class rook : public piece {
    public:

        bool hasMoved;

        rook(char color) : piece(color, ROOK){
            hasMoved = false;
        }

        bool move(int xSource,
                    int ySource,
                    int xDest,
                    int yDest,
                    bool output,
                    bool simulation,
                    piece* (&board)[BOARDSIZE][BOARDSIZE]);

        bool validateMove(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        bool output,
                        piece* (&board)[BOARDSIZE][BOARDSIZE]);

        void placePiece(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        bool simulation,
                        piece* (&board)[BOARDSIZE][BOARDSIZE]);
};

class knight : public piece {
    public:
        knight(char color) : piece(color, KNIGHT){}

        bool move(int xSource,
                int ySource,
                int xDest,
                int yDest,
                bool output,
                piece* (&board)[BOARDSIZE][BOARDSIZE]);

        bool validateMove(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        bool output,
                        piece* (&board)[BOARDSIZE][BOARDSIZE]);
        
        void placePiece(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        piece* (&board)[BOARDSIZE][BOARDSIZE]);
};

class bishop : public piece {
    public:
        bishop(char color) : piece(color, BISHOP){}

        bool move(int xSource,
                int ySource,
                int xDest,
                int yDest,
                bool output,
                piece* (&board)[BOARDSIZE][BOARDSIZE]);

        bool validateMove(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        bool output,
                        piece* (&board)[BOARDSIZE][BOARDSIZE]);

        void placePiece(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        piece* (&board)[BOARDSIZE][BOARDSIZE]);
};

class queen : public piece {
    public:
        queen(char color) : piece(color, QUEEN){}

        bool move(int xSource,
                int ySource,
                int xDest,
                int yDest,
                bool output,
                piece* (&board)[BOARDSIZE][BOARDSIZE]);

        bool validateMove(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        bool output, 
                        piece* (&board)[BOARDSIZE][BOARDSIZE]);

        bool validateStraight(int xSource,
                            int ySource,
                            int xDest,
                            int yDest,
                            bool output,
                            piece* (&board)[BOARDSIZE][BOARDSIZE]);

        bool validateDiagonal(int xSource,
                            int ySource,
                            int xDest,
                            int yDest,
                            int xChange,
                            int yChange,
                            bool output,
                            piece* (&board)[BOARDSIZE][BOARDSIZE]);
        
        void placePiece(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        piece* (&board)[BOARDSIZE][BOARDSIZE]);
};

// One day consider writing alg to determine checkmate
class king : public piece {
    public:
        int position [2];
        bool hasMoved;
        bool isCheck;
        bool isCheckmate;

        king(char color) : piece(color, KING){
            position[1] = 3;
            
            if(myColor == RED){
                position[0] = 7;
            } else {
                position[0] = 0;
            }

            this->isCheck = false;
            this->isCheckmate = false;
        }

        bool move(int xSource,
                int ySource,
                int xDest,
                int yDest,
                bool output,
                bool simulation,
                piece* (&board)[BOARDSIZE][BOARDSIZE]);

        bool validateMove(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        bool output,
                        piece* (&board)[BOARDSIZE][BOARDSIZE]);

        bool validateCastle(int xSource,
                            int ySource,
                            int xDest,
                            int yDest,
                            bool output,
                            piece* (&board)[BOARDSIZE][BOARDSIZE]);

        void placePiece(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        bool simulation,
                        piece* (&board)[BOARDSIZE][BOARDSIZE]);
        
        void printMySelf(void);
};

// Because these require the knowledge of piece classes these constants are initialized here instead of constants.h
constexpr int PAWNSIZE = sizeof(pawn);
constexpr int ROOKSIZE = sizeof(rook);
constexpr int KNIGHTSIZE = sizeof(knight);
constexpr int BISHOPSIZE = sizeof(bishop);
constexpr int QUEENSIZE = sizeof(queen);
constexpr int KINGSIZE = sizeof(king);

bool isMoveChecking(int xSource, int ySource, int xDest, int yDest, king* theKing);
bool isCheck(king* theKing, piece* (&board)[BOARDSIZE][BOARDSIZE]);

#endif