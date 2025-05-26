#include <string>
#include <random>
#include <iostream>
#include <vector>
#include <algorithm>

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
    Cards* joker1 = createCard("JOKER", char(0));
    if (head == nullptr) {
        head = joker1;
        tail = joker1;
    } else {
        tail->next = joker1;
        tail = joker1;
    }
    numCard++;

    Cards* joker2 = createCard("JOKER", char(0));
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
            cout <<  current->suit;
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
    bool mazo = true;
    Cards* deck = nullptr;
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

void insertar(Cards **P, string value, char suit) {
    Cards* newcards= new Cards;
    newcards->value = value;
    newcards->suit=suit;
    newcards->next=*P;
    *P=newcards;

}

void repartirMazo(Cards*& deck, Player* headplayer) {
    if (!deck || !headplayer) {
        cout << "Error: Baraja o lista de jugadores no inicializada." << endl;
        return;
    }

    Player* jugadorActual = headplayer;
    Cards* cartaActualBaraja = deck;
    while (cartaActualBaraja != nullptr) {
        Cards* siguienteCartaBaraja = cartaActualBaraja->next; // Guardar el siguiente antes de desenlazar
        // Desenlazar la carta del mazo
        cartaActualBaraja->next = nullptr;
        // Añadir la carta al mazo del jugador actual
        insertar(&jugadorActual->deck, cartaActualBaraja->value, cartaActualBaraja->suit);
        delete cartaActualBaraja; // Liberar la memoria de la carta actual


        // Mover al siguiente jugador (en la lista cíclica)
        jugadorActual = jugadorActual->next;

        // Avanzar a la siguiente carta en la baraja
        cartaActualBaraja = siguienteCartaBaraja;
    }

    // Vaciar el mazo original
    deck = nullptr;
    cout << "Baraja repartida a los jugadores." << endl;
}

void imprimirMano(Player* jugador) {
    cout << "Mano de " << jugador->nick << ": ";
    Cards* current = jugador->deck;
    while (current != nullptr) {
        cout<<" | ";
        cout << current->value;
        if (current->suit != char(0)) {
            cout <<  current->suit;
        }
        cout << " | ";
        current = current->next;
    }
    
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

int getCardPriority(const string& value) {
    if (value == "JOKER") return 15; // Joker siempre al final
    if (value == "2") return 14;    // 2 tiene la máxima prioridad
    if (value == "A") return 13;    // As
    if (value == "K") return 12;    // Rey
    if (value == "Q") return 11;    // Reina
    if (value == "J") return 10;    // Jota
    return stoi(value);             // Para valores numéricos (3-10)
}

// Función para insertar un nuevo nodo al final de la lista
void insertEnd(Cards** head, int data, char suit) {
    Cards* newCard = new Cards; // Crea un nuevo nodo

    if (*head == nullptr) { // Si la lista está vacía, el nuevo nodo es la cabeza
        *head = newCard;
        return;
    }

    // Si no está vacía, recorre hasta el final y añade el nuevo nodo
    Cards* temp = *head;
    while (temp->next != nullptr) {
        temp = temp->next;
    }
    temp->next = newCard;
}

void ordenarMano(Cards*& deck) {
    if (!deck || !deck->next) return; // Si la lista está vacía o tiene un solo elemento, no hay nada que ordenar

    bool swapped;
    Cards* current;
    Cards* lastSorted = nullptr;
     // Puntero al último nodo ya ordenado en la pasada anterior
    do {

        swapped = false;
        current = deck;
        while (current->next != lastSorted) {

            // Comparar prioridades
            if (getCardPriority(current->value) > getCardPriority(current->next->value)) {
                // Si están en el orden incorrecto, intercambia sus datos
                swap(current->value, current->next->value);
                swap(current->suit, current->next->suit);
                swapped = true; // Indica que se realizó un intercambio
            }
            current = current->next;
        }
        lastSorted = current;
    } while (swapped);
}

void ordenarCartasJugadores(Player* headplayer) {
    if (!headplayer) return;

    Player* currentPlayer = headplayer;
    do {
        ordenarMano(currentPlayer->deck);
        currentPlayer = currentPlayer->next;
    } while (currentPlayer != headplayer);
}

// Función para cambiar el puntero 'cabeza' al nodo de un jugador específico
Player* cambiarCabezaPorNombre(Player* cabeza, string nombreBuscado) {
    if (cabeza == nullptr) {
        return nullptr;
    }

    Player* actual = cabeza;
    do {
        if (actual->nick == nombreBuscado) {
            return actual; // Se encontró el jugador, se devuelve este nodo como la nueva cabeza
        }
        actual = actual->next;
    } while (actual != cabeza); // Continúa hasta dar una vuelta completa
}

/*Pozo*/

struct Pozo{
    string value;
    int num;
    int suit;
    Cards* deck = nullptr; 
    Pozo* next;
};

void push (Pozo **P, string value, int num, int suit) {
    Pozo* newElement = new Pozo;
    newElement->value = value;
    newElement->num = num;
    newElement->suit = suit;
    newElement->next = *P;
    *P = newElement;
}

bool isVoid(Pozo* P){
    if (!P) return true;
    return false;
}

string top(Pozo* P){
    if(!P) return 0;
    else return P->value;
}

void vaciarPozo(Pozo** pozo) {
    while (!isVoid(*pozo)) {
        Pozo* temp = *pozo;
        *pozo = (*pozo)->next;
        delete temp; 
    }
    cout << "El pozo ha sido vaciado." << endl;
} 

void imprimirpozo(Pozo* pozo){
    if (isVoid(pozo)){
        cout << "El pozo esta vacio." << endl;
        return;
    }
    string num;
    switch (pozo->num){
        case 1: num = "(single)"; break;
        case 2: num = "(par)"; break;
        case 3: num = "(triple)"; break;
        case 4: num = "(poker)"; break;
        default: num = "(desconocido)"; break;
    }
    Cards* current = pozo->deck;
    while(current != nullptr) {
        cout<<" | ";
        cout << current->value;
        if (current->suit != char(0)) {
            cout <<  current->suit;
        }
        cout << " | ";
        current = current->next;
    }
    cout<<num<<endl;
}

void pasarCartasAlPozo(Player* jugador, Pozo* pozo, const string& value, const std::vector<char>& suits, int cantidad) {
    if (!jugador || !pozo || cantidad <= 0 || suits.size() != cantidad) return;

    if (value == "JOKER") {
        // Mover los primeros 'cantidad' jokers encontrados
        for (int i = 0; i < cantidad; ++i) {
            Cards** pp = &jugador->deck;
            bool encontrada = false;
            while (*pp) {
                Cards* actual = *pp;
                if (actual->value == "JOKER" && actual->suit == char(0)) {
                    *pp = actual->next;
                    actual->next = pozo->deck;
                    pozo->deck = actual;
                    encontrada = true;
                    break;
                } else {
                    pp = &((*pp)->next);
                }
            }
            if (!encontrada) {
                cout << "No se encontró un JOKER en la mano del jugador." << endl;
            }
        }
    } else {
        // Mismo comportamiento que antes para otras cartas
        if (suits.size() != cantidad) return;
        for (int i = 0; i < cantidad; ++i) {
            char suit = suits[i];
            Cards** pp = &jugador->deck;
            bool encontrada = false;
            while (*pp) {
                Cards* actual = *pp;
                if (actual->value == value && actual->suit == suit) {
                    *pp = actual->next;
                    actual->next = pozo->deck;
                    pozo->deck = actual;
                    encontrada = true;
                    break;
                } else {
                    pp = &((*pp)->next);
                }
            }
            if (!encontrada) {
                cout << "No se encontró la carta " << value << suit << " en la mano del jugador." << endl;
            }
        }
    }
}

/*Reglas especiales*/

bool tresdiamantes(Player*& headPlayer, Player*& primerJugador) {
    if (!headPlayer) return false;

    Player* actual = headPlayer;

    while (actual->next != headPlayer) {
        Cards* cartaActual = actual->deck;
        while (cartaActual) {
            if (cartaActual->value == "3" && cartaActual->suit == char(4)) { // ♦
                primerJugador = actual;
                return true;
            }
            cartaActual = cartaActual->next;
        }
        actual = actual->next;
    }

    return false; 
}

string primerJugadorConTresDiamantes(Player*& headPlayer, Player*& primerJugador) {
    if (tresdiamantes(headPlayer, primerJugador)) {
        string name = primerJugador->nick; 
        return name;
    }
}

bool validatevalueindeck(Cards* deck, string value) {
    string n = value;
    Cards* current = deck;
    while (current != nullptr) {
        if (current->value == n) {
            return true; // La carta está en la mano
        }
        current = current->next;
    }

    return false; // La carta no está en la mano

}

bool validatevalueinpozo(Pozo* pozo, string value) {
    if (pozo == nullptr) {
        return true; // El pozo está vacío
    }
    string n = value;
    Pozo* current = pozo;
    if (getCardPriority(current->value) < getCardPriority(n)) {
        return true; 
    }
    return false; 
}

bool validatenumindeck(Cards* deck, string value, int num) {
    Cards* current = deck;
    int count = 0;
    string n = value;
    transform(n.begin(), n.end(), n.begin(), ::toupper);
    while (current != nullptr) {
        if (current->value == n) {
            count++;
        }
        current = current->next;
    }
    return count >= num; // Devuelve true si hay al menos 'num' cartas del valor especificado
}

bool validatenuminpozo(Pozo* pozo, int num) {
    if (pozo == nullptr) {
        return true; // El pozo está vacío
    }
    Pozo* current = pozo;
    if (current->num == 1){
        return true;
    }
    else if(current->num == num) {
        return true; // El número de cartas en el pozo coincide con 'num'
    }
    return false; // El número de cartas en el pozo no coincide con 'num'
}