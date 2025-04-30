#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <algorithm>
#include "list.h"

using namespace std;


int main(){
    Cards* myDeck = nullptr;
    int numCards = 0;

    myDeck = initializeDeck(numCards);
    std::cout << "Initial deck:" << std::endl;
    displayDeck(myDeck, numCards);

    myDeck = shuffleDeck(myDeck, numCards);
    std::cout << "\nShuffled deck:" << std::endl;
    displayDeck(myDeck, numCards);

}