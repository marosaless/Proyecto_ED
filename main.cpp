#include <vector>
#include "players.h"

using namespace std;


int main(){
    Cards* myDeck = nullptr;
    Player* players = nullptr;
    int numCards = 0;
    string name1;
    string name2;
    myDeck = initializeDeck(numCards);
    myDeck = shuffleDeck(myDeck, numCards);
    displayDeck(myDeck, numCards);
    cout<<"Ingrese el nombre del jugador 1"<<endl;
    cin>>name1;
    createPlayer(players, name1, 1);
    cout<<"Ingrese el nombre del jugador 2"<<endl;
    cin>>name2;
    createPlayer(players, name2, 2);
    displayplayers(players);
    repartirMazo(myDeck, players);
    

}