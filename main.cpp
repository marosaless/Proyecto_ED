#include <vector>
#include <conio.h>
#include "players.h"

using namespace std;


int main(){
    Cards* myDeck = nullptr;
    Player* players = nullptr;
    Player* primerjugadore = nullptr;
    int numCards = 0;
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
                    cout<<"             COMIENZA EL JUEGO                  "<<endl;
                    cout<<"------------------------------------------------"<<endl; 
                    int ronda = 1;
                    cout<<"Ronda "<<ronda<<endl;
                    cout<<"------------------------------------------------"<<endl;
                    string turno = (primerJugadorConTresDiamantes(players, primerjugadore));
                    cambiarCabezaPorNombre(players, turno);
                    cout<<"Turno de "<<turno<<endl;
                    cout<<"presione una tecla para continuar..."<<endl;
                    _getch();
                    cout<<"------------------------------------------------"<<endl;
                    while(players->deck){
                        cout<<"------------------------------------------------"<<endl;
                        
                    }
                    break;
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
