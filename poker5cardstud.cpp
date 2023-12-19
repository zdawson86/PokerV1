/*
Description:
5 card stud poker game against computer players.

*/

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#include "poker.h"

const int PLAYER_CARD_COUNT = 5;
const int PLAYER_CHIP_STACK = 1000;
const int VALID = 1;
const int NOT_VALID = 0;
const int NORMAL_MODE = 0;
const int TEST_MODE = 1;

// Prompt the user for an action (check, bet, or fold)
action getAction() {
    int a;
    int userInput = NOT_VALID;
    while (userInput == NOT_VALID) {
        cout << "'" << check <<"' to check, ";
        cout << "'" << bet <<"' to bet, ";
        cout << "'" << fold <<"' to fold" << endl;
        cin >> a;
        if ((a == check) || (a == bet) || (a == fold)) {
            userInput = VALID;
        }
        else {
            cout << endl << "NOT A VALID ENTRY!" << endl << endl;
        }
    }

    return static_cast<action>(a);
}

int main() {
    // SET "mode" TO "TEST_MODE" FOR TESTING CUSTOM HANDS (NOT RANDOM)
    // SET "mode" TO "NORMAL_MODE" FOR RUNNING A STANDARD GAME WITH RANDOMLY GENERATED HANDS
    int mode = NORMAL_MODE;

    vector<card> mainDeck;
    vector<vector<card>> playerHand;
    vector<card> dummyHand;
    card dummyCard = {two, club};
    vector<handtype> playerHandtype;
    vector<strength> playerStrength;
    array<vector<card>,2> cardStacks;
    action currentAction = check;
    handtype currentHandType;
    strength currentStrength;
    vector<int> playerChips;
    int playerCount = 0;
    int chipsToBet = 0;
    int chipPot = 0;
    int round = 0;
    int winningPlayer = 0;
    int userInput = NOT_VALID;
    int testvar = 0;

    cout << endl << "Welcome to 5 Card Stud Poker!" << endl;

    while (userInput == NOT_VALID) {
        cout << endl << "How many players (2 to 10)?" << endl;
        cin >> playerCount;
        if ((playerCount <= 10) && (playerCount >= 2)) {
            userInput = VALID;
        }
        else {
            cout << endl << "NOT A VALID ENTRY!" << endl << endl;
        }
    }

    // Set up dummy hands and chip stacks for each player to initialize the game
    for (int i = 0; i < PLAYER_CARD_COUNT; i++) {
        dummyHand.push_back(dummyCard);
    }
    for (int i = 0; i < playerCount; i++) {
        playerHand.push_back(dummyHand);
        playerChips.push_back(PLAYER_CHIP_STACK);
    }

    while(1) {
        round++;
        printNewRoundHeader(round);

        // TEST HANDS
        if (mode == TEST_MODE) {
            playerHand[0][0].p = eight, playerHand[0][0].s = club;
            playerHand[0][1].p = eight, playerHand[0][1].s = spade;
            playerHand[0][2].p = jack, playerHand[0][2].s = diamond;
            playerHand[0][3].p = jack, playerHand[0][3].s = heart;
            playerHand[0][4].p = five, playerHand[0][4].s = spade;
            playerHand[1][0].p = two, playerHand[1][0].s = spade;
            playerHand[1][1].p = three, playerHand[1][1].s = club;
            playerHand[1][2].p = ace, playerHand[1][2].s = spade;
            playerHand[1][3].p = ten, playerHand[1][3].s = heart;
            playerHand[1][4].p = queen, playerHand[1][4].s = spade;
        }
        else {
            // Create a single deck, shuffle, and deal out cards to all players
            mainDeck = createDeck(singleDeck, 0);
            mainDeck = shuffleDeck(mainDeck);

            for (int i = 0; i < playerCount; i++) {
                playerHand[i].erase(playerHand[i].begin(), playerHand[i].end());
                cardStacks = moveCard(mainDeck, playerHand[i], 0, PLAYER_CARD_COUNT);
                playerHand[i] = cardStacks[1];
                mainDeck = cardStacks[0];
            }
        }

        // Print out the chip stacks for each player
        for (int i = 0; i < playerHand.size(); i++) {
            if (i == 0) {
                cout << endl << "Your chip stack: ";
            }
            else {
                cout << endl << "Player " << i+1 << " chip stack: ";
            }
            cout << playerChips[i];
        }

        // Print the hand for the main player 1
        cout << endl << endl << "Your hand:";
        printDeck(playerHand[0], PLAYER_CARD_COUNT);
        cout << "- ";
        currentHandType = findHandType(playerHand[0]);
        currentStrength = findHandStrength(playerHand[0], currentHandType);
        printPokerHand(currentHandType, currentStrength);
        cout << endl;

        // Prompt the user for an action (check, bet, or fold)
        currentAction = getAction();

        // If user folds or checks, game continues and user will start the next hand
        if (currentAction == fold || check) {
            continue;
        }
        if (currentAction == bet) {
            cout << endl << "How many chips to bet?" << endl;
            cin >> chipsToBet;
            // Take away same number of chips from each player no matter what
            for (int i = 0; i < playerChips.size(); i++) {
                playerChips[i] -= chipsToBet;
                chipPot += chipsToBet;
            }
        }

        // Print the hands for all the players
        for (int i = 0; i < playerHand.size(); i++) {
            if (i == 0) {
                cout << endl << "Your hand:";
            }
            else {
                cout << endl << "Player " << i+1 << " hand:";
            }
            printDeck(playerHand[i], PLAYER_CARD_COUNT);
            cout << "- ";
            currentHandType = findHandType(playerHand[i]);
            currentStrength = findHandStrength(playerHand[i], currentHandType);
            printPokerHand(currentHandType, currentStrength);
        }
        
        // Determine who has the best hand out of all of the players
        winningPlayer = determineWinner(playerHand);

        // Pay out the pot to the winning player
        playerChips[winningPlayer] += chipPot;
        chipPot = 0;

    }  // END OF WHILE(1) LOOP

    return 0;
}