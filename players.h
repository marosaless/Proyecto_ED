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

Cards* shuffleDeck(Cards* head) {
    if (!head || !head->next) return head;

    // 1. Copiar las cartas a un vector
    vector<Cards*> cartas;
    Cards* current = head;
    while (current) {
        cartas.push_back(current);
        current = current->next;
    }

    // 2. Barajar el vector
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cartas.begin(), cartas.end(), g);

    // 3. Reconstruir la lista enlazada
    for (size_t i = 0; i < cartas.size() - 1; ++i) {
        cartas[i]->next = cartas[i + 1];
    }
    cartas.back()->next = nullptr;

    // 4. Retornar la nueva cabeza
    return cartas[0];
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

bool invertirirjerarquia = false; // Variable global para controlar la jerarquía de las cartas

void setInvertirJerarquia(bool valor) {
    invertirirjerarquia = valor; // Cambia el estado de la variable global
}

int getCardPriority(string value) {
    if(!invertirirjerarquia){
        if (value == "JOKER") return 16; // Joker siempre al final
        if (value == "2") return 15;    // 2 tiene la máxima prioridad
        if (value == "A") return 14;    // As
        if (value == "K") return 13;    // Rey
        if (value == "Q") return 12;    // Reina
        if (value == "J") return 11;    // Jota
        return stoi(value);             // Para valores numéricos (3-10)
    }
    else {
        if (value == "JOKER") return 0; // Joker siempre al final
        if (value == "2") return 1;    // 2 tiene la máxima prioridad
        if (value == "A") return 2;    // As
        if (value == "K") return 3;    // Rey
        if (value == "Q") return 4;    // Reina
        if (value == "J") return 5;    // Jota
        return stoi(value) + 6;        // Para valores numéricos (3-10)
    }
}



// Función para insertar un nuevo nodo al final de la lista
/*void insertEnd(Cards** head, int data, char suit) {
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
}*/

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

void vaciarMazosJugadores(Player* head) {
    if (!head) return;
    Player* actual = head;
    do {
        // Liberar todas las cartas del mazo del jugador actual
        while (actual->deck != nullptr) {
            Cards* temp = actual->deck;
            actual->deck = actual->deck->next;
            delete temp;
        }
        actual->deck = nullptr;
        actual = actual->next;
    } while (actual != head);
}

Player* getPlayerByRole(Player* head, string role){
    if (!head) return nullptr;
    Player* actual = head;
    do {
        if (actual->role == role) {
            return actual;
        }
        actual = actual->next;
    } while (actual != head);
    return head; // Si no se encuentra, retorna la cabeza original
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

    int cartasColocadas = 0;
    std::vector<Cards*> cartasNormales;
    std::vector<Cards*> jokers;

    // 1. Buscar cartas normales (valor y palo)
    for (int i = 0; i < suits.size(); ++i) {
        char suit = suits[i];
        Cards** pp = &jugador->deck;
        bool encontrada = false;
        while (*pp) {
            Cards* actual = *pp;
            if (actual->value == value && actual->suit == suit) {
                // Quitar de la mano
                *pp = actual->next;
                actual->next = nullptr;
                cartasNormales.push_back(actual);
                encontrada = true;
                break;
            } else {
                pp = &((*pp)->next);
            }
        }
    }

    // 2. Si faltan cartas, buscar jokers
    int faltan = cantidad - cartasNormales.size();
    for (int i = 0; i < faltan; ++i) {
        Cards** pp = &jugador->deck;
        bool encontrada = false;
        while (*pp) {
            Cards* actual = *pp;
            if (actual->value == "JOKER" && actual->suit == char(0)) {
                *pp = actual->next;
                actual->next = nullptr;
                jokers.push_back(actual);
                encontrada = true;
                break;
            } else {
                pp = &((*pp)->next);
            }
        }
        if (!encontrada) {
            cout << "No se encontró un JOKER para completar la jugada." << endl;
        }
    }

    // 3. Poner todas las cartas en el pozo
    for (Cards* c : cartasNormales) {
        c->next = pozo->deck;
        pozo->deck = c;
        cartasColocadas++;
    }
    for (Cards* c : jokers) {
        c->next = pozo->deck;
        pozo->deck = c;
        cartasColocadas++;
    }

    if (cartasColocadas < cantidad) {
        cout << "No se pudieron colocar todas las cartas requeridas en el pozo." << endl;
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

bool validatevalueinpozo(Pozo* pozo, string value, int num) {
    if (pozo == nullptr) {
        return true; // El pozo está vacío
    }
    string n = value;
    Pozo* current = pozo;
    if (getCardPriority(current->value) < getCardPriority(n)) {
        return true; 
    }
    else if(current->value == "JOKER" && current->num == 1 && n == "3" && current->suit == char(6)) {
        return true; // El pozo tiene un JOKER y el valor es 3♠
    }
    else if(current->value == n) {
        return true; // El valor de la carta en el pozo coincide con el valor ingresado
    }
    return false; 
}

bool validatenumindeck(Cards* deck, string value, int num) {
    Cards* current = deck;
    int count = 0;
    int jokers = 0;
    while (current != nullptr) {
        if (current->value == value) {
            count++;
        }
        else if(current->value == "JOKER") {
            jokers++;
        }
        current = current->next;
    }
    return (count+jokers) >= num; // Devuelve true si hay al menos 'num' cartas del valor especificado
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

void asignarRolesYPuntos(vector<Player*>& ordenSalida){
    if (ordenSalida.size() != 4) return;
    ordenSalida[0]->role = "Magnate";
    ordenSalida[0]->points += 30;
    ordenSalida[1]->role = "Rico";
    ordenSalida[1]->points += 20;
    ordenSalida[2]->role = "Pobre";
    ordenSalida[2]->points += 10;
    ordenSalida[3]->role = "Mendigo";
}

void asignarRolesYPuntosEspecial(vector<Player*>& ordenSalida, Player* jugadores) {
    if (ordenSalida.size() != 4) return;

    // Buscar al magnate anterior
    Player* magnateAnterior = nullptr;
    Player* temp = jugadores;
    do {
        if (temp->role == "Magnate") {
            magnateAnterior = temp;
            break;
        }
        temp = temp->next;
    } while (temp != jugadores);

    // Si el primero en salir NO es el magnate anterior
    if (magnateAnterior && ordenSalida[0] != magnateAnterior) {
        // El magnate anterior pasa a ser mendigo y no suma puntos
        magnateAnterior->role = "Mendigo";
        // El primero en salir es el nuevo magnate
        ordenSalida[0]->role = "Magnate";
        ordenSalida[0]->points += 30;
        ordenSalida[1]->role = "Rico";
        ordenSalida[1]->points += 20;
        ordenSalida[2]->role = "Pobre";
        ordenSalida[2]->points += 10;
        // El magnate anterior ya fue marcado como mendigo, no suma puntos
        // El cuarto jugador (ordenSalida[3]) recibe el rol que le corresponde si no es el magnate anterior
        if (ordenSalida[3] != magnateAnterior) {
            ordenSalida[3]->role = "Mendigo";
            // No suma puntos
        }
    } else {
        // Reparto normal
        ordenSalida[0]->role = "Magnate";
        ordenSalida[0]->points += 30;
        ordenSalida[1]->role = "Rico";
        ordenSalida[1]->points += 20;
        ordenSalida[2]->role = "Pobre";
        ordenSalida[2]->points += 10;
        ordenSalida[3]->role = "Mendigo";
        // No suma puntos
    }
}

Cards* seleccionarCartas(Cards*& deck, int cantidad) {
    if (!deck) return nullptr;

    Cards* seleccionadas = nullptr;
    
    for (int i = 0; i < cantidad; i++) {
        string valor;
        char suit;
        cout << "Ingresa el valor y el palo de la carta que quieres seleccionar (Ejemplo: 3 ♦): ";
        cout << "Para colocar el palo, usa los siguientes caracteres mientras pulsas (alt): " << endl;
        cout << "Corazón: ♥ (3), Diamante: ♦ (4), Trébol: ♣ (5), Pica: ♠ (6)" << endl;
        cin >> valor >> suit;

        // Buscar y eliminar la carta de la mano del jugador
        Cards* actual = deck;
        Cards* anterior = nullptr;

        while (actual) {
            if (actual->value == valor && actual->suit == suit) { 
                if (anterior) {
                    anterior->next = actual->next;
                } else {
                    deck = actual->next;
                }

                // Agregar la carta seleccionada a la nueva lista
                actual->next = seleccionadas;
                seleccionadas = actual;

                cout << "Carta " << valor << suit << " seleccionada." << endl;
                break;
            }
            anterior = actual;
            actual = actual->next;
        }
    }
    return seleccionadas;
}


Cards* obtenerCartasMasAltas(Cards*& deck, int cantidad) {
    if (!deck) return nullptr;

    Cards* seleccionadas = nullptr;
    
    for (int i = 0; i < cantidad; i++) {
        Cards* cartaMasAlta = deck;
        Cards* actual = deck;
        Cards* anterior = nullptr;
        Cards* anteriorMasAlta = nullptr;

        // Buscar la carta de mayor jerarquía
        while (actual) {
            if (getCardPriority(actual->value) > getCardPriority(cartaMasAlta->value)) {
                cartaMasAlta = actual;
                anteriorMasAlta = anterior;
            }
            anterior = actual;
            actual = actual->next;
        }

        // Eliminar la carta de la mano y agregarla a la nueva lista
        if (cartaMasAlta) {
            if (anteriorMasAlta) {
                anteriorMasAlta->next = cartaMasAlta->next;
            } else {
                deck = cartaMasAlta->next;
            }
            cartaMasAlta->next = seleccionadas;
            seleccionadas = cartaMasAlta;
        }
    }
    return seleccionadas;
}


void eliminarCarta(Cards*& deck, string valor, char suit) {
    if (!deck) return;

    Cards* actual = deck;
    Cards* anterior = nullptr;

    while (actual) {
        if (actual->value == valor && actual->suit == suit) {
            if (anterior) {
                anterior->next = actual->next;
            } else {
                deck = actual->next;
            }
            delete actual;
            return;
        }
        anterior = actual;
        actual = actual->next;
    }
}


void insertar(Cards*& deck, string value, char suit) {
    Cards* nuevaCarta = new Cards;
    nuevaCarta->value = value;
    nuevaCarta->suit = suit;
    nuevaCarta->next = deck;
    deck = nuevaCarta;
}


void intercambiarCartasMagnateMendigo(Player* magnate, Player* mendigo) {
    if (!magnate || !mendigo) {
        cout << "Error: Jugadores no válidos." << endl;
        return;
    }

    cout << magnate->nick << " (Magnate), selecciona dos cartas para dar al Mendigo:" << endl;
    Cards* seleccionadasMagnate = seleccionarCartas(magnate->deck, 2);

    while (seleccionadasMagnate) {
        insertar(mendigo->deck, seleccionadasMagnate->value, seleccionadasMagnate->suit);
        Cards* temp = seleccionadasMagnate;
        seleccionadasMagnate = seleccionadasMagnate->next;
        delete temp;
    }

    cout << mendigo->nick << " (Mendigo), cede sus dos cartas de mayor jerarquía al Magnate:" << endl;
    Cards* cartasMendigo = obtenerCartasMasAltas(mendigo->deck, 2);

    while (cartasMendigo) {
        insertar(magnate->deck, cartasMendigo->value, cartasMendigo->suit);
        Cards* temp = cartasMendigo;
        cartasMendigo = cartasMendigo->next;
        delete temp;
    }

    cout << "Intercambio completo entre " << magnate->nick << " (Magnate) y " << mendigo->nick << " (Mendigo)." << endl;
}


void intercambiarCartasRicoPobre(Player* rico, Player* pobre) {
    if (!rico || !pobre) {
        cout << "Error: Jugadores no válidos." << endl;
        return;
    }

    cout << rico->nick << " (Rico), selecciona una carta para dar al Pobre:" << endl;
    Cards* seleccionadasRico = seleccionarCartas(rico->deck, 1);

    while (seleccionadasRico) {
        insertar(pobre->deck, seleccionadasRico->value, seleccionadasRico->suit);
        Cards* temp = seleccionadasRico;
        seleccionadasRico = seleccionadasRico->next;
        delete temp;
    }

    cout << pobre->nick << " (Pobre), cede su carta de mayor jerarquía al Rico:" << endl;
    Cards* cartasPobre = obtenerCartasMasAltas(pobre->deck, 1);

    while (cartasPobre) {
        insertar(rico->deck, cartasPobre->value, cartasPobre->suit);
        Cards* temp = cartasPobre;
        cartasPobre = cartasPobre->next;
        delete temp;
    }

    cout << "Intercambio completo entre " << rico->nick << " (Rico) y " << pobre->nick << " (Pobre)." << endl;
}

void mostrarRankingFinal(Player* head) {
    if (!head) return;
    vector<Player*> ranking;
    Player* temp = head;
    do {
        ranking.push_back(temp);
        temp = temp->next;
    } while (temp != head);

    // Ordenar de mayor a menor por puntos
    std::sort(ranking.begin(), ranking.end(), [](Player* a, Player* b) {
        return a->points > b->points;
    });

    for (size_t i = 0; i < ranking.size(); ++i) {
        cout << i+1 << ". " << ranking[i]->nick << " (" << ranking[i]->role << ") - " << ranking[i]->points << " puntos" << endl;
    }
}
