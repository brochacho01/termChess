#include <iostream>
#include "piece.h"

using namespace std;

bool king::move(int xSource, int ySource, int xDest, int yDest, piece ownKing){
    return true;
}

void king::printSelf(void){
    cout << "Position[0] is: " << this->position[0] << endl;
    cout << "Position[1] is: " << this->position[1] << endl;
    cout << "isCheck is: " << this->isCheck << endl;
    cout << "isCheckMate is: " << this->isCheckmate << endl;
}