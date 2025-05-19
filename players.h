#include <string>
#include <random>
#include <iostream>
#include <vector>

using namespace std;

/*Cartas*/
struct Cards{
    string value;
    char suit;
    Cards* next;
};

Cards* createCard(string value, char suit) {
    Cards* newCard = new Cards;
    newCard->value = value;
    newCard->suit = suit;
    newCard->next = nullptr;
    return newCard;
}
// Función para crear una nueva carta y devolver su dirección


// Función para inicializar la baraja y devolver la cabeza de la lista enlazada
Cards* initializeDeck(int& numCard) {
    Cards* head = NULL;
    Cards* tail = NULL;
    char suits[] = {char(3), char(4), char(5), char(6)};
    string values[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

    for (char& suit : suits) {
        for (string& value : values) {
            Cards* newCard = createCard(value, suit);
            if (head == NULL) {
                head = newCard;
                tail = newCard;
            } else {
                tail->next = newCard;
                tail = newCard;
            }
            numCard++;
        }
    }

    // Agregar los jokers
    Cards* joker1 = createCard("Joker", char(0));
    if (head == nullptr) {
        head = joker1;
        tail = joker1;
    } else {
        tail->next = joker1;
        tail = joker1;
    }
    numCard++;

    Cards* joker2 = createCard("Joker", char(0));
    if (head == nullptr) {
        head = joker2;
        tail = joker2;
    } else {
        tail->next = joker2;
        tail = joker2;
    }
    numCard++;

    return head;
}
Cards* getNodeAtIndex(Cards* head, int index) {
    Cards* current = head;
    int count = 0;
    while (current != nullptr && count < index) {
        current = current->next;
        count++;
    }
    return current;
}

Cards* shuffleDeck(Cards* head, int numCards) {
    if (numCards <= 1 || head == nullptr) {
        return head;
    }

    random_device rd;
    mt19937 gen(rd());

    Cards* current = head;
    for (int i = numCards - 1; i > 0; --i) {
        uniform_int_distribution<> distrib(0, i);
        int j = distrib(gen);

        // Find the nodes at indices i and j
        Cards* nodeI = getNodeAtIndex(head, i);
        Cards* nodeJ = getNodeAtIndex(head, j);

        // Swap the values and suits of the nodes
        if (nodeI && nodeJ) {
            swap(nodeI->value, nodeJ->value);
            swap(nodeI->suit, nodeJ->suit);
        }
    }

    return head;
}

void displayDeck(Cards* head, int numCards) {
    Cards* current = head;
    while (current != nullptr) {
        cout<<" | ";
        cout << current->value;
        if (current->suit != char(0)) {
            std::cout <<  current->suit;
        }
        cout << " | ";
        current = current->next;
    }
    cout << "(" << numCards << " cards)" << endl;
}

void freeDeck(Cards* head) {
    while (head != nullptr) {
        Cards* temp = head;
        head = head->next;
        delete temp;
    }
}

/*Jugadores*/

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
    cout<<"jugador creado correctamente"<<endl;
}

void repartirCarta(Player* player, Cards* card) {
    card->next = nullptr; 
    Cards* aux;

    if (player->deck == nullptr) {
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
        ax = ax->next;
        cout<<endl;
    } while (ax != head);
    cout<<endl;
}

