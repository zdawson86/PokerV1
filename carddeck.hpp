/*
Description:
This sets up a deck of cards to be used for various card games.

*/

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <array>
using namespace std;

typedef enum pips{acelow, two, three, four, five, six, seven, eight, nine, ten, jack, queen, king, ace, jokerpips} pips;
typedef enum suit{club, diamond, heart, spade, jokersuit} suit;
typedef struct card{pips p; suit s;} card;
typedef enum deckType{singleDeck, doubleDeck, euchreDeck} deckType;

card makeCard(pips p, suit s) {
    card myCard = {p, s};
    return myCard;
}

vector<card> createDeck(deckType type, int jokerCount) {
    vector<card> cardDeck;
    switch (type) {
        case singleDeck:
            cardDeck.push_back(makeCard(two, club));
            cardDeck.push_back(makeCard(three, club));
            cardDeck.push_back(makeCard(four, club));
            cardDeck.push_back(makeCard(five, club));
            cardDeck.push_back(makeCard(six, club));
            cardDeck.push_back(makeCard(seven, club));
            cardDeck.push_back(makeCard(eight, club));
            cardDeck.push_back(makeCard(nine, club));
            cardDeck.push_back(makeCard(ten, club));
            cardDeck.push_back(makeCard(jack, club));
            cardDeck.push_back(makeCard(queen, club));
            cardDeck.push_back(makeCard(king, club));
            cardDeck.push_back(makeCard(ace, club));
            cardDeck.push_back(makeCard(two, diamond));
            cardDeck.push_back(makeCard(three, diamond));
            cardDeck.push_back(makeCard(four, diamond));
            cardDeck.push_back(makeCard(five, diamond));
            cardDeck.push_back(makeCard(six, diamond));
            cardDeck.push_back(makeCard(seven, diamond));
            cardDeck.push_back(makeCard(eight, diamond));
            cardDeck.push_back(makeCard(nine, diamond));
            cardDeck.push_back(makeCard(ten, diamond));
            cardDeck.push_back(makeCard(jack, diamond));
            cardDeck.push_back(makeCard(queen, diamond));
            cardDeck.push_back(makeCard(king, diamond));
            cardDeck.push_back(makeCard(ace, diamond));
            cardDeck.push_back(makeCard(two, heart));
            cardDeck.push_back(makeCard(three, heart));
            cardDeck.push_back(makeCard(four, heart));
            cardDeck.push_back(makeCard(five, heart));
            cardDeck.push_back(makeCard(six, heart));
            cardDeck.push_back(makeCard(seven, heart));
            cardDeck.push_back(makeCard(eight, heart));
            cardDeck.push_back(makeCard(nine, heart));
            cardDeck.push_back(makeCard(ten, heart));
            cardDeck.push_back(makeCard(jack, heart));
            cardDeck.push_back(makeCard(queen, heart));
            cardDeck.push_back(makeCard(king, heart));
            cardDeck.push_back(makeCard(ace, heart));
            cardDeck.push_back(makeCard(two, spade));
            cardDeck.push_back(makeCard(three, spade));
            cardDeck.push_back(makeCard(four, spade));
            cardDeck.push_back(makeCard(five, spade));
            cardDeck.push_back(makeCard(six, spade));
            cardDeck.push_back(makeCard(seven, spade));
            cardDeck.push_back(makeCard(eight, spade));
            cardDeck.push_back(makeCard(nine, spade));
            cardDeck.push_back(makeCard(ten, spade));
            cardDeck.push_back(makeCard(jack, spade));
            cardDeck.push_back(makeCard(queen, spade));
            cardDeck.push_back(makeCard(king, spade));
            cardDeck.push_back(makeCard(ace, spade));            
            break;
        case doubleDeck:
            cardDeck.push_back(makeCard(two, club));
            cardDeck.push_back(makeCard(three, club));
            cardDeck.push_back(makeCard(four, club));
            cardDeck.push_back(makeCard(five, club));
            cardDeck.push_back(makeCard(six, club));
            cardDeck.push_back(makeCard(seven, club));
            cardDeck.push_back(makeCard(eight, club));
            cardDeck.push_back(makeCard(nine, club));
            cardDeck.push_back(makeCard(ten, club));
            cardDeck.push_back(makeCard(jack, club));
            cardDeck.push_back(makeCard(queen, club));
            cardDeck.push_back(makeCard(king, club));
            cardDeck.push_back(makeCard(ace, club));
            cardDeck.push_back(makeCard(two, diamond));
            cardDeck.push_back(makeCard(three, diamond));
            cardDeck.push_back(makeCard(four, diamond));
            cardDeck.push_back(makeCard(five, diamond));
            cardDeck.push_back(makeCard(six, diamond));
            cardDeck.push_back(makeCard(seven, diamond));
            cardDeck.push_back(makeCard(eight, diamond));
            cardDeck.push_back(makeCard(nine, diamond));
            cardDeck.push_back(makeCard(ten, diamond));
            cardDeck.push_back(makeCard(jack, diamond));
            cardDeck.push_back(makeCard(queen, diamond));
            cardDeck.push_back(makeCard(king, diamond));
            cardDeck.push_back(makeCard(ace, diamond));
            cardDeck.push_back(makeCard(two, heart));
            cardDeck.push_back(makeCard(three, heart));
            cardDeck.push_back(makeCard(four, heart));
            cardDeck.push_back(makeCard(five, heart));
            cardDeck.push_back(makeCard(six, heart));
            cardDeck.push_back(makeCard(seven, heart));
            cardDeck.push_back(makeCard(eight, heart));
            cardDeck.push_back(makeCard(nine, heart));
            cardDeck.push_back(makeCard(ten, heart));
            cardDeck.push_back(makeCard(jack, heart));
            cardDeck.push_back(makeCard(queen, heart));
            cardDeck.push_back(makeCard(king, heart));
            cardDeck.push_back(makeCard(ace, heart));
            cardDeck.push_back(makeCard(two, spade));
            cardDeck.push_back(makeCard(three, spade));
            cardDeck.push_back(makeCard(four, spade));
            cardDeck.push_back(makeCard(five, spade));
            cardDeck.push_back(makeCard(six, spade));
            cardDeck.push_back(makeCard(seven, spade));
            cardDeck.push_back(makeCard(eight, spade));
            cardDeck.push_back(makeCard(nine, spade));
            cardDeck.push_back(makeCard(ten, spade));
            cardDeck.push_back(makeCard(jack, spade));
            cardDeck.push_back(makeCard(queen, spade));
            cardDeck.push_back(makeCard(king, spade));
            cardDeck.push_back(makeCard(ace, spade));
            cardDeck.push_back(makeCard(two, club));
            cardDeck.push_back(makeCard(three, club));
            cardDeck.push_back(makeCard(four, club));
            cardDeck.push_back(makeCard(five, club));
            cardDeck.push_back(makeCard(six, club));
            cardDeck.push_back(makeCard(seven, club));
            cardDeck.push_back(makeCard(eight, club));
            cardDeck.push_back(makeCard(nine, club));
            cardDeck.push_back(makeCard(ten, club));
            cardDeck.push_back(makeCard(jack, club));
            cardDeck.push_back(makeCard(queen, club));
            cardDeck.push_back(makeCard(king, club));
            cardDeck.push_back(makeCard(ace, club));
            cardDeck.push_back(makeCard(two, diamond));
            cardDeck.push_back(makeCard(three, diamond));
            cardDeck.push_back(makeCard(four, diamond));
            cardDeck.push_back(makeCard(five, diamond));
            cardDeck.push_back(makeCard(six, diamond));
            cardDeck.push_back(makeCard(seven, diamond));
            cardDeck.push_back(makeCard(eight, diamond));
            cardDeck.push_back(makeCard(nine, diamond));
            cardDeck.push_back(makeCard(ten, diamond));
            cardDeck.push_back(makeCard(jack, diamond));
            cardDeck.push_back(makeCard(queen, diamond));
            cardDeck.push_back(makeCard(king, diamond));
            cardDeck.push_back(makeCard(ace, diamond));
            cardDeck.push_back(makeCard(two, heart));
            cardDeck.push_back(makeCard(three, heart));
            cardDeck.push_back(makeCard(four, heart));
            cardDeck.push_back(makeCard(five, heart));
            cardDeck.push_back(makeCard(six, heart));
            cardDeck.push_back(makeCard(seven, heart));
            cardDeck.push_back(makeCard(eight, heart));
            cardDeck.push_back(makeCard(nine, heart));
            cardDeck.push_back(makeCard(ten, heart));
            cardDeck.push_back(makeCard(jack, heart));
            cardDeck.push_back(makeCard(queen, heart));
            cardDeck.push_back(makeCard(king, heart));
            cardDeck.push_back(makeCard(ace, heart));
            cardDeck.push_back(makeCard(two, spade));
            cardDeck.push_back(makeCard(three, spade));
            cardDeck.push_back(makeCard(four, spade));
            cardDeck.push_back(makeCard(five, spade));
            cardDeck.push_back(makeCard(six, spade));
            cardDeck.push_back(makeCard(seven, spade));
            cardDeck.push_back(makeCard(eight, spade));
            cardDeck.push_back(makeCard(nine, spade));
            cardDeck.push_back(makeCard(ten, spade));
            cardDeck.push_back(makeCard(jack, spade));
            cardDeck.push_back(makeCard(queen, spade));
            cardDeck.push_back(makeCard(king, spade));
            cardDeck.push_back(makeCard(ace, spade));   
            break;
        case euchreDeck:
            cardDeck.push_back(makeCard(nine, club));
            cardDeck.push_back(makeCard(ten, club));
            cardDeck.push_back(makeCard(jack, club));
            cardDeck.push_back(makeCard(queen, club));
            cardDeck.push_back(makeCard(king, club));
            cardDeck.push_back(makeCard(ace, club));
            cardDeck.push_back(makeCard(nine, diamond));
            cardDeck.push_back(makeCard(ten, diamond));
            cardDeck.push_back(makeCard(jack, diamond));
            cardDeck.push_back(makeCard(queen, diamond));
            cardDeck.push_back(makeCard(king, diamond));
            cardDeck.push_back(makeCard(ace, diamond));
            cardDeck.push_back(makeCard(nine, heart));
            cardDeck.push_back(makeCard(ten, heart));
            cardDeck.push_back(makeCard(jack, heart));
            cardDeck.push_back(makeCard(queen, heart));
            cardDeck.push_back(makeCard(king, heart));
            cardDeck.push_back(makeCard(ace, heart));
            cardDeck.push_back(makeCard(nine, spade));
            cardDeck.push_back(makeCard(ten, spade));
            cardDeck.push_back(makeCard(jack, spade));
            cardDeck.push_back(makeCard(queen, spade));
            cardDeck.push_back(makeCard(king, spade));
            cardDeck.push_back(makeCard(ace, spade));   
            break;
        default: break;
    }
    // Add jokers to the deck
    for (int i = 0; i < jokerCount; i++) {
        cardDeck.push_back(makeCard(jokerpips, jokersuit)); 
    }

    return cardDeck;
}

void printCard(card c)
{
    // Print the card's pips value
    switch (c.p) {
        case two: cout << "[2_"; break;
        case three: cout << "[3_"; break;
        case four: cout << "[4_"; break;
        case five: cout << "[5_"; break;
        case six: cout << "[6_"; break;
        case seven: cout << "[7_"; break;
        case eight: cout << "[8_"; break;
        case nine: cout << "[9_"; break;
        case ten: cout << "[10"; break;
        case jack: cout << "[J_"; break;
        case queen: cout << "[Q_"; break;
        case king: cout << "[K_"; break;
        case ace: cout << "[A_"; break;
        case jokerpips: cout << "[JKR]"; break;
    }
    // Print the card's suit value
    switch (c.s) {
        case club: cout << "C]"; break;
        case diamond: cout << "D]"; break;
        case heart: cout << "H]"; break;
        case spade: cout << "S]"; break;
    }
    cout << " ";
}

void printDeck(vector<card> deck, int cardsPerRow) {
    for (int i = 0; i < deck.size(); i++) {
        if (i % cardsPerRow == 0) {
            cout << "\n";
        }
        printCard(deck[i]);
    }
    if (deck.size() == 0) {
        cout << "EMPTY DECK" << endl;
    }
    
}

void TESTprintDeck() {
    vector<card> mainDeck;
    int deckSize = 0;

    mainDeck = createDeck(singleDeck, 2);
    cout << endl << "Single Deck Test:" << endl;
    printDeck(mainDeck, 13);
    cout << endl << endl;
    deckSize = mainDeck.size();
    cout << "Size of single deck = " << deckSize << " cards" << endl << endl;
    mainDeck.clear();

    cout << "Double Deck Test:" << endl;
    mainDeck = createDeck(doubleDeck, 4);
    printDeck(mainDeck, 13);
    cout << endl << endl;
    deckSize = mainDeck.size();
    cout << "Size of double deck = " << deckSize << " cards" << endl << endl;
    mainDeck.clear();

    cout << "Euchre Deck Test:" << endl;
    mainDeck = createDeck(euchreDeck, 0);
    printDeck(mainDeck, 6);
    cout << endl << endl;
    deckSize = mainDeck.size();
    cout << "Size of euchre deck = " << deckSize << " cards" << endl << endl;
    mainDeck.clear();

    cout << "Deck printing test is complete!" << endl << endl;
}

vector<card> shuffleDeck(vector<card> deck) {
    //auto rd = random_device {}; 
    //auto rng = default_random_engine { rd() };
    //shuffle(begin(deck), end(deck), rng);
    random_device rd;
    shuffle(deck.begin(), deck.end(), rd);
    return deck;
}

void TESTshuffleDeck() {
    vector<card> mainDeck;

    cout << endl << "Single Deck Shuffle Test 1:" << endl;
    mainDeck = createDeck(singleDeck, 2);
    mainDeck = shuffleDeck(mainDeck);
    printDeck(mainDeck, 13);
    cout << endl << endl;

    cout << "Single Deck Shuffle Test 2:" << endl;
    mainDeck = shuffleDeck(mainDeck);
    printDeck(mainDeck, 13);
    cout << endl << endl;
    mainDeck.clear();

    cout << "Double Deck Shuffle Test 1:" << endl;
    mainDeck = createDeck(doubleDeck, 4);
    mainDeck = shuffleDeck(mainDeck);
    printDeck(mainDeck, 13);
    cout << endl << endl;

    cout << "Double Deck Shuffle Test 2:" << endl;
    mainDeck = shuffleDeck(mainDeck);
    printDeck(mainDeck, 13);
    cout << endl << endl;
    mainDeck.clear();

    cout << "Euchre Deck Shuffle Test 1:" << endl;
    mainDeck = createDeck(euchreDeck, 0);
    mainDeck = shuffleDeck(mainDeck);
    printDeck(mainDeck, 6);
    cout << endl << endl;

    cout << "Euchre Deck Shuffle Test 2:" << endl;
    mainDeck = shuffleDeck(mainDeck);
    printDeck(mainDeck, 6);
    cout << endl << endl;
    mainDeck.clear();

    cout << "Deck shuffle test is complete!" << endl << endl;
}

// Move cards from a certain index range (from index 1 to index 2) of deck 1 to deck 2
array<vector<card>,2> moveCard(vector<card> deck1, vector<card> deck2, int index1, int index2) {
    array<vector<card>,2> cardDecks;

    // Make sure index2 is not less than index1
    if (index2 <= index1) {
        index2 = index1 + 1;
    }

    deck2.insert(deck2.end(), make_move_iterator(deck1.begin() + index1), make_move_iterator(deck1.begin() + index2));
    deck1.erase(deck1.begin() + index1, deck1.begin() + index2);
    
    cardDecks[0] = deck1;
    cardDecks[1] = deck2;
    return cardDecks;
}

void TESTmoveCard() {
    vector<card> mainDeck;
    vector<card> subDeck1;
    vector<card> subDeck2;
    vector<card> subDeck3;

    array<vector<card>,2> cardDecks1;
    array<vector<card>,2> cardDecks2;
    array<vector<card>,2> cardDecks3;

    cout << endl << "Move Card Test - Main Deck" << endl;
    mainDeck = createDeck(singleDeck, 0);
    mainDeck = shuffleDeck(mainDeck);
    printDeck(mainDeck, 13);
    cout << endl << endl;

    cout << "Sub deck 1 (2 cards)" << endl;
    cardDecks1 = moveCard(mainDeck, subDeck1, 0, 2);
    mainDeck = cardDecks1[0];
    subDeck1 = cardDecks1[1];
    printDeck(subDeck1, 13);
    cout << endl << endl;

    cout << "Sub deck 2 (2 cards)" << endl;
    cardDecks2 = moveCard(mainDeck, subDeck2, 0, 2);
    mainDeck = cardDecks2[0];
    subDeck2 = cardDecks2[1];
    printDeck(subDeck2, 13);
    cout << endl << endl;

    cout << "Sub deck 3 (5 cards)" << endl;
    cardDecks3 = moveCard(mainDeck, subDeck3, 0, 5);
    mainDeck = cardDecks3[0];
    subDeck3 = cardDecks3[1];
    printDeck(subDeck3, 13);
    cout << endl << endl;

    cout << "Main deck after creating sub decks" << endl;
    printDeck(mainDeck, 13);
    cout << endl << endl;

    cout << "Move card test is complete!" << endl << endl;
}

