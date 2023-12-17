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
    /*
    vector<card> player1Hand;
    vector<card> player2Hand;
    vector<card> player3Hand;
    vector<card> player4Hand;
    vector<card> player5Hand;
    vector<card> player6Hand;
    vector<card> player7Hand;
    vector<card> player8Hand;
    vector<card> player9Hand;
    vector<card> player10Hand;
    */
    array<vector<card>,2> cardStacks;
    vector<handtype> playerHandtype;
    vector<strength> playerStrength;
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

            for (int i = 0; i < PLAYER_CARD_COUNT; i++) {
                dummyHand.push_back(dummyCard);
            }

            for (int i = 0; i < playerCount; i++) {
                playerHand.push_back(dummyHand);
                playerHand[i].erase(playerHand[i].begin(), playerHand[i].end());
                cardStacks = moveCard(mainDeck, playerHand[i], 0, PLAYER_CARD_COUNT);
                playerHand[i] = cardStacks[1];
                mainDeck = cardStacks[0];
                // DEBUG MESSAGES
                cout << endl << "Player " << i + 1 << " hand:" << endl;
                printDeck(playerHand[i], PLAYER_CARD_COUNT);
            }
            // DEBUG MESSAGES
            cout << "Leftover cards in main deck:" << endl;
            printDeck(mainDeck, 10);
        }
        // DEBUG MESSAGES
        //cout << endl << "Enter a number to resume" << endl;
        //cin >> testvar;

        // Print the hand for player1
        cout << endl << "Your hand:" << endl;
        printDeck(playerHand[0], PLAYER_CARD_COUNT);
        currentHandType = findHandType(playerHand[0]);
        PrintHandType(currentHandType);
        cout << endl;
        currentStrength = findHandStrength(playerHand[0], currentHandType);
        cout << "Strength (high to low) = " << currentStrength.p1 << " " << currentStrength.p2 << " " << currentStrength.p3 << " " << currentStrength.p4 << " " << currentStrength.p5 << endl << endl;

        // Prompt the user for an action (check, bet, or fold)
        currentAction = getAction();

        // If user folds, game continues and user will start the next hand
        if (currentAction == fold) {
            continue;
        }

        // Show player 1's cards
        // Print the hand for player1
        printf("\nPlayer 1 hand:\n");
        printDeck(playerHand[0], PLAYER_CARD_COUNT);
        currentHandType = findHandType(playerHand[0]);
        PrintHandType(currentHandType);
        cout << endl;
        currentStrength = findHandStrength(playerHand[0], currentHandType);
        printf("Strength (high to low) = %d %d %d %d %d \n\n", currentStrength.p1, currentStrength.p2, currentStrength.p3, currentStrength.p4, currentStrength.p5);

        // Prompt the user for an action (check, bet, or fold)
        currentAction = getAction();

        // If user folds, game continues and user will start the next hand
        if (currentAction == fold) {
            continue;
        }

        // Print the hand for player2
        printf("\nPlayer 2 hand:\n");
        printDeck(playerHand[1], PLAYER_CARD_COUNT);
        currentHandType = findHandType(playerHand[1]);
        PrintHandType(currentHandType);
        printf("\n");
        currentStrength = findHandStrength(playerHand[1], currentHandType);
        printf("Strength (high to low) = %d %d %d %d %d \n\n", currentStrength.p1, currentStrength.p2, currentStrength.p3, currentStrength.p4, currentStrength.p5);

        // Print the hand for player3
        if (playerHand.size() >= 3) {
            printf("Player 3 hand:\n");
            printDeck(playerHand[2], PLAYER_CARD_COUNT);
            currentHandType = findHandType(playerHand[2]);
            PrintHandType(currentHandType);
            printf("\n");
            currentStrength = findHandStrength(playerHand[2], currentHandType);
            printf("Strength (high to low) = %d %d %d %d %d \n\n", currentStrength.p1, currentStrength.p2, currentStrength.p3, currentStrength.p4, currentStrength.p5);
        }
        
        // Print the hand for player4
        if (playerHand.size() >= 4) {
            printf("Player 4 hand:\n");
            printDeck(playerHand[3], PLAYER_CARD_COUNT);
            currentHandType = findHandType(playerHand[3]);
            PrintHandType(currentHandType);
            printf("\n");
            currentStrength = findHandStrength(playerHand[3], currentHandType);
            printf("Strength (high to low) = %d %d %d %d %d \n\n", currentStrength.p1, currentStrength.p2, currentStrength.p3, currentStrength.p4, currentStrength.p5);
        }
        
        // Print the hand for player5
        if (playerHand.size() >= 5) {
            printf("Player 5 hand:\n");
            printDeck(playerHand[4], PLAYER_CARD_COUNT);
            currentHandType = findHandType(playerHand[4]);
            PrintHandType(currentHandType);
            printf("\n");
            currentStrength = findHandStrength(playerHand[4], currentHandType);
            printf("Strength (high to low) = %d %d %d %d %d \n\n", currentStrength.p1, currentStrength.p2, currentStrength.p3, currentStrength.p4, currentStrength.p5);
        }
        
        // Print the hand for player6
        if (playerHand.size() >= 6) {
            printf("Player 6 hand:\n");
            printDeck(playerHand[5], PLAYER_CARD_COUNT);
            currentHandType = findHandType(playerHand[5]);
            PrintHandType(currentHandType);
            printf("\n");
            currentStrength = findHandStrength(playerHand[5], currentHandType);
            printf("Strength (high to low) = %d %d %d %d %d \n\n", currentStrength.p1, currentStrength.p2, currentStrength.p3, currentStrength.p4, currentStrength.p5);
        }

        // Print the hand for player7
        if (playerHand.size() >= 7) {
            printf("Player 7 hand:\n");
            printDeck(playerHand[6], PLAYER_CARD_COUNT);
            currentHandType = findHandType(playerHand[6]);
            PrintHandType(currentHandType);
            printf("\n");
            currentStrength = findHandStrength(playerHand[6], currentHandType);
            printf("Strength (high to low) = %d %d %d %d %d \n\n", currentStrength.p1, currentStrength.p2, currentStrength.p3, currentStrength.p4, currentStrength.p5);
        }

        // Print the hand for player8
        if (playerHand.size() >= 8) {
            printf("Player 8 hand:\n");
            printDeck(playerHand[7], PLAYER_CARD_COUNT);
            currentHandType = findHandType(playerHand[7]);
            PrintHandType(currentHandType);
            printf("\n");
            currentStrength = findHandStrength(playerHand[7], currentHandType);
            printf("Strength (high to low) = %d %d %d %d %d \n\n", currentStrength.p1, currentStrength.p2, currentStrength.p3, currentStrength.p4, currentStrength.p5);
        }

        // Print the hand for player9
        if (playerHand.size() >= 9) {
            printf("Player 9 hand:\n");
            printDeck(playerHand[8], PLAYER_CARD_COUNT);
            currentHandType = findHandType(playerHand[8]);
            PrintHandType(currentHandType);
            printf("\n");
            currentStrength = findHandStrength(playerHand[8], currentHandType);
            printf("Strength (high to low) = %d %d %d %d %d \n\n", currentStrength.p1, currentStrength.p2, currentStrength.p3, currentStrength.p4, currentStrength.p5);
        }

        // Print the hand for player10
        if (playerHand.size() >= 10) {
            printf("Player 10 hand:\n");
            printDeck(playerHand[9], PLAYER_CARD_COUNT);
            currentHandType = findHandType(playerHand[9]);
            PrintHandType(currentHandType);
            printf("\n");
            currentStrength = findHandStrength(playerHand[9], currentHandType);
            printf("Strength (high to low) = %d %d %d %d %d \n\n", currentStrength.p1, currentStrength.p2, currentStrength.p3, currentStrength.p4, currentStrength.p5);
        }

        // Determine who has the best hand out of all of the players
        cout << "GOT HERE 0";  // DEBUG TEST
        determineWinner(playerHand);

        // Print all of the cards in play
        //printf("Cards in play:\n");
        //for (i = 0; i < (numberOfCards * numberOfPlayers); i++) {
        //    PrintCard(cardsInPlay[i]);
        //}

    }  // END OF WHILE(1) LOOP

    return 0;
}