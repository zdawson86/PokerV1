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
                cout << endl << "Player " << i + 1 << " hand:" << endl;
                printDeck(playerHand[i], PLAYER_CARD_COUNT);
            }
            cout << "Leftover cards in main deck:" << endl;
            printDeck(mainDeck, 10);
        }
        cout << "Enter a number to resume" << endl;
        cin >> testvar;
/*
        // Show player 1's cards
        // Print the hand for player1
        printf("\nPlayer 1 hand:\n");
        PrintHand(player1, numberOfCards);
        currentHandType = FindHandType(player1, numberOfCards);
        PrintHandType(currentHandType);
        printf("\n");
        currentStrength = FindHandStrength(player1, currentHandType, numberOfCards);
        printf("Strength (high to low) = %d %d %d %d %d \n\n", currentStrength.p1, currentStrength.p2, currentStrength.p3, currentStrength.p4, currentStrength.p5);

        // Prompt the user for an action (check, bet, or fold)
        getAction(&currentAction);

        // If user folds, game continues and user will start the next hand
        if (currentAction == fold) {
            continue;
        }

        // Print the hand for player2
        printf("\nPlayer 2 hand:\n");
        PrintHand(player2, numberOfCards);
        currentHandType = FindHandType(player2, numberOfCards);
        PrintHandType(currentHandType);
        printf("\n");
        currentStrength = FindHandStrength(player2, currentHandType, numberOfCards);
        printf("Strength (high to low) = %d %d %d %d %d \n\n", currentStrength.p1, currentStrength.p2, currentStrength.p3, currentStrength.p4, currentStrength.p5);

        // Print the hand for player3
        if (numberOfPlayers >= 3) {
            printf("Player 3 hand:\n");
            PrintHand(player3, numberOfCards);
            currentHandType = FindHandType(player3, numberOfCards);
            PrintHandType(currentHandType);
            printf("\n");
            currentStrength = FindHandStrength(player3, currentHandType, numberOfCards);
            printf("Strength (high to low) = %d %d %d %d %d \n\n", currentStrength.p1, currentStrength.p2, currentStrength.p3, currentStrength.p4, currentStrength.p5);
        }
        
        // Print the hand for player4
        if (numberOfPlayers >= 4) {
            printf("Player 4 hand:\n");
            PrintHand(player4, numberOfCards);
            currentHandType = FindHandType(player4, numberOfCards);
            PrintHandType(currentHandType);
            printf("\n");
            currentStrength = FindHandStrength(player4, currentHandType, numberOfCards);
            printf("Strength (high to low) = %d %d %d %d %d \n\n", currentStrength.p1, currentStrength.p2, currentStrength.p3, currentStrength.p4, currentStrength.p5);
        }
        
        // Print the hand for player5
        if (numberOfPlayers >= 5) {
            printf("Player 5 hand:\n");
            PrintHand(player5, numberOfCards);
            currentHandType = FindHandType(player5, numberOfCards);
            PrintHandType(currentHandType);
            printf("\n");
            currentStrength = FindHandStrength(player5, currentHandType, numberOfCards);
            printf("Strength (high to low) = %d %d %d %d %d \n\n", currentStrength.p1, currentStrength.p2, currentStrength.p3, currentStrength.p4, currentStrength.p5);
        }
        
        // Print the hand for player6
        if (numberOfPlayers >= 6) {
            printf("Player 6 hand:\n");
            PrintHand(player6, numberOfCards);
            currentHandType = FindHandType(player6, numberOfCards);
            PrintHandType(currentHandType);
            printf("\n");
            currentStrength = FindHandStrength(player6, currentHandType, numberOfCards);
            printf("Strength (high to low) = %d %d %d %d %d \n\n", currentStrength.p1, currentStrength.p2, currentStrength.p3, currentStrength.p4, currentStrength.p5);
        }

        // Print the hand for player7
        if (numberOfPlayers >= 7) {
            printf("Player 7 hand:\n");
            PrintHand(player7, numberOfCards);
            currentHandType = FindHandType(player7, numberOfCards);
            PrintHandType(currentHandType);
            printf("\n");
            currentStrength = FindHandStrength(player7, currentHandType, numberOfCards);
            printf("Strength (high to low) = %d %d %d %d %d \n\n", currentStrength.p1, currentStrength.p2, currentStrength.p3, currentStrength.p4, currentStrength.p5);
        }

        // Print the hand for player8
        if (numberOfPlayers >= 8) {
            printf("Player 8 hand:\n");
            PrintHand(player8, numberOfCards);
            currentHandType = FindHandType(player8, numberOfCards);
            PrintHandType(currentHandType);
            printf("\n");
            currentStrength = FindHandStrength(player8, currentHandType, numberOfCards);
            printf("Strength (high to low) = %d %d %d %d %d \n\n", currentStrength.p1, currentStrength.p2, currentStrength.p3, currentStrength.p4, currentStrength.p5);
        }

        // Print the hand for player9
        if (numberOfPlayers >= 9) {
            printf("Player 9 hand:\n");
            PrintHand(player9, numberOfCards);
            currentHandType = FindHandType(player9, numberOfCards);
            PrintHandType(currentHandType);
            printf("\n");
            currentStrength = FindHandStrength(player9, currentHandType, numberOfCards);
            printf("Strength (high to low) = %d %d %d %d %d \n\n", currentStrength.p1, currentStrength.p2, currentStrength.p3, currentStrength.p4, currentStrength.p5);
        }

        // Print the hand for player10
        if (numberOfPlayers >= 10) {
            printf("Player 10 hand:\n");
            PrintHand(player10, numberOfCards);
            currentHandType = FindHandType(player10, numberOfCards);
            PrintHandType(currentHandType);
            printf("\n");
            currentStrength = FindHandStrength(player10, currentHandType, numberOfCards);
            printf("Strength (high to low) = %d %d %d %d %d \n\n", currentStrength.p1, currentStrength.p2, currentStrength.p3, currentStrength.p4, currentStrength.p5);
        }

        // Determine who has the best hand out of all of the players
        DetermineWinner(numberOfPlayers, numberOfCards);

        // Print all of the cards in play
        //printf("Cards in play:\n");
        //for (i = 0; i < (numberOfCards * numberOfPlayers); i++) {
        //    PrintCard(cardsInPlay[i]);
        //}
*/
        }  // END OF WHILE(1) LOOP

    return 0;
}