#include <iostream>
#include "piece.h"

using namespace std;

bool piece::move(int xSource, int ySource, int xDest, int yDest, piece ownKing){
    cout << "Called piece move" << endl;
    return true;
}

void piece::printSelf(){
    cout << "Printing a generic piece: " << endl;
    cout << "My type is: " << this->myType << endl;
    cout << "My color is: " << this->myColor << endl;
}