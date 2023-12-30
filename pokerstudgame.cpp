/*
Description:
Stud poker game against computer players.

*/

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#include "poker.hpp"

//const int PLAYER_CARD_COUNT = 5;
const int PLAYER_CHIP_STACK = 1000;
const int VALID = 1;
const int NOT_VALID = 0;
const int NORMAL_MODE = 0;
const int TEST_MODE = 1;
const int PLAYER_OUT_OF_GAME = -1;

// Prompt the user for an action (check, bet, or fold)
action getAction() {
    int a;
    int userInput = NOT_VALID;
    while (userInput == NOT_VALID) {
        cout << "'" << check <<"' to check, ";
        cout << "'" << bet <<"' to bet, ";
        cout << "'" << fold <<"' to fold, ";
        cout << "'Ctrl+C' to exit game" << endl;
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
    int mode = TEST_MODE;

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
    vector<int> winningPlayers;
    int playerCount = 0;
    int maxPlayers = 0;
    int cardCount = 0;
    int chipsToBet = 0;
    int chipPot = 0;
    int round = 0;
    int playersWithChips = 0;
    int userInput = NOT_VALID;
    int testvar = 0;

    cout << endl << "Welcome to Stud Poker!" << endl;

    while (userInput == NOT_VALID) {
        cout << endl << "How many cards (5 or 7)?" << endl;
        cin >> cardCount;
        if ((cardCount == 5) || (cardCount == 7)) {
            userInput = VALID;
        }
        else {
            cout << endl << "NOT A VALID ENTRY!" << endl << endl;
        }
    }
    userInput = NOT_VALID;

    // Set the max number of players based on number of cards per player
    if (cardCount == 5) {
        maxPlayers = 10;
    }
    else {
        maxPlayers = 7;
    }

    while (userInput == NOT_VALID) {
        cout << endl << "How many players (2 to " << maxPlayers << ")?" << endl;
        cin >> playerCount;
        if ((playerCount <= maxPlayers) && (playerCount >= 2)) {
            userInput = VALID;
        }
        else {
            cout << endl << "NOT A VALID ENTRY!" << endl << endl;
        }
    }

    // Set up dummy hands and chip stacks for each player to initialize the game
    for (int i = 0; i < cardCount; i++) {
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
            playerHand[0][2].p = eight, playerHand[0][2].s = diamond;
            playerHand[0][3].p = nine, playerHand[0][3].s = diamond;
            playerHand[0][4].p = nine, playerHand[0][4].s = club;
            playerHand[0][5].p = four, playerHand[0][5].s = club;
            playerHand[0][6].p = two, playerHand[0][6].s = club;
            playerHand[1][0].p = two, playerHand[1][0].s = spade;
            playerHand[1][1].p = three, playerHand[1][1].s = club;
            playerHand[1][2].p = ace, playerHand[1][2].s = spade;
            playerHand[1][3].p = ten, playerHand[1][3].s = heart;
            playerHand[1][4].p = queen, playerHand[1][4].s = spade;
            playerHand[1][5].p = seven, playerHand[1][5].s = spade;
            playerHand[1][6].p = nine, playerHand[1][6].s = spade;
            /*
            playerHand[0][0].p = two, playerHand[0][0].s = club;
            playerHand[0][1].p = three, playerHand[0][1].s = spade;
            playerHand[0][2].p = ace, playerHand[0][2].s = diamond;
            playerHand[0][3].p = ten, playerHand[0][3].s = diamond;
            playerHand[0][4].p = queen, playerHand[0][4].s = club;
            playerHand[1][0].p = two, playerHand[1][0].s = spade;
            playerHand[1][1].p = three, playerHand[1][1].s = club;
            playerHand[1][2].p = ace, playerHand[1][2].s = spade;
            playerHand[1][3].p = ten, playerHand[1][3].s = heart;
            playerHand[1][4].p = queen, playerHand[1][4].s = spade;
            playerHand[2][0].p = two, playerHand[2][0].s = spade;
            playerHand[2][1].p = three, playerHand[2][1].s = club;
            playerHand[2][2].p = ace, playerHand[2][2].s = spade;
            playerHand[2][3].p = ten, playerHand[2][3].s = heart;
            playerHand[2][4].p = queen, playerHand[2][4].s = spade;
            playerHand[3][0].p = two, playerHand[3][0].s = spade;
            playerHand[3][1].p = three, playerHand[3][1].s = club;
            playerHand[3][2].p = king, playerHand[3][2].s = spade;
            playerHand[3][3].p = ten, playerHand[3][3].s = heart;
            playerHand[3][4].p = queen, playerHand[3][4].s = spade;
            */
        }
        else {
            // Create a single deck, shuffle, and deal out cards to all players still in the game
            mainDeck = createDeck(singleDeck, 0);
            mainDeck = shuffleDeck(mainDeck);

            for (int i = 0; i < playerCount; i++) {
                if (playerChips[i] > PLAYER_OUT_OF_GAME) {
                    playerHand[i].erase(playerHand[i].begin(), playerHand[i].end());
                    cardStacks = moveCard(mainDeck, playerHand[i], 0, cardCount);
                    playerHand[i] = cardStacks[1];
                    mainDeck = cardStacks[0];
                }
            }
        }

        // Print out the chip stacks for each player that's still in the game
        for (int i = 0; i < playerHand.size(); i++) {
            if (i == 0) {
                cout << endl << "Your chip stack: " << playerChips[i];
            }
            else if (playerChips[i] > PLAYER_OUT_OF_GAME) {
                cout << endl << "Player " << i+1 << " chip stack: " << playerChips[i];
            }
        }

        // Print the hand for the main player 1
        cout << endl << endl << "Your hand:";
        printDeck(playerHand[0], cardCount);
        cout << "- ";
        currentHandType = findHandType(playerHand[0]);
        currentStrength = findHandStrength(playerHand[0], currentHandType);
        printPokerHand(currentHandType, currentStrength);
        cout << endl;

        // Prompt the user for an action (check, bet, or fold)
        currentAction = getAction();

        // If user folds or checks, game continues and user will start the next hand
        if ((currentAction == fold) || (currentAction == check)) {
            continue;
        }
        if (currentAction == bet) {
            cout << endl << "How many chips to bet?" << endl;
            cin >> chipsToBet;
            // Take away same number of chips from each player no matter what
            // unless they don't have enough
            for (int i = 0; i < playerChips.size(); i++) {
                if (playerChips[i] > PLAYER_OUT_OF_GAME) {
                    if ((playerChips[i] - chipsToBet) <= 0) {
                        chipPot += playerChips[i];
                        playerChips[i] = 0;
                    }
                    else {
                        chipPot += chipsToBet;
                        playerChips[i] -= chipsToBet;
                    }
                }
            }
        }

        // Print the hands for all the players still in the game
        for (int i = 0; i < playerHand.size(); i++) {
            if (playerChips[i] > PLAYER_OUT_OF_GAME) {
                if (i == 0) {
                    cout << endl << "Your hand:";
                }
                else {
                    cout << endl << "Player " << i+1 << " hand:";
                }
                printDeck(playerHand[i], cardCount);
                cout << "- ";
                currentHandType = findHandType(playerHand[i]);
                currentStrength = findHandStrength(playerHand[i], currentHandType);
                printPokerHand(currentHandType, currentStrength);
            }
        }
        
        // Determine who has the best hand out of all of the players
        winningPlayers = determineWinner(playerHand);

        // Pay out the pot to the winning player
        // If there's a tie, split the pot evenly among the tied players
        if (winningPlayers.size() == 0) {
            playerChips[winningPlayers[0]] += chipPot;
        }
        else {
            chipPot /= winningPlayers.size();
            for (int i = 0; i < winningPlayers.size(); i++) {
                playerChips[winningPlayers[i]] += chipPot;
            }
        }
        chipPot = 0;

        // If only 1 player remains, they are the winner
        // then exit the game
        for (int i = 0; i < playerChips.size(); i++) {
            if (playerChips[i] > 0) {
                playersWithChips += 1;
            }
        }
        if (playersWithChips == 1) {
            if (winningPlayers[0] == 0) {
                cout << endl << "You won the game! Congrats!" << endl;
                exit(1);
            }
            else {
                cout << endl << "Player " << winningPlayers[0]+1 << " won the game! Game over." << endl;
                exit(1);
            }
        }

        // If any players have zero chips, they are out of the game
        // If the main player 1 is out of the chips, exit the game
        // Only print a message once when a player initially leaves the game
        // Set the chip count to "-1" to indicate that particular player has been knocked out of the game
        for (int i = 0; i < playerChips.size(); i++) {
            if (playerChips[i] == 0) {
                if (i == 0) {
                    cout << endl << "You ran out of chips! Game over." << endl;
                    exit(1);
                }
                else {
                    cout << endl << "Player " << i+1 << " ran out of chips! They're out of the game." << endl;
                    playerChips[i] = PLAYER_OUT_OF_GAME;
                    if (cardCount == 5) {
                        playerHand[i][0].p = seven, playerHand[i][0].s = club;
                        playerHand[i][1].p = five, playerHand[i][1].s = spade;
                        playerHand[i][2].p = four, playerHand[i][2].s = diamond;
                        playerHand[i][3].p = three, playerHand[i][3].s = heart;
                        playerHand[i][4].p = two, playerHand[i][4].s = spade;
                    }
                    else {
                        playerHand[i][0].p = nine, playerHand[i][0].s = club;
                        playerHand[i][1].p = eight, playerHand[i][1].s = spade;
                        playerHand[i][2].p = seven, playerHand[i][2].s = diamond;
                        playerHand[i][3].p = five, playerHand[i][3].s = heart;
                        playerHand[i][4].p = four, playerHand[i][4].s = spade;
                        playerHand[i][5].p = three, playerHand[i][5].s = club;
                        playerHand[i][6].p = two, playerHand[i][6].s = diamond;
                    }
                }
            }
        }
        
        playersWithChips = 0;
        winningPlayers.clear();

    }  // END OF WHILE(1) LOOP

    return 0;
}