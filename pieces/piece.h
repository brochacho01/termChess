#ifndef PIECE
#define PIECE

#define PAWN 'p'
#define ROOK 'r'
#define KNIGHT 'n'
#define BISHOP 'b'
#define QUEEN 'q'
#define KING 'k'
#define EMPTY '0'

#define PAWNSIZE sizeof(pawn)
#define ROOKSIZE sizeof(rook)
#define KNIGHTSIZE sizeof(knight)
#define BISHOPSIZE sizeof(bishop)
#define QUEENSIZE sizeof(queen)
#define KINGSIZE sizeof(king)

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

        pawn(char color){
            myColor = color;
            myType = PAWN;
            hasMoved = false;
            lastMoveTwo = false;
        }

        bool move(int xSource,
                int ySource,
                int xDest,
                int yDest,
                piece* (&board)[8][8]);

        bool validateMove(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        bool output,
                        piece* (&board)[8][8]);

        void placePiece(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        piece* (&board)[8][8]);
};

class rook : public piece {
    public:

        bool hasMoved;

        rook(char color){
            myColor = color;
            myType = ROOK;
            hasMoved = false;
        }

        bool move(int xSource,
                    int ySource,
                    int xDest,
                    int yDest,
                    piece* (&board)[8][8]);

        bool validateMove(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        bool output,
                        piece* (&board)[8][8]);

        void placePiece(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        piece* (&board)[8][8]);
};

class knight : public piece {
    public:
        knight(char color){
            myColor = color;
            myType = KNIGHT;
        }

        bool move(int xSource,
                int ySource,
                int xDest,
                int yDest,
                piece* (&board)[8][8]);

        bool validateMove(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        bool output,
                        piece* (&board)[8][8]);
        
        void placePiece(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        piece* (&board)[8][8]);
};

class bishop : public piece {
    public:
        bishop(char color){
            myColor = color;
            myType = BISHOP;
        }

        bool move(int xSource,
                int ySource,
                int xDest,
                int yDest,
                piece* (&board)[8][8]);

        bool validateMove(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        bool output,
                        piece* (&board)[8][8]);

        void placePiece(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        piece* (&board)[8][8]);
};

class queen : public piece {
    public:
        queen(char color){
            myColor = color;
            myType = QUEEN;
        }

        bool move(int xSource,
                int ySource,
                int xDest,
                int yDest,
                piece* (&board)[8][8]);

        bool validateMove(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        bool output, 
                        piece* (&board)[8][8]);

        bool validateStraight(int xSource,
                            int ySource,
                            int xDest,
                            int yDest,
                            bool output,
                            piece* (&board)[8][8]);

        bool validateDiagonal(int xSource,
                            int ySource,
                            int xDest,
                            int yDest,
                            int xChange,
                            int yChange,
                            bool output,
                            piece* (&board)[8][8]);
        
        void placePiece(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        piece* (&board)[8][8]);
};

class king : public piece {
    public:
        int position [2];
        bool hasMoved;
        bool isCheck;
        bool isCheckmate;

        king(char color){
            myColor = color;
            myType = KING;

            position[1] = 4;
            
            if(myColor == 'r'){
                position[0] = 7;
            } else {
                position[0] = 0;
            }
        }

        bool move(int xSource,
                int ySource,
                int xDest,
                int yDest,
                piece* (&board)[8][8]);

        bool validateMove(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        bool output,
                        piece* (&board)[8][8]);

        bool validateCastle(int xSource,
                            int ySource,
                            int xDest,
                            int yDest,
                            bool output,
                            piece* (&board)[8][8]);

        void placePiece(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        piece* (&board)[8][8]);
        
        void printMySelf(void);
};

bool isChecking(int xSource, int ySource, int xDest, int yDest, king* theKing);

#endif