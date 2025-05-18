#include <string>
#include "cards.h"

using namespace std;

struct Player{
    string nick;
    string role;
    int points;
    int numplay;
    Cards* deck;
    Player* next;

};

Player* createPlayer(Player*& p, string nick, int numplay){
    Player* newPlayer = new Player;
    newPlayer->nick = nick;
    newPlayer->role = "";
    newPlayer->points = 0;
    newPlayer->numplay = numplay;
    Player* ax = newPlayer;
    if (!p){ // si es vacia, ax es el primer elemento
		p = ax;
        p->next=p;
    }
	else { // si no, se mueve el puntero cabeza
        Player* temp = p;
        while (temp->next != p){
            temp = temp->next;
        }
		temp->next = newPlayer;
		newPlayer->next = p;
	};
}

void repartirCarta(Player* player, Cards* card) {
    card->next = NULL; 
    Cards* aux;

    if (player->deck == NULL) {
        player->deck = card;
        aux = card;
    } else {
        aux->next = card;
        aux->next = card;
    }
}

// Función para repartir todo el mazo de cartas a los jugadores
void repartirMazo(Cards*& baraja, Player* cabezaJugador) {
    if (baraja == nullptr || cabezaJugador == nullptr) {
        cout << "Error: Baraja o lista de jugadores no inicializada." << std::endl;
        return;
    }

    Player* jugadorActual = cabezaJugador;
    Cards* cartaActualBaraja = baraja;
    Cards* siguienteCartaBaraja = nullptr;

    while (cartaActualBaraja != nullptr) {
        siguienteCartaBaraja = cartaActualBaraja->next; // Guardar el siguiente antes de desenlazar

        // Repartir la carta al jugador actual
        repartirCarta(jugadorActual, cartaActualBaraja);

        // Mover al siguiente jugador (en la lista cíclica)
        jugadorActual = jugadorActual->next;

        // Avanzar a la siguiente carta en la baraja
        cartaActualBaraja = siguienteCartaBaraja;
    }

    // La baraja ahora está vacía (baraja se ha movido hasta nullptr)
}

void imprimirMano(const Player* jugador) {
    cout << "Mano de " << jugador->nick << ": ";
    Cards* cartaActualMano = jugador->deck;
    while (cartaActualMano != nullptr) {
        cout << cartaActualMano->value << cartaActualMano->suit;
        if (cartaActualMano->next != nullptr) {
            std::cout << ", ";
        }
        cartaActualMano = cartaActualMano->next;
    }
    cout << endl;
}

void displayplayers(Player* head) {
    if (!head){
        cout<<"No hay jugadores"<<endl;
        return;
    }
    Player* ax = head;
    do{
        cout<<"jugador "<<ax->numplay<<":"<<endl;
        cout << ax->nick<<endl;
        cout<<ax->points;
    } while (ax != head);
    cout<<endl;
}