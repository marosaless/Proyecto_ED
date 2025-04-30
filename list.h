
using namespace std;

struct Cards{
    string value;
    char suit;
    Cards* next;
};

Cards* createCard(const string& value, const char& suit) {
    Cards* newCard = new Cards;
    newCard->value = value;
    newCard->suit = suit;
    newCard->next = NULL;
    return newCard;
}

// Función para inicializar la baraja y devolver la cabeza de la lista enlazada
Cards* initializeDeck(int& numCard) {
    Cards* head = NULL;
    Cards* tail = NULL;
    char suits[] = {char(3), char(4), char(5), char(6)};
    string values[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

    for (const char& suit : suits) {
        for (const string& value : values) {
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

    std::random_device rd;
    std::mt19937 gen(rd());

    Cards* current = head;
    for (int i = numCards - 1; i > 0; --i) {
        std::uniform_int_distribution<> distrib(0, i);
        int j = distrib(gen);

        // Find the nodes at indices i and j
        Cards* nodeI = getNodeAtIndex(head, i);
        Cards* nodeJ = getNodeAtIndex(head, j);

        // Swap the values and suits of the nodes
        if (nodeI && nodeJ) {
            std::swap(nodeI->value, nodeJ->value);
            std::swap(nodeI->suit, nodeJ->suit);
        }
    }

    return head;
}

void displayDeck(Cards* head, int numCards) {
    Cards* current = head;
    while (current != nullptr) {
        cout << current->value;
        if (current->suit != char(0)) {
            std::cout <<  current->suit;
        }
        std::cout << " -> ";
        current = current->next;
    }
    std::cout << "nullptr (" << numCards << " cards)" << std::endl;
}
