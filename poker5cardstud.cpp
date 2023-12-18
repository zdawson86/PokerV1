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
const int VALID = 1;
const int NOT_VALID = 0;
const int NORMAL_MODE = 0;
const int TEST_MODE = 1;

// Prompt the user for an action (check, bet, or fold)
action getAction() {
    int a;
    int userInput = NOT_VALID;
    while (userInput == NOT_VALID) {
        cout << endl << "What would you like to do?" << endl;
        cout << "Enter '" << check <<"' to check" << endl;
        cout << "Enter '" << bet <<"' to place a bet" << endl;
        cout << "Enter '" << fold <<"' to fold" << endl;
        cin >> a;
        if ((a == check) || (a == bet) || (a == fold)) {
            userInput = VALID;
        }
        else {
            cout << "NOT A VALID ENTRY!" << endl << endl;
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
    int playerCount = 0;
    int userInput = NOT_VALID;
    int testvar = 0;

    while (userInput == NOT_VALID) {
        cout << "\nHow many players?" << endl;
        cout << "Enter a number between '2' and '10'" << endl;
        cin >> playerCount;
        if ((playerCount <= 10) && (playerCount >= 2)) {
            userInput = VALID;
        }
        else {
            cout << "NOT A VALID ENTRY!" << endl << endl;
        }
    }

    // Set up dummy hands for each player to initialize the game
    for (int i = 0; i < PLAYER_CARD_COUNT; i++) {
        dummyHand.push_back(dummyCard);
    }
    for (int i = 0; i < playerCount; i++) {
        playerHand.push_back(dummyHand);
    }

    while(1) {
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

        // Print the hand for the main player 1
        cout << endl << "Your hand:";
        printDeck(playerHand[0], PLAYER_CARD_COUNT);
        cout << "- ";
        currentHandType = findHandType(playerHand[0]);
        //PrintHandType(currentHandType);
        currentStrength = findHandStrength(playerHand[0], currentHandType);
        printPokerHand(currentHandType, currentStrength);
        //cout << "Strength (high to low) = " << currentStrength.p1 << " " << currentStrength.p2 << " " << currentStrength.p3 << " " << currentStrength.p4 << " " << currentStrength.p5 << endl << endl;

        // Prompt the user for an action (check, bet, or fold)
        currentAction = getAction();

        // If user folds, game continues and user will start the next hand
        if (currentAction == fold) {
            continue;
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
            //PrintHandType(currentHandType);
            currentStrength = findHandStrength(playerHand[i], currentHandType);
            printPokerHand(currentHandType, currentStrength);
            //cout << "Strength (high to low) = " << currentStrength.p1 << " " << currentStrength.p2 << " " << currentStrength.p3 << " " << currentStrength.p4 << " " << currentStrength.p5 << endl;
        }
        
        // Determine who has the best hand out of all of the players
        determineWinner(playerHand);

        // Get the hand types and strengths for each player
        // Print all of the cards in play
        //printf("Cards in play:\n");
        //for (i = 0; i < (numberOfCards * numberOfPlayers); i++) {
        //    PrintCard(cardsInPlay[i]);
        //}

    }  // END OF WHILE(1) LOOP

    return 0;
}