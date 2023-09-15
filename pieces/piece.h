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

        void move(int xSource, 
                int ySource, 
                int xDest, 
                int yDest, 
                piece ownKing);
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

        void move(int xSource,
                int ySource,
                int xDest,
                int yDest,
                piece ownKing);

    private:
        bool validateMove(int xSource,
                        int ySource,
                        int xDest,
                        int yDest,
                        piece ownKing);

        void placePiece(int xSource,
                        int ySource,
                        int xDest,
                        int yDest);
};

class rook : public piece {
    public:
        rook(char color){
            myColor = color;
            myType = ROOK;
        }

        void move(int xSource,
                    int ySource,
                    int xDest,
                    int yDest,
                    piece ownKing);
};

class knight : public piece {
    public:
        knight(char color){
            myColor = color;
            myType = KNIGHT;
        }

        void move(int xSource,
                int ySource,
                int xDest,
                int yDest,
                piece ownKing);
};

class bishop : public piece {
    public:
        bishop(char color){
            myColor = color;
            myType = BISHOP;
        }

        void move(int xSource,
                int ySource,
                int xDest,
                int yDest,
                piece ownKing);
};

class queen : public piece {
    public:
        queen(char color){
            myColor = color;
            myType = QUEEN;
        }

        void move(int xSource,
                int ySource,
                int xDest,
                int yDest,
                piece ownKing);
};

class king : public piece {
    public:
        int position [2];
        bool isCheck;
        bool isCheckmate;

        king(char color){
            myColor = color;
            myType = KING;

            position[0] = 5;
            
            if(myColor == 'r'){
                position[1] = 1;
            } else {
                position[1] = 8;
            }
        }

        void move(int xSource,
                int ySource,
                int xDest,
                int yDest,
                piece ownKing);
        
        void printSelf(void);
};

#endif