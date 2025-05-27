#include <vector>
#include <conio.h>
#include "players.h"

using namespace std;


int main(){
    Cards* myDeck = nullptr;
    Player* players = nullptr;
    Player* actualplayer = nullptr;
    Pozo* pozo = nullptr;
    int numCards = 0;
    int n;
    int cont = 0;
    int ronda = 1;
    vector<Player*> ordenSalida;
    string name1;
    string name2;
    string name3;
    string name4;
    int option = -1;
    myDeck = initializeDeck(numCards);
    myDeck = shuffleDeck(myDeck, numCards);
    
    //displayDeck(myDeck, numCards);
    while (option){
        cout<<"------------------------------------------------"<<endl;
        cout<<"             BIENVENIDO A MAGNATE               "<<endl;
        cout<<"------------------------------------------------"<<endl;
        cout<<endl;
        cout<<"Comenzar juego (1)                      salir(0)"<<endl;       
        cin>>option;

        switch(option){
            case 0:
                cout<<"Saliendo del juego..."<<endl;
                break;
            case 1:
                cout<<"------------------------------------------------"<<endl;
                cout<<"             CREACION DE JUGADORES              "<<endl;
                cout<<"------------------------------------------------"<<endl;
                cout<<endl;
                cout<<"Ingrese el nombre del jugador 1"<<endl;
                cin>>name1;
                createPlayer(players, name1, 1);
                cout<<"Ingrese el nombre del jugador 2"<<endl;
                cin>>name2;
                createPlayer(players, name2, 2);
                cout<<"Ingrese el nombre del jugador 3"<<endl;
                cin>>name3;
                createPlayer(players, name3, 3);
                cout<<"Ingrese el nombre del jugador 4"<<endl;
                cin>>name4;
                createPlayer(players, name4, 4);
                cout<<endl<<"jugadores creados correctamente"<<endl;
                option = -1;
                repartirMazo(myDeck, players);
                ordenarCartasJugadores(players);
                while(option){
                    cout<<"------------------------------------------------"<<endl;
                    cout<<"                    RONDA 1                     "<<endl;
                    cout<<"------------------------------------------------"<<endl; 
                    cout<<"Ronda "<<ronda<<endl;
                    cout<<"------------------------------------------------"<<endl;
                    
                    while(actualplayer->mazo && actualplayer->next->mazo && actualplayer->next->next->mazo && actualplayer->next->next->next->mazo){
                        if (actualplayer->deck == nullptr && actualplayer->mazo) {
                            ordenSalida.push_back(actualplayer);
                            actualplayer->mazo = false;
                            actualplayer = actualplayer->next;
                        }  
                        string turno = (primerJugadorConTresDiamantes(players, actualplayer));
                        cambiarCabezaPorNombre(players, turno);
                        cout<<"Turno de "<<actualplayer->nick<<endl;
                        cout<<"presione una tecla para continuar..."<<endl;
                        _getch();
                        cout<<"------------------------------------------------"<<endl;
                        imprimirpozo(pozo);
                        cout<<"------------------------------------------------"<<endl;
                        imprimirMano(actualplayer);
                        cout<<endl;
                        cout<<"------------------------------------------------"<<endl;
                        cout<<"Ingrese 1 si desea jugar una carta"<<endl;
                        cout<<"Ingrese 2 si desea pasar el turno"<<endl;
                        cin>>n;
                        switch (n)
                        {
                        case 1: {
                            cout<<"ingrese el valor de la carta que desea jugar"<<endl;
                            string value;
                            cin>>value;
                            int suit = 0; // Inicializar suit antes de cualquier goto, break, continue, etc.
                            if (validatevalueindeck(actualplayer->deck, value) && validatevalueinpozo(pozo, value)){
                                cout<<"ingrese el numero de cartas que desea jugar"<<endl;
                                int num;
                                cin>>num;
                                vector<char> suits;
                                if (validatenumindeck(actualplayer->deck, value, num) && validatenuminpozo(pozo, num)){
                                    // Asignar el palo de la carta si es necesario
                                    if (value == "JOKER") {
                                        suit = 0; // Joker no tiene palo
                                        suits.push_back(char(0)); 
                                    } else {
                                        for (int i = 0; i < num; i++) {
                                            cout<<"Ingrese el palo de la carta (1: CORAZON, 2: DIAMANTE, 3: TREBOL, 4: PICAS): ";
                                            cin>>suit;
                                            
                                             suits.push_back(char(suit+2));
                                            if (suit < 1 || suit > 4) {
                                                cout<<"Palo no valido. Intente de nuevo."<<endl;
                                                i--; // Repetir la iteración para obtener un palo válido
                                                continue;
                                            }
                                        }
                                    }
                                    push(&pozo, value, num, suit);
                                    pasarCartasAlPozo(actualplayer, pozo, value, suits, num);
                                    cout<<"Carta(s) jugada(s) correctamente"<<endl;
                                    //imprimirpozo(pozo);
                                    actualplayer=actualplayer->next;
                                }
                            } else {
                                cout<<"No se puede jugar esa carta"<<endl;
                            }

                        }
                        break;
                        case 2:
                            cout<<"Turno pasado"<<endl;
                            actualplayer = actualplayer->next;
                            cont++;
                            if(cont == 3) {
                                vaciarPozo(&pozo);
                                cont = 0;
                            }
                            break;
                        default:
                            break;   
                        }
                        int jugadoresConCartas = 0;
                        Player* ultimoconCartas = nullptr;
                        Player* temp = players;
                        do{
                            if (temp->mazo) {
                                jugadoresConCartas++;
                                ultimoconCartas = temp;
                            }
                            temp = temp->next;
                        }while (temp != players);
                        if (jugadoresConCartas == 1){
                            ordenSalida.push_back(ultimoconCartas);
                            asignarRolesYPuntos(ordenSalida);
                        }

                    }
                    break;
                    ronda++;
                    cout<<"------------------------------------------------"<<endl;
                    cout<<"             FIN DE LA RONDA                    "<<endl;
                    cout<<"------------------------------------------------"<<endl;
                    _getch();
                    vaciarMazosJugadores(players);
                    vaciarPozo(&pozo);
                    freeDeck(myDeck);
                    myDeck = initializeDeck(numCards);
                    myDeck = shuffleDeck(myDeck, numCards);
                    repartirMazo(myDeck, players);
                    ordenarCartasJugadores(players);
                    Player* Magnate = getPlayerByRole(players, "Magnate");
                    Player* Rico = getPlayerByRole(players, "Rico");
                    Player* Pobre = getPlayerByRole(players, "Pobre");
                    Player* Mendigo = getPlayerByRole(players, "Mendigo");
                    if (Magnate && Rico && Pobre && Mendigo) {
                        cout<<"------------------------------------------------"<<endl;
                        cout<<"             RONDA DE PUNTUACION                 "<<endl;
                        cout<<"------------------------------------------------"<<endl;
                        cout<<"Puntos de "<<Magnate->nick<<": "<<Magnate->points<<endl;
                        cout<<"Puntos de "<<Rico->nick<<": "<<Rico->points<<endl;
                        cout<<"Puntos de "<<Pobre->nick<<": "<<Pobre->points<<endl;
                        cout<<"Puntos de "<<Mendigo->nick<<": "<<Mendigo->points<<endl;
                    } else {
                        cout<<"No se han asignado roles correctamente."<<endl;
                    }
                    _getch();
                    intercambiarCartasMagnateMendigo(Magnate, Mendigo);
                    intercambiarCartasRicoPobre(Rico, Pobre);
                    
                    actualplayer = getPlayerByRole(players, "Magnate");
                    while(actualplayer->mazo && actualplayer->next->mazo && actualplayer->next->next->mazo && actualplayer->next->next->next->mazo){
                        if (actualplayer->deck == nullptr && actualplayer->mazo) {
                            ordenSalida.push_back(actualplayer);
                            actualplayer->mazo = false;
                            actualplayer = actualplayer->next;
                        }  
                        cout<<"------------------------------------------------"<<endl;
                        cout<<"                    RONDA "<<ronda<<endl;
                        cout<<"------------------------------------------------"<<endl;
                        cout<<"turno de "<<actualplayer->nick<<endl;
                        cout<<"presione una tecla para continuar..."<<endl;
                        _getch();
                        cout<<"------------------------------------------------"<<endl;
                        imprimirpozo(pozo);
                        cout<<"------------------------------------------------"<<endl;
                        imprimirMano(actualplayer);
                        cout<<endl;
                        cout<<"------------------------------------------------"<<endl;
                        cout<<"Ingrese 1 si desea jugar una carta"<<endl;
                        cout<<"Ingrese 2 si desea pasar el turno"<<endl;
                        cin>>n;
                        switch (n)
                        {
                        case 1: {
                            cout<<"ingrese el valor de la carta que desea jugar"<<endl;
                            string value;
                            cin>>value;
                            int suit = 0; // Inicializar suit antes de cualquier goto, break, continue, etc.
                            if (validatevalueindeck(actualplayer->deck, value) && validatevalueinpozo(pozo, value)){
                                cout<<"ingrese el numero de cartas que desea jugar"<<endl;
                                int num;
                                cin>>num;
                                vector<char> suits;
                                if (validatenumindeck(actualplayer->deck, value, num) && validatenuminpozo(pozo, num)){
                                    // Asignar el palo de la carta si es necesario
                                    if (value == "JOKER") {
                                        suit = 0; // Joker no tiene palo
                                        suits.push_back(char(0)); 
                                    } else {
                                        for (int i = 0; i < num; i++) {
                                            cout<<"Ingrese el palo de la carta (1: CORAZON, 2: DIAMANTE, 3: TREBOL, 4: PICAS): ";
                                            cin>>suit;
                                            
                                             suits.push_back(char(suit+2));
                                            if (suit < 1 || suit > 4) {
                                                cout<<"Palo no valido. Intente de nuevo."<<endl;
                                                i--; // Repetir la iteración para obtener un palo válido
                                                continue;
                                            }
                                        }
                                    }
                                    push(&pozo, value, num, suit);
                                    pasarCartasAlPozo(actualplayer, pozo, value, suits, num);
                                    cout<<"Carta(s) jugada(s) correctamente"<<endl;
                                    //imprimirpozo(pozo);
                                    actualplayer=actualplayer->next;
                                }
                            } else {
                                cout<<"No se puede jugar esa carta"<<endl;
                            }

                        }
                        break;
                        case 2:
                            cout<<"Turno pasado"<<endl;
                            actualplayer = actualplayer->next;
                            cont++;
                            if(cont == 3) {
                                vaciarPozo(&pozo);
                                cont = 0;
                            }
                            break;
                        default:
                            break;   
                        }
                        int jugadoresConCartas = 0;
                        Player* ultimoconCartas = nullptr;
                        Player* temp = players;
                        do{
                            if (temp->mazo) {
                                jugadoresConCartas++;
                                ultimoconCartas = temp;
                            }
                            temp = temp->next;
                        }while (temp != players);
                        if (jugadoresConCartas == 1){
                            ordenSalida.push_back(ultimoconCartas);
                            asignarRolesYPuntosEspecial(ordenSalida, players);
                        }
                    break;
                    }
                    ronda++;
                    cout<<"------------------------------------------------"<<endl;
                    cout<<"             FIN DE LA RONDA                    "<<endl;
                    cout<<"------------------------------------------------"<<endl;
                    _getch();
                    vaciarMazosJugadores(players);
                    vaciarPozo(&pozo);
                    freeDeck(myDeck);
                    myDeck = initializeDeck(numCards);
                    myDeck = shuffleDeck(myDeck, numCards);
                    repartirMazo(myDeck, players);
                    ordenarCartasJugadores(players);

                }
            default:
                cout<<"Opcion no valida"<<endl;
            break;                                       
        }
       
    }
    


    /*cout<<"Ingrese el nombre del jugador 1"<<endl;
    cin>>name1;
    createPlayer(players, name1, 1);
    cout<<"Ingrese el nombre del jugador 2"<<endl;
    cin>>name2;
    createPlayer(players, name2, 2);
    displayplayers(players);
    repartirMazo(myDeck, players);
    imprimirMano(players);
    imprimirMano(players->next);
    ordenarCartasJugadores(players);
    cout<<"Baraja ordenada"<<endl;
    imprimirMano(players);
    imprimirMano(players->next);*/

}
