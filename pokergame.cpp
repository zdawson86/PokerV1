/*
Description:
Various poker games that can be played against computer opponents.

*/

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

// Define constants
const int NUMBER_OF_SUITS = 4;
const int NUMBER_OF_PIPS = 13;
const int CARD_DECK_SIZE = 52;
const int MAX_CARDS_IN_HAND = 7;
const int MAX_PLAYERS = 10;
const int VALID = 1;
const int NOT_VALID = 0;
const int FOUND = 1;
const int NOT_FOUND = 0;
const int NORMAL_MODE = 0;
const int TEST_MODE = 1;

// Define the pips, suit, card, and handtype data types
typedef enum gametype{noGameType, fiveCardStud, sevenCardStud, texasHoldEm, monteCarlo} gametype;
typedef enum pips{noPips, two = 2, three, four, five, six, seven, eight, nine, ten, jack, queen, king, ace} pips;
typedef enum suit{noSuit, club, diamond, heart, spade} suit;
typedef struct card{pips p; suit s;} card;
typedef struct strength{pips p1; pips p2; pips p3; pips p4; pips p5;} strength;
typedef enum handtype{noHandType, aceHighOrLess, singlePair, twoPair, threeOfKind, stdStraight, stdFlush, fullHouse, fourOfKind, straightFlush, royalFlush} handtype;
typedef enum action{noAction, check, bet, fold} action;

// The "cards in play" array is for keeping track of what's been played so far to prevent duplicates
card cardsInPlay[CARD_DECK_SIZE];

// The card arrays for each player (human player is "player1")
card player1[MAX_CARDS_IN_HAND];
card player2[MAX_CARDS_IN_HAND];
card player3[MAX_CARDS_IN_HAND];
card player4[MAX_CARDS_IN_HAND];
card player5[MAX_CARDS_IN_HAND];
card player6[MAX_CARDS_IN_HAND];
card player7[MAX_CARDS_IN_HAND];
card player8[MAX_CARDS_IN_HAND];
card player9[MAX_CARDS_IN_HAND];
card player10[MAX_CARDS_IN_HAND];

// The handtype and strength arrays for each player is used to determine the winner
handtype playerHandtype[MAX_PLAYERS];
strength playerStrength[MAX_PLAYERS];

// FOR MONTE CARLO SIMULATION - Define the simulation hand card array for calculating probabilities of each hand type
card handSim[MAX_CARDS_IN_HAND];


// Command line propmts for user inputs
// NOTE: fflush(stdout) IS NEEDED BEFORE EVERY scanf LINE AND AFTER ITS ASSOCIATED printf LINES
void setupGame(gametype* game, int* playerCount, int* cardCount, int* handCount) {
    int userInput = NOT_VALID;
    while (userInput == NOT_VALID) {
        printf("\nWhat poker game would you like to play?\n");
        printf("Enter '%d' for 5 Card Stud\n", fiveCardStud);
        printf("Enter '%d' for 7 Card Stud\n", sevenCardStud);
        printf("Enter '%d' for a Monte Carlo Simulation\n", monteCarlo);
        fflush(stdout);
        scanf("%d", game);
        if ((*game == fiveCardStud) || (*game == sevenCardStud) || (*game == monteCarlo)) {
            userInput = VALID;
        }
        else {
            printf("NOT A VALID ENTRY!\n\n");
        }
    }
    userInput = NOT_VALID;
    if ((*game == fiveCardStud) || (*game == sevenCardStud)) {
        while (userInput == NOT_VALID) {
            printf("\nHow many players?\n");
            if (*game == fiveCardStud) {
                printf("Enter a number between '2' and '10'\n");
            }
            else if (*game == sevenCardStud) {
                printf("Enter a number between '2' and '7'\n");
            }
            else {
                printf("HOUSTON, WE HAVE A PROBLEM!\n\n");
            }
            fflush(stdout);
            scanf("%d", playerCount);
            if (((*game == fiveCardStud) && (*playerCount <= 10) && (*playerCount >= 2))
            || ((*game == sevenCardStud) && (*playerCount <= 7) && (*playerCount >= 2))) {
                userInput = VALID;
            }
            else {
                printf("NOT A VALID ENTRY!\n\n");
            }
        }
    }
    else if (*game == monteCarlo) {
        while (userInput == NOT_VALID) {
            printf("\nHow many cards per hand?\n");
            printf("Enter a number between '5' and '7'\n");
            fflush(stdout);
            scanf("%d", cardCount);
            if ((*cardCount <= 7) && (*cardCount >= 5)) {
                userInput = VALID;
            }
            else {
                printf("NOT A VALID ENTRY!\n\n");
            }
        }
        userInput = NOT_VALID;
        while (userInput == NOT_VALID) {
            printf("\nHow many total hands for running the simulation?\n");
            printf("Enter a number between '1' and '10000000'\n");
            fflush(stdout);
            scanf("%d", handCount);
            if ((*handCount <= 10000000) && (*handCount >= 1)) {
                userInput = VALID;
            }
            else {
                printf("NOT A VALID ENTRY!\n\n");
            }
        }
    }
    
    userInput = NOT_VALID;
}

// Prompt the user for an action (check, bet, or fold)
void getAction(action* a) {
    int userInput = NOT_VALID;
    while (userInput == NOT_VALID) {
        printf("\nWhat would you like to do?\n");
        printf("Enter '%d' to check\n", check);
        printf("Enter '%d' to place a bet\n", bet);
        printf("Enter '%d' to fold\n", fold);
        fflush(stdout);
        scanf("%d", a);
        if ((*a == check) || (*a == bet) || (*a == fold)) {
            userInput = VALID;
        }
        else {
            printf("NOT A VALID ENTRY!\n\n");
        }
    }
}

// Swap the values at 2 different addresses
void swap(int* xp, int* yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

// Sort values of an array in ascending order
void selectionSort(int arr[], int n)
{
	int i, j, min_idx;
	// One by one move boundary of unsorted subarray
	for (i = 0; i < n - 1; i++) {
		// Find the minimum element in unsorted array
		min_idx = i;
		for (j = i + 1; j < n; j++)
			if (arr[j] < arr[min_idx])
				min_idx = j;
		// Swap the found minimum element with the first element
		swap(&arr[min_idx], &arr[i]);
	}
}

// Deal a card
card DealCard(int cardCount, int playerCount)
{
    card c;
    int i = 0;
    int duplicateFound = FOUND;
    // Check to see if the card is already included in the dealt cards
    while (duplicateFound == FOUND) {
        duplicateFound = NOT_FOUND;
        // Generate random numbers for the pips and suit values of the card
        // Note that we need to add by the enum offset value, otherwise we'll get ERROR types for the pips and suits
        c.p = static_cast<pips>((rand() % (NUMBER_OF_PIPS)) + 2);
        c.s = static_cast<suit>((rand() % (NUMBER_OF_SUITS)) + 1);
        
        for (i = 0; i < (cardCount * playerCount); i++) {
            if (playerCount == 1) {  // IF ONE PLAYER, ASSUME IT'S A MONTE CARLO SIMULATION
                if ((c.p == handSim[i].p) && (c.s == handSim[i].s)) {
                    duplicateFound = FOUND;
                }
            }
            else if ((c.p == cardsInPlay[i].p) && (c.s == cardsInPlay[i].s)) {
                duplicateFound = FOUND;
            }
        }
    }
    return c;
}

// Assign all of the dealt cards to the players
void AssignCard(int cardCount, int playerCount)
{
    int i = 0, j = 0;
    for (j = 0; j < cardCount; j++) {
        switch (playerCount) {
            case 2:
                player1[j] = cardsInPlay[i];
                player2[j] = cardsInPlay[i+1];
                i += playerCount;
                break;
            case 3:
                player1[j] = cardsInPlay[i];
                player2[j] = cardsInPlay[i+1];
                player3[j] = cardsInPlay[i+2];
                i += playerCount;
                break;
            case 4:
                player1[j] = cardsInPlay[i];
                player2[j] = cardsInPlay[i+1];
                player3[j] = cardsInPlay[i+2];
                player4[j] = cardsInPlay[i+3];
                i += playerCount;
                break;
            case 5:
                player1[j] = cardsInPlay[i];
                player2[j] = cardsInPlay[i+1];
                player3[j] = cardsInPlay[i+2];
                player4[j] = cardsInPlay[i+3];
                player5[j] = cardsInPlay[i+4];
                i += playerCount;
                break;
            case 6:
                player1[j] = cardsInPlay[i];
                player2[j] = cardsInPlay[i+1];
                player3[j] = cardsInPlay[i+2];
                player4[j] = cardsInPlay[i+3];
                player5[j] = cardsInPlay[i+4];
                player6[j] = cardsInPlay[i+5];
                i += playerCount;
                break;
            case 7:
                player1[j] = cardsInPlay[i];
                player2[j] = cardsInPlay[i+1];
                player3[j] = cardsInPlay[i+2];
                player4[j] = cardsInPlay[i+3];
                player5[j] = cardsInPlay[i+4];
                player6[j] = cardsInPlay[i+5];
                player7[j] = cardsInPlay[i+6];
                i += playerCount;
                break;
            case 8:
                player1[j] = cardsInPlay[i];
                player2[j] = cardsInPlay[i+1];
                player3[j] = cardsInPlay[i+2];
                player4[j] = cardsInPlay[i+3];
                player5[j] = cardsInPlay[i+4];
                player6[j] = cardsInPlay[i+5];
                player7[j] = cardsInPlay[i+6];
                player8[j] = cardsInPlay[i+7];
                i += playerCount;
                break;
            case 9:
                player1[j] = cardsInPlay[i];
                player2[j] = cardsInPlay[i+1];
                player3[j] = cardsInPlay[i+2];
                player4[j] = cardsInPlay[i+3];
                player5[j] = cardsInPlay[i+4];
                player6[j] = cardsInPlay[i+5];
                player7[j] = cardsInPlay[i+6];
                player8[j] = cardsInPlay[i+7];
                player9[j] = cardsInPlay[i+8];
                i += playerCount;
                break;
            case 10:
                player1[j] = cardsInPlay[i];
                player2[j] = cardsInPlay[i+1];
                player3[j] = cardsInPlay[i+2];
                player4[j] = cardsInPlay[i+3];
                player5[j] = cardsInPlay[i+4];
                player6[j] = cardsInPlay[i+5];
                player7[j] = cardsInPlay[i+6];
                player8[j] = cardsInPlay[i+7];
                player9[j] = cardsInPlay[i+8];
                player10[j] = cardsInPlay[i+9];
                i += playerCount;
                break;
            default: break;
        }
    }
}

// FOR DEBUGGING - Print a card's values to the screen in a readable format
void PrintCard(card c)
{
    // Print the card's pips value
    switch (c.p) {
        //case ace: printf("ace"); break;
        case two: printf("two"); break;
        case three: printf("three"); break;
        case four: printf("four"); break;
        case five: printf("five"); break;
        case six: printf("six"); break;
        case seven: printf("seven"); break;
        case eight: printf("eight"); break;
        case nine: printf("nine"); break;
        case ten: printf("ten"); break;
        case jack: printf("jack"); break;
        case queen: printf("queen"); break;
        case king: printf("king"); break;
        case ace: printf("ace"); break;
        default: printf("ERROR"); break;
    }
    printf(" of ");
    // Print the card's pips value
    switch (c.s) {
        case club: printf("clubs"); break;
        case diamond: printf("diamonds"); break;
        case heart: printf("hearts"); break;
        case spade: printf("spades"); break;
        default: printf("ERROR"); break;
    }
    printf("\n");
}

// FOR DEBUGGING - Print the cards of a hand in a readable format
void PrintHand(card hand[MAX_CARDS_IN_HAND], int cardCount)
{
    int i = 0;
    for (i = 0; i < cardCount; i++) {
        PrintCard(hand[i]);
    }
    printf("\n");
}

// FOR DEBUGGING - Print the hand type to the screen in a readable format
void PrintHandType(handtype ht)
{
    printf("hand type is ");
    // Print the hand type value
    switch (ht) {
        case royalFlush: printf("royal flush"); break;
        case straightFlush: printf("straight flush"); break;
        case fourOfKind: printf("four of a kind"); break;
        case fullHouse: printf("full house"); break;
        case stdFlush: printf("flush"); break;
        case stdStraight: printf("straight"); break;
        case threeOfKind: printf("three of a kind"); break;
        case twoPair: printf("two pair"); break;
        case singlePair: printf("pair"); break;
        case aceHighOrLess: printf("ace high or less"); break;
        default: printf("ERROR"); break;
    }
    printf("\n");
}

// Determine if the hand contains a single pair
int determineSinglePair(card hand[MAX_CARDS_IN_HAND], int cardCount)
{
    int i = 0, j = 0;
    for (i = 0; i < cardCount; i++) {
        for (j = i+1; j < cardCount; j++) {
            if (hand[i].p == hand[j].p) {
                return FOUND;  // Found a single pair in the hand
            }   
        }    
    }
    return NOT_FOUND;  // Did not find any pairs in the hand
}

// Determine if the hand contains two pairs
int determineTwoPair(card hand[MAX_CARDS_IN_HAND], int cardCount)
{
    int i = 0, j = 0, pairTracker = 0;
    for (i = 0; i < cardCount; i++) {
        for (j = i+1; j < cardCount; j++) {
            if ((hand[i].p == hand[j].p) && pairTracker == 1) {
                return FOUND;  // Found two pairs in the hand
            }
            else if (hand[i].p == hand[j].p) {
                pairTracker = 1;
            }
        }    
    }
    return NOT_FOUND;  // Did not find two pairs in the hand
}

// Determine if the hand contains three of a kind
int determineThreeOfKind(card hand[MAX_CARDS_IN_HAND], int cardCount)
{
    int i = 0, j = 0, threeOfKindTracker = 0;
    for (i = 0; i < cardCount; i++) {
        threeOfKindTracker = 0;
        for (j = i+1; j < cardCount; j++) {
            if ((hand[i].p == hand[j].p) && threeOfKindTracker == 1) {
                return FOUND;
            }
            else if (hand[i].p == hand[j].p) {
                threeOfKindTracker = 1;
            }
        }    
    }
    return NOT_FOUND;
}

// Determine if the hand contains a straight
int determineStraight(card hand[MAX_CARDS_IN_HAND], int cardCount)
{
    int i = 0, j = 0;
    int straightUpPossible = NOT_FOUND, straightDownPossible = NOT_FOUND;
    int straightUpPips[4], straightDownPips[4];
    for (i = 0; i < cardCount; i++) {
        straightUpPossible = FOUND;
        straightDownPossible = FOUND;
        straightUpPips[0] = hand[i].p + 1;
        straightUpPips[1] = hand[i].p + 2;
        straightUpPips[2] = hand[i].p + 3;
        straightUpPips[3] = hand[i].p + 4;
        if (straightUpPips[3] > ace) {
            straightUpPossible = NOT_FOUND;
        }
        int straightUpPipsFound[4] = {NOT_FOUND, NOT_FOUND, NOT_FOUND, NOT_FOUND};
        straightDownPips[0] = hand[i].p - 1;
        straightDownPips[1] = hand[i].p - 2;
        straightDownPips[2] = hand[i].p - 3;
        straightDownPips[3] = hand[i].p - 4;
        if (straightDownPips[3] == (two - 1)) {
            straightDownPips[3] = ace;  // Ace is low for a downward straight
        }
        else if (straightDownPips[3] < (two - 1)) {
            straightDownPossible = NOT_FOUND;
        }
        int straightDownPipsFound[4] = {NOT_FOUND, NOT_FOUND, NOT_FOUND, NOT_FOUND};
        for (j = 0; j < cardCount; j++) {
            if (straightUpPossible == FOUND) {
                if (hand[j].p == straightUpPips[0]) {
                    straightUpPipsFound[0] = FOUND;
                }
                else if (hand[j].p == straightUpPips[1]) {
                    straightUpPipsFound[1] = FOUND;
                }
                else if (hand[j].p == straightUpPips[2]) {
                    straightUpPipsFound[2] = FOUND;
                }
                else if (hand[j].p == straightUpPips[3]) {
                    straightUpPipsFound[3] = FOUND;
                }
            }
            if (straightDownPossible == 1) {
                if (hand[j].p == straightDownPips[0]) {
                    straightDownPipsFound[0] = FOUND;
                }
                else if (hand[j].p == straightDownPips[1]) {
                    straightDownPipsFound[1] = FOUND;
                }
                else if (hand[j].p == straightDownPips[2]) {
                    straightDownPipsFound[2] = FOUND;
                }
                else if (hand[j].p == straightDownPips[3]) {
                    straightDownPipsFound[3] = FOUND;
                }
            }
        }
        if (((straightDownPipsFound[0] == FOUND)
            && (straightDownPipsFound[1] == FOUND)
            && (straightDownPipsFound[2] == FOUND)
            && (straightDownPipsFound[3] == FOUND))
            || ((straightUpPipsFound[0] == FOUND)
            && (straightUpPipsFound[1] == FOUND)
            && (straightUpPipsFound[2] == FOUND)
            && (straightUpPipsFound[3] == FOUND))) {
            return FOUND;
        }
    }
    return NOT_FOUND;
}

// Determine if the hand contains a flush
int determineFlush(card hand[MAX_CARDS_IN_HAND], int cardCount)
{
    int i = 0, j = 0, flushTracker = 0;
    for (i = 0; i < cardCount; i++) {
        flushTracker = 0;
        for (j = i+1; j < cardCount; j++) {
            if ((hand[i].s == hand[j].s) && flushTracker == 3) {
                return FOUND;
            }
            else if ((hand[i].s == hand[j].s) && flushTracker == 2) {
                flushTracker = 3;
            }
            else if ((hand[i].s == hand[j].s) && flushTracker == 1) {
                flushTracker = 2;
            }
            else if ((hand[i].s == hand[j].s) && flushTracker == 0) {
                flushTracker = 1;
            }
        }    
    }
    return NOT_FOUND;
}

// Determine if the hand contains a full house
int determineFullHouse(card hand[MAX_CARDS_IN_HAND], int cardCount)
{
    int i = 0, j = 0, threeOfKindTracker = 0, threeOfKindPips = 0;
    for (i = 0; i < cardCount; i++) {
        threeOfKindTracker = 0;
        for (j = i+1; j < cardCount; j++) {
            if ((hand[i].p == hand[j].p) && threeOfKindTracker == 1) {
                threeOfKindTracker = 2;
                // Keep track of the three of a kind pips value so that we don't use it when looking for a pair
                threeOfKindPips = hand[i].p;
            }
            else if (hand[i].p == hand[j].p) {
                threeOfKindTracker = 1;
            }
        }
        if (threeOfKindTracker == 2) {
            break;
        }
    }
    // Only check for a pair if a three of a kind was found
    if (threeOfKindTracker == 2) {
        for (i = 0; i < cardCount; i++) {
            // Only check for pairs with a different pips value than the three of a kind pips
            if (!(hand[i].p == threeOfKindPips)) {
                for (j = i+1; j < cardCount; j++) {
                    if ((hand[i].p == hand[j].p)) {
                        return FOUND;
                    }
                }
            }
        }
    }
    return NOT_FOUND;
}

// Determine if the hand contains four of a kind
int determineFourOfKind(card hand[MAX_CARDS_IN_HAND], int cardCount)
{
    int i = 0, j = 0, fourOfKindTracker = 0;
    for (i = 0; i < cardCount; i++) {
        fourOfKindTracker = 0;
        for (j = i+1; j < cardCount; j++) {
            if ((hand[i].p == hand[j].p) && fourOfKindTracker == 2) {
                return FOUND;
            }
            else if ((hand[i].p == hand[j].p) && fourOfKindTracker == 1) {
                fourOfKindTracker = 2;
            }
            else if (hand[i].p == hand[j].p) {
                fourOfKindTracker = 1;
            }
        }    
    }
    return NOT_FOUND;
}
    
// Determine if the hand contains a straight flush
int determineStraightFlush(card hand[MAX_CARDS_IN_HAND], int cardCount)
{
    int straightResult = 0, flushResult = 0;
    straightResult = determineStraight(hand, cardCount);
    if (straightResult == FOUND) {
        flushResult = determineFlush(hand, cardCount);
        if (flushResult == FOUND) {
            return FOUND;
        }
    }
    return NOT_FOUND;
}

// Determine if the hand contains a royal flush
int determineRoyalFlush(card hand[MAX_CARDS_IN_HAND], int cardCount)
{
    int i = 0, result = NOT_FOUND, foundTen = NOT_FOUND, foundAce = NOT_FOUND;
    result = determineStraightFlush(hand, cardCount);
    if (result == FOUND) {
        for (i = 0; i < cardCount; i++) {
            if (hand[i].p == ten) {
                foundTen = FOUND;
            }
            if (hand[i].p == ace) {
                foundAce = FOUND;
            }
            if ((foundTen == FOUND) && (foundAce == FOUND)) {
                return FOUND;
            }
        }
    }
    return NOT_FOUND;
}

// Determine the type of hand that was dealt
handtype FindHandType(card hand[MAX_CARDS_IN_HAND], int cardCount)
{
    int result = NOT_FOUND;

    // Determine if the hand contains a royal flush
    result = determineRoyalFlush(hand, cardCount);
    if (result == FOUND) {
        return royalFlush;
    }

    // Determine if the hand contains a straight flush
    result = determineStraightFlush(hand, cardCount);
    if (result == FOUND) {
        return straightFlush;
    }

    // Determine if the hand contains four of a kind
    result = determineFourOfKind(hand, cardCount);
    if (result == FOUND) {
        return fourOfKind;
    }

    // Determine if the hand contains a full house
    result = determineFullHouse(hand, cardCount);
    if (result == FOUND) {
        return fullHouse;
    }

    // Determine if the hand contains a flush
    result = determineFlush(hand, cardCount);
    if (result == FOUND) {
        return stdFlush;
    }

    // Determine if the hand contains a straight
    result = determineStraight(hand, cardCount);
    if (result == FOUND) {
        return stdStraight;
    }

    // Determine if the hand contains three of a kind
    result = determineThreeOfKind(hand, cardCount);
    if (result == FOUND) {
        return threeOfKind;
    }

    // Determine if the hand contains two pairs
    result = determineTwoPair(hand, cardCount);
    if (result == FOUND) {
        return twoPair;
    }

    // Determine if the hand contains a single pair
    result = determineSinglePair(hand, cardCount);
    if (result == FOUND) {
        return singlePair;
    }
    
    // At this point, assume the hand is an ace high or less
    return aceHighOrLess;

}

// Find the strength of a hand with ace high or less
strength strengthAceHighOrLess(card hand[MAX_CARDS_IN_HAND], int cardCount)
{
    int i = 0, pipsArray[MAX_CARDS_IN_HAND]; 
    strength cardStrength = {noPips, noPips, noPips, noPips, noPips};
    // Reset the pips array to all zeros
    for (i = 0; i < cardCount; i++) {
        pipsArray[i] = 0;
    }
    // Get pips values from the hand
    for (i = 0; i < cardCount; i++) {
        pipsArray[i] = hand[i].p;
    }
    // Sort the hand's pips values into the card strength struct (p1 strongest, p5 weakest)
    selectionSort(pipsArray, cardCount);
    cardStrength.p1 = static_cast<pips>(pipsArray[cardCount-1]);
    cardStrength.p2 = static_cast<pips>(pipsArray[cardCount-2]);
    cardStrength.p3 = static_cast<pips>(pipsArray[cardCount-3]);
    cardStrength.p4 = static_cast<pips>(pipsArray[cardCount-4]);
    cardStrength.p5 = static_cast<pips>(pipsArray[cardCount-5]);

    return cardStrength;
}

// Find the strength of a hand with a single pair
strength strengthSinglePair(card hand[MAX_CARDS_IN_HAND], int cardCount)
{
    int i = 0, j = 0, pairPips = 0, pipsArray[MAX_CARDS_IN_HAND];
    strength cardStrength = {noPips, noPips, noPips, noPips, noPips};
    // Reset the pips array to all zeros
    for (i = 0; i < cardCount; i++) {
        pipsArray[i] = 0;
    }
    // Get pips values from the hand
    for (i = 0; i < cardCount; i++) {
        pipsArray[i] = hand[i].p;
    }
    // Find the pair in the hand
    for (i = 0; i < cardCount; i++) {
        for (j = i+1; j < cardCount; j++) {
            if (hand[i].p == hand[j].p) {
                pairPips = hand[i].p;
                break;
            }
        }
    }
    // Remove the pair pips from the pips array
    for (i = 0; i < cardCount; i++) {
        pipsArray[i] = 0;
    }
    for (i = 0; i < cardCount; i++) {
        if (!(hand[i].p == pairPips)) {
            pipsArray[i] = hand[i].p;
        }   
    }
    // Sort the remaining pips values for the card strength struct
    selectionSort(pipsArray, cardCount);
    cardStrength.p1 = static_cast<pips>(pairPips);  // pips value for the pair
    cardStrength.p2 = static_cast<pips>(pipsArray[cardCount-1]);  // highest single pips
    cardStrength.p3 = static_cast<pips>(pipsArray[cardCount-2]);  // second highest single pips
    cardStrength.p4 = static_cast<pips>(pipsArray[cardCount-3]);  // lowest single pips
    cardStrength.p5 = noPips;  // not used

    return cardStrength;
}

// Find the strength of a hand with two pairs
strength strengthTwoPair(card hand[MAX_CARDS_IN_HAND], int cardCount)
{
    int i = 0, j = 0, temp = 0;
    int pairPipsHigh = 0, pairPipsLow = 0, thirdPairPips = 0, singlePips = 0, firstPairFound = NOT_FOUND;
    int pipsArray[MAX_CARDS_IN_HAND];
    strength cardStrength = {noPips, noPips, noPips, noPips, noPips};
    // Reset the pips array to all zeros
    for (i = 0; i < cardCount; i++) {
        pipsArray[i] = 0;
    }
    // Get pips values from the hand
    for (i = 0; i < cardCount; i++) {
        pipsArray[i] = hand[i].p;
    }
    // Find the 2 pairs in the hand
    for (i = 0; i < cardCount; i++) {
        for (j = i+1; j < cardCount; j++) {
            if ((hand[i].p == hand[j].p) && (firstPairFound == FOUND)) {
                pairPipsLow = hand[i].p;
                break;
            }
            else if (hand[i].p == hand[j].p) {
                pairPipsHigh = hand[i].p;
                firstPairFound = FOUND;
            }
        }
    }
    // Check to see if there's a third pair (possible for 7 card hands)
    for (i = 0; i < cardCount; i++) {
        for (j = i+1; j < cardCount; j++) {
            if ((hand[i].p == hand[j].p) && (!(hand[i].p == pairPipsHigh)) && (!(hand[i].p == pairPipsLow))) {
                thirdPairPips = hand[i].p;
                break;
            }
        }
    }
    // If there are three pairs, check if the third pair is bigger than either of the other two pairs
    if (thirdPairPips > pairPipsHigh) {
        pairPipsHigh = thirdPairPips;
    }
    else if (thirdPairPips > pairPipsLow) {
        pairPipsLow = thirdPairPips;
    }
    // Find the highest single card pips by sorting the remaining cards
    for (i = 0; i < cardCount; i++) {
        pipsArray[i] = 0;
    }
    for (i = 0; i < cardCount; i++) {
        if ((!(hand[i].p == pairPipsHigh)) && (!(hand[i].p == pairPipsLow))) {
            pipsArray[i] = hand[i].p;
        }   
    }
    selectionSort(pipsArray, cardCount);
    // Check if the "high" pips value is actually higher than the "low" pips value
    // If not, swap them
    if (pairPipsHigh < pairPipsLow) {
        swap(&pairPipsHigh, &pairPipsLow);
    }
    // Assign the pips values to the card strength struct
    cardStrength.p1 = static_cast<pips>(pairPipsHigh);  // high pair pips
    cardStrength.p2 = static_cast<pips>(pairPipsLow);  // low pair pips
    cardStrength.p3 = static_cast<pips>(pipsArray[cardCount-1]);  // single pips
    cardStrength.p4 = noPips;  // lowest single pips
    cardStrength.p5 = noPips;  // not used
    
    return cardStrength;
}

// Find the strength of a hand with three of a kind
strength strengthThreeOfKind(card hand[MAX_CARDS_IN_HAND], int cardCount)
{
    int i = 0, j = 0, threeOfKindTracker = 0, threeOfKindPips = 0, pipsArray[MAX_CARDS_IN_HAND];
    strength cardStrength = {noPips, noPips, noPips, noPips, noPips};
    // Reset the pips array to all zeros
    for (i = 0; i < cardCount; i++) {
        pipsArray[i] = 0;
    }
    // Get pips values from the hand
    for (i = 0; i < cardCount; i++) {
        pipsArray[i] = hand[i].p;
    }
    // Find the three of a kind pips value in the hand
    for (i = 0; i < cardCount; i++) {
        threeOfKindTracker = 0;
        for (j = i+1; j < cardCount; j++) {
            if ((hand[i].p == hand[j].p) && threeOfKindTracker == 1) {
                threeOfKindPips = hand[i].p;
            }
            else if (hand[i].p == hand[j].p) {
                threeOfKindTracker = 1;
            }
        }
    }
    // Remove the three of a kind pips from the pips array
    for (i = 0; i < cardCount; i++) {
        pipsArray[i] = 0;
    }
    for (i = 0; i < cardCount; i++) {
        if (!(hand[i].p == threeOfKindPips)) {
            pipsArray[i] = hand[i].p;
        }   
    }
    // Sort the remaining pips values for the card strength struct
    selectionSort(pipsArray, cardCount);
    cardStrength.p1 = static_cast<pips>(threeOfKindPips);  // pips value for the three of a kind
    cardStrength.p2 = static_cast<pips>(pipsArray[cardCount-1]);  // highest single pips
    cardStrength.p3 = static_cast<pips>(pipsArray[cardCount-2]);  // lowest single pips
    cardStrength.p4 = noPips;  // not used
    cardStrength.p5 = noPips;  // not used
    
    return cardStrength;
}

// Find the strength of a hand with a straight
strength strengthStraight(card hand[MAX_CARDS_IN_HAND], int cardCount)
{
    int i = 0, pipsArray[MAX_CARDS_IN_HAND], fiveFound = NOT_FOUND, aceFound = NOT_FOUND, straightPipsHigh = 0; 
    strength cardStrength = {noPips, noPips, noPips, noPips, noPips};
    // Reset the pips array to all zeros
    for (i = 0; i < cardCount; i++) {
        pipsArray[i] = 0;
    }
    // Get pips values from the hand
    for (i = 0; i < cardCount; i++) {
        pipsArray[i] = hand[i].p;
        // If both a five and ace are found, convert the ace pips value to be low instead of high
        if (pipsArray[i] == five) {
            fiveFound = FOUND;
        }
        else if (pipsArray[i] == ace) {
            aceFound = FOUND;
        }
        if ((fiveFound == FOUND) && (aceFound == FOUND)) {
            for (i = 0; i < cardCount; i++) {
                if (pipsArray[i] == ace) {
                    pipsArray[i] = two - 1;  // ace is low for this particular straight
                    break;
                }
            }
        }
    }
    // Sort the pips values and determine if either of the top two pips values are part of the straight (applicable for 7 card hands)
    // Should only need to check the next two pips values directly below the pips value being checked to verify the straight
    selectionSort(pipsArray, cardCount);
    if ((pipsArray[cardCount-2] == (pipsArray[cardCount-1]-1))
    && (pipsArray[cardCount-3] == (pipsArray[cardCount-1]-2))) {
        straightPipsHigh = pipsArray[cardCount-1];
    }
    else if ((pipsArray[cardCount-3] == (pipsArray[cardCount-2]-1))
    && (pipsArray[cardCount-4] == (pipsArray[cardCount-2]-2))) {
        straightPipsHigh = pipsArray[cardCount-2];
    }
    else {
        straightPipsHigh = pipsArray[cardCount-3];
    }
    // Assign the hand's pips values into the card strength struct (p1 strongest, p5 weakest)
    cardStrength.p1 = static_cast<pips>(straightPipsHigh);  // highest pips value of the straight
    cardStrength.p2 = noPips;  // not used
    cardStrength.p3 = noPips;  // not used
    cardStrength.p4 = noPips;  // not used
    cardStrength.p5 = noPips;  // not used
    
    return cardStrength;
}

// Find the strength of a hand with a flush
strength strengthFlush(card hand[MAX_CARDS_IN_HAND], int cardCount)
{
    int i = 0, j = 0, pipsArray[MAX_CARDS_IN_HAND], flushTracker = 0, flushSuit = 0;
    strength cardStrength = {noPips, noPips, noPips, noPips, noPips};
    // Find the flush suit (5 cards of same suit)
    for (i = 0; i < cardCount; i++) {
        flushTracker = 0;
        for (j = i+1; j < cardCount; j++) {
            if ((hand[i].s == hand[j].s) && flushTracker == 3) {
                flushSuit = hand[i].s;
            }
            else if ((hand[i].s == hand[j].s) && flushTracker == 2) {
                flushTracker = 3;
            }
            else if ((hand[i].s == hand[j].s) && flushTracker == 1) {
                flushTracker = 2;
            }
            else if ((hand[i].s == hand[j].s) && flushTracker == 0) {
                flushTracker = 1;
            }
        }    
    }
    // Reset the pips array to all zeros
    for (i = 0; i < cardCount; i++) {
        pipsArray[i] = 0;
    }
    // Get pips values from the hand for only the flush suit
    for (i = 0; i < cardCount; i++) {
        if (hand[i].s == flushSuit) {
            pipsArray[i] = hand[i].p;
        }
    }
    // Sort the hand's pips values into the card strength struct (p1 strongest, p5 weakest)
    selectionSort(pipsArray, cardCount);
    cardStrength.p1 = static_cast<pips>(pipsArray[cardCount-1]);
    cardStrength.p2 = static_cast<pips>(pipsArray[cardCount-2]);
    cardStrength.p3 = static_cast<pips>(pipsArray[cardCount-3]);
    cardStrength.p4 = static_cast<pips>(pipsArray[cardCount-4]);
    cardStrength.p5 = static_cast<pips>(pipsArray[cardCount-5]);
    
    return cardStrength;
}

// Find the strength of a hand with a full house
strength strengthFullHouse(card hand[MAX_CARDS_IN_HAND], int cardCount)
{
    int i = 0, j = 0, threeOfKindTracker = 0, threeOfKindPips = 0, threeOfKindPips2 = 0, pairPips = 0, pairPips2 = 0;
    strength cardStrength = {noPips, noPips, noPips, noPips, noPips};
    // Find the three of a kind pips value
    for (i = 0; i < cardCount; i++) {
        threeOfKindTracker = 0;
        for (j = i+1; j < cardCount; j++) {
            if ((hand[i].p == hand[j].p) && threeOfKindTracker == 1) {
                threeOfKindTracker = 2;
                threeOfKindPips = hand[i].p;
            }
            else if (hand[i].p == hand[j].p) {
                threeOfKindTracker = 1;
            }
        }
        if (threeOfKindTracker == 2) {
            break;
        }
    }
    // Check to see if there's a second three of a kind (possible for 7 card hands)
    for (i = 0; i < cardCount; i++) {
        threeOfKindTracker = 0;
        for (j = i+1; j < cardCount; j++) {
            if ((hand[i].p == hand[j].p) && (threeOfKindTracker == 1) && (!(hand[i].p == threeOfKindPips))) {
                threeOfKindPips2 = hand[i].p;
            }
            else if ((hand[i].p == hand[j].p) && (!(hand[i].p == threeOfKindPips))) {
                threeOfKindTracker = 1;
            }
        }
    }
    // If there's two sets of "three of a kind", find the bigger pips value between the two sets
    if (threeOfKindPips2 > threeOfKindPips) {
        threeOfKindPips = threeOfKindPips2;
    }
    // Find the pair pips value
    for (i = 0; i < cardCount; i++) {
        if (!(hand[i].p == threeOfKindPips)) {
            pairPips = hand[i].p;
        }
    }
    // Check to see if there's a second pair (possible for 7 card hands)
    for (i = 0; i < cardCount; i++) {
        if ((!(hand[i].p == threeOfKindPips)) && (!(hand[i].p == pairPips))) {
            pairPips2 = hand[i].p;
        }
    }
    // If the second pair is bigger than the first pair, replace the first pair with the second pair's pips value
    if (pairPips2 > pairPips) {
        pairPips = pairPips2;
    }
    // Assign pips values for the card strength struct
    cardStrength.p1 = static_cast<pips>(threeOfKindPips);  // pips value for the three of a kind
    cardStrength.p2 = static_cast<pips>(pairPips);  // pips value for the pair
    cardStrength.p3 = noPips;  // not used
    cardStrength.p4 = noPips;  // not used
    cardStrength.p5 = noPips;  // not used

    return cardStrength;
}

// Find the strength of a hand with four of a kind
strength strengthFourOfKind(card hand[MAX_CARDS_IN_HAND], int cardCount)
{
    int i = 0, j = 0, fourOfKindTracker = 0, fourOfKindPips = 0, singlePips = 0, pipsArray[MAX_CARDS_IN_HAND];
    strength cardStrength = {noPips, noPips, noPips, noPips, noPips};
    // Reset the pips array to all zeros
    for (i = 0; i < cardCount; i++) {
        pipsArray[i] = 0;
    }
    // Get pips values from the hand
    for (i = 0; i < cardCount; i++) {
        pipsArray[i] = hand[i].p;
    }
    // Find the four of a kind pips value in the hand
    for (i = 0; i < cardCount; i++) {
        fourOfKindTracker = 0;
        for (j = i+1; j < cardCount; j++) {
            if ((hand[i].p == hand[j].p) && fourOfKindTracker == 2) {
                fourOfKindPips = hand[i].p;
            }
            else if ((hand[i].p == hand[j].p) && fourOfKindTracker == 1) {
                fourOfKindTracker = 2;
            }
            else if (hand[i].p == hand[j].p) {
                fourOfKindTracker = 1;
            }
        }    
    }
    // Find the highest single pips value by sorting the remaining pips
    for (i = 0; i < cardCount; i++) {
        if (!(hand[i].p == fourOfKindPips)) {
            pipsArray[i] = hand[i].p;
        }   
    }
    selectionSort(pipsArray, cardCount);
    // Assign pips values for the card strength struct
    cardStrength.p1 = static_cast<pips>(fourOfKindPips);  // pips value for the four of a kind
    cardStrength.p2 = static_cast<pips>(pipsArray[cardCount-1]);  // highest single pips
    cardStrength.p3 = noPips;  // not used
    cardStrength.p4 = noPips;  // not used
    cardStrength.p5 = noPips;  // not used
    
    return cardStrength;
}

// Find the strength of a hand with a straight flush
strength strengthStraightFlush(card hand[MAX_CARDS_IN_HAND], int cardCount)
{
    int i = 0, j = 0, flushTracker = 0, flushSuit, pipsArray[MAX_CARDS_IN_HAND], straightPipsHigh = 0;
    strength cardStrength = {noPips, noPips, noPips, noPips, noPips};
    // Find the flush suit (5 cards of same suit)
    for (i = 0; i < cardCount; i++) {
        flushTracker = 0;
        for (j = i+1; j < cardCount; j++) {
            if ((hand[i].s == hand[j].s) && flushTracker == 3) {
                flushSuit = hand[i].s;
            }
            else if ((hand[i].s == hand[j].s) && flushTracker == 2) {
                flushTracker = 3;
            }
            else if ((hand[i].s == hand[j].s) && flushTracker == 1) {
                flushTracker = 2;
            }
            else if ((hand[i].s == hand[j].s) && flushTracker == 0) {
                flushTracker = 1;
            }
        }    
    }
    // Reset the pips array to all zeros
    for (i = 0; i < cardCount; i++) {
        pipsArray[i] = 0;
    }
    // Get pips values from the hand for only the flush suit
    for (i = 0; i < cardCount; i++) {
        if (hand[i].s == flushSuit) {
            pipsArray[i] = hand[i].p;
            // If an ace is found, it has to be converted to be low instead of high (high would be royal flush)
            if (pipsArray[i] == ace) {
                pipsArray[i] = two - 1;
            }
        }
    }
    // Sort the pips values and determine if either of the top two pips values are part of the straight (applicable for 7 card hands)
    // Should only need to check the next two pips values directly below the pips value being checked to verify the straight
    selectionSort(pipsArray, cardCount);
    if ((pipsArray[cardCount-2] == (pipsArray[cardCount-1]-1))
    && (pipsArray[cardCount-3] == (pipsArray[cardCount-1]-2))) {
        straightPipsHigh = pipsArray[cardCount-1];
    }
    else if ((pipsArray[cardCount-3] == (pipsArray[cardCount-2]-1))
    && (pipsArray[cardCount-4] == (pipsArray[cardCount-2]-2))) {
        straightPipsHigh = pipsArray[cardCount-2];
    }
    else {
        straightPipsHigh = pipsArray[cardCount-3];
    }
    // Assign the hand's pips values into the card strength struct (p1 strongest, p5 weakest)
    selectionSort(pipsArray, cardCount);
    cardStrength.p1 = static_cast<pips>(straightPipsHigh);  // highest pips value of the straight flush
    cardStrength.p2 = noPips;  // not used
    cardStrength.p3 = noPips;  // not used
    cardStrength.p4 = noPips;  // not used
    cardStrength.p5 = noPips;  // not used
    
    return cardStrength;
}

// Find the strength of the hand that was dealt based on the hand type
strength FindHandStrength(card hand[MAX_CARDS_IN_HAND], handtype ht, int cardCount)
{
    strength result;
    strength cardStrength = {noPips, noPips, noPips, noPips, noPips};
    switch (ht) {
        case aceHighOrLess: result = strengthAceHighOrLess(hand, cardCount); break;
        case singlePair: result = strengthSinglePair(hand, cardCount); break;
        case twoPair: result = strengthTwoPair(hand, cardCount); break;
        case threeOfKind: result = strengthThreeOfKind(hand, cardCount); break;
        case stdStraight: result = strengthStraight(hand, cardCount); break;
        case stdFlush: result = strengthFlush(hand, cardCount); break;
        case fullHouse: result = strengthFullHouse(hand, cardCount); break;
        case fourOfKind: result = strengthFourOfKind(hand, cardCount); break;
        case straightFlush: result = strengthStraightFlush(hand, cardCount); break;
        default: break;
    }

    return result;
}

// Determine who has the best hand out of all of the players
void DetermineWinner(int playerCount, int cardCount)
{
    int i = 0, j = 0, bestHandCount = 0;
    handtype bestHandtype = noHandType;
    int playersWithBestHand[MAX_PLAYERS], playerWinner = 0, playerCurrent = 0;
    int playersWithTiedHand[MAX_PLAYERS], tiedHandCount = 0, tiedHandIsBest = 0;
    int stopEvaluating = 0;
    // Get the hand types and strengths for each player
    playerHandtype[0] = FindHandType(player1, cardCount);
    playerStrength[0] = FindHandStrength(player1, playerHandtype[0], cardCount);
    playerHandtype[1] = FindHandType(player2, cardCount);
    playerStrength[1] = FindHandStrength(player2, playerHandtype[1], cardCount);
    if (playerCount >= 3) {
        playerHandtype[2] = FindHandType(player3, cardCount);
        playerStrength[2] = FindHandStrength(player3, playerHandtype[2], cardCount);
    }
    if (playerCount >= 4) {
        playerHandtype[3] = FindHandType(player4, cardCount);
        playerStrength[3] = FindHandStrength(player4, playerHandtype[3], cardCount);
    }
    if (playerCount >= 5) {
        playerHandtype[4] = FindHandType(player5, cardCount);
        playerStrength[4] = FindHandStrength(player5, playerHandtype[4], cardCount);
    }
    if (playerCount >= 6) {
        playerHandtype[5] = FindHandType(player6, cardCount);
        playerStrength[5] = FindHandStrength(player6, playerHandtype[5], cardCount);
    }
    if (playerCount >= 7) {
        playerHandtype[6] = FindHandType(player7, cardCount);
        playerStrength[6] = FindHandStrength(player7, playerHandtype[6], cardCount);
    }
    if (playerCount >= 8) {
        playerHandtype[7] = FindHandType(player8, cardCount);
        playerStrength[7] = FindHandStrength(player8, playerHandtype[7], cardCount);
    }
    if (playerCount >= 9) {
        playerHandtype[8] = FindHandType(player9, cardCount);
        playerStrength[8] = FindHandStrength(player9, playerHandtype[8], cardCount);
    }
    if (playerCount >= 10) {
        playerHandtype[9] = FindHandType(player10, cardCount);
        playerStrength[9] = FindHandStrength(player10, playerHandtype[9], cardCount);
    }
    
    // Find the best hand type out of all the players
    for (i = 0; i < playerCount; i++) {
        if (i == 0) {  // For starting off, set the first player's hand to be the best
            bestHandtype = playerHandtype[i];
        }
        else if (playerHandtype[i] > bestHandtype) {
            bestHandtype = playerHandtype[i];
        }
    }
    printf("\nThe best ");
    PrintHandType(bestHandtype);
    
    // Determine which players have the best hand type
    for (i = 0; i < playerCount; i++) {
        if (playerHandtype[i] == bestHandtype) {
            playersWithBestHand[j] = i;
            bestHandCount++;
            j++;
        }
    }
    // If only 1 player has the best hand type, that player wins the hand
    if (bestHandCount == 1) {
        printf("\nPlayer %d is the winner!\n\n", playersWithBestHand[0]+1);
    }
    // If 2 or more players have the best hand type, use the strength values to break the tie
    else if (bestHandCount >= 2) {
        for (i = 0; i < bestHandCount; i++) {
            if (i == 0) {  // For starting off, set the first player within the tie to be the winner
                playerWinner = playersWithBestHand[i];
            }
            else {
                playerCurrent = playersWithBestHand[i];
                if (playerStrength[playerCurrent].p1 > playerStrength[playerWinner].p1) {
                    playerWinner = playerCurrent;
                    tiedHandIsBest = 0;
                    tiedHandCount = 0;
                }
                else if ((playerStrength[playerCurrent].p1 == playerStrength[playerWinner].p1)
                && (playerStrength[playerCurrent].p2 > playerStrength[playerWinner].p2)) {
                    playerWinner = playerCurrent;
                    tiedHandIsBest = 0;
                    tiedHandCount = 0;
                }
                else if ((playerStrength[playerCurrent].p1 == playerStrength[playerWinner].p1)
                && (playerStrength[playerCurrent].p2 == playerStrength[playerWinner].p2)
                && (playerStrength[playerCurrent].p3 > playerStrength[playerWinner].p3)) {
                    playerWinner = playerCurrent;
                    tiedHandIsBest = 0;
                    tiedHandCount = 0;
                }
                else if ((playerStrength[playerCurrent].p1 == playerStrength[playerWinner].p1)
                && (playerStrength[playerCurrent].p2 == playerStrength[playerWinner].p2)
                && (playerStrength[playerCurrent].p3 == playerStrength[playerWinner].p3)
                && (playerStrength[playerCurrent].p4 > playerStrength[playerWinner].p4)) {
                    playerWinner = playerCurrent;
                    tiedHandIsBest = 0;
                    tiedHandCount = 0;
                }
                else if ((playerStrength[playerCurrent].p1 == playerStrength[playerWinner].p1)
                && (playerStrength[playerCurrent].p2 == playerStrength[playerWinner].p2)
                && (playerStrength[playerCurrent].p3 == playerStrength[playerWinner].p3)
                && (playerStrength[playerCurrent].p4 == playerStrength[playerWinner].p4)
                && (playerStrength[playerCurrent].p5 > playerStrength[playerWinner].p5)) {
                    playerWinner = playerCurrent;
                    tiedHandIsBest = 0;
                    tiedHandCount = 0;
                }
                // If 2 or more players have both the same hand type and strength values, they are tied and the hand ends
                else if ((playerStrength[playerCurrent].p1 == playerStrength[playerWinner].p1)
                && (playerStrength[playerCurrent].p2 == playerStrength[playerWinner].p2)
                && (playerStrength[playerCurrent].p3 == playerStrength[playerWinner].p3)
                && (playerStrength[playerCurrent].p4 == playerStrength[playerWinner].p4)
                && (playerStrength[playerCurrent].p5 == playerStrength[playerWinner].p5)) {
                    if (tiedHandCount == 0) {
                        playersWithTiedHand[0] == playerWinner;
                        playersWithTiedHand[1] == playerCurrent;
                        tiedHandCount += 2;
                        tiedHandIsBest = 1;
                    }
                    else {
                        playersWithTiedHand[tiedHandCount] == playerCurrent;
                        tiedHandCount++;
                    }
                }
                stopEvaluating = 0;
            }
        }
        // Print the winner after evaluating the strength values
        if (tiedHandIsBest == 0) {
            printf("\nPlayer %d is the winner!\n\n", playerWinner+1);
        }
        // If there's still a tie after evaluating the strength values, print the players that are involved in the tie
        else {
            switch (tiedHandCount) {
                case 2: 
                    printf("\nPlayers %d and %d are tied!\n\n", 
                    playersWithTiedHand[0]+1, playersWithTiedHand[1]+1);
                    break;
                case 3:
                    printf("\nPlayers %d, %d, and %d are tied!\n\n", 
                    playersWithTiedHand[0]+1, playersWithTiedHand[1]+1, playersWithTiedHand[2]+1);
                    break;
                case 4:
                    printf("\nPlayers %d, %d, %d, and %d are tied!\n\n", 
                    playersWithTiedHand[0]+1, playersWithTiedHand[1]+1, playersWithTiedHand[2]+1, playersWithTiedHand[3]+1);
                    break;
                case 5:
                    printf("\nPlayers %d, %d, %d, %d, and %d are tied!\n\n", 
                    playersWithTiedHand[0]+1, playersWithTiedHand[1]+1, playersWithTiedHand[2]+1, playersWithTiedHand[3]+1, playersWithTiedHand[4]+1);
                    break;
                case 6:
                    printf("\nPlayers %d, %d, %d, %d, %d, and %d are tied!\n\n", 
                    playersWithTiedHand[0]+1, playersWithTiedHand[1]+1, playersWithTiedHand[2]+1, playersWithTiedHand[3]+1, playersWithTiedHand[4]+1,
                    playersWithTiedHand[5]+1);
                    break;
                case 7:
                    printf("\nPlayers %d, %d, %d, %d, %d, %d, and %d are tied!\n\n", 
                    playersWithTiedHand[0]+1, playersWithTiedHand[1]+1, playersWithTiedHand[2]+1, playersWithTiedHand[3]+1, playersWithTiedHand[4]+1,
                    playersWithTiedHand[5]+1, playersWithTiedHand[6]+1);
                    break;
                case 8:
                    printf("\nPlayers %d, %d, %d, %d, %d, %d, %d, and %d are tied!\n\n", 
                    playersWithTiedHand[0]+1, playersWithTiedHand[1]+1, playersWithTiedHand[2]+1, playersWithTiedHand[3]+1, playersWithTiedHand[4]+1,
                    playersWithTiedHand[5]+1, playersWithTiedHand[6]+1, playersWithTiedHand[7]+1);
                    break;
                case 9:
                    printf("\nPlayers %d, %d, %d, %d, %d, %d, %d, %d, and %d are tied!\n\n", 
                    playersWithTiedHand[0]+1, playersWithTiedHand[1]+1, playersWithTiedHand[2]+1, playersWithTiedHand[3]+1, playersWithTiedHand[4]+1,
                    playersWithTiedHand[5]+1, playersWithTiedHand[6]+1, playersWithTiedHand[7]+1, playersWithTiedHand[8]+1);
                    break;
                case 10:
                    printf("\nPlayers %d, %d, %d, %d, %d, %d, %d, %d, and %d are tied!\n\n", 
                    playersWithTiedHand[0]+1, playersWithTiedHand[1]+1, playersWithTiedHand[2]+1, playersWithTiedHand[3]+1, playersWithTiedHand[4]+1,
                    playersWithTiedHand[5]+1, playersWithTiedHand[6]+1, playersWithTiedHand[7]+1, playersWithTiedHand[8]+1, playersWithTiedHand[9]+1);
                    break;
                default: break;
            }
        }
    }
}

int main()
{
    // SET "mode" TO "TEST_MODE" FOR TESTING CUSTOM HANDS (NOT RANDOM)
    // SET "mode" TO "NORMAL_MODE" FOR RUNNING A STANDARD GAME WITH RANDOMLY GENERATED HANDS
    int mode = NORMAL_MODE;

    // User inputs
    gametype currentGameType = noGameType;
    int numberOfPlayers = 0;
    action currentAction = noAction;
    int numberOfCardsForSim = 0;
    int numberOfHandsForSim = 0;

    int i = 0;
    int userInput = NOT_VALID;
    int numberOfCards = 0;
    handtype currentHandType;
    strength currentStrength;
    
    // Monte Carlo simulation related variables
    int handCount = 0;
    int aceHighOrLessCount = 0;
    int pairCount = 0;
    int twoPairCount = 0;
    int threeOfKindCount = 0;
    int straightCount = 0;
    int flushCount = 0;
    int fullHouseCount = 0;
    int fourOfKindCount = 0;
    int straightFlushCount = 0;
    int royalFlushCount = 0;
    int handSamples = 0;
    float startTime = 0.0;
    float endTime = 0.0;
    float timeElapsed = 0.0;

    // Command line propmts for user inputs
    setupGame(&currentGameType, &numberOfPlayers, &numberOfCardsForSim, &numberOfHandsForSim);

    if ((currentGameType == fiveCardStud) || (currentGameType == sevenCardStud) || (currentGameType == texasHoldEm)) {
        // Determine the number of cards per hand based on the current game type
        switch (currentGameType) {
            case fiveCardStud: numberOfCards = 5; break;
            case sevenCardStud: numberOfCards = 7; break;
            case texasHoldEm: numberOfCards = 7; break;
            default: break;
        }
        
        while(1) {

        // TEST HANDS
        if (mode == TEST_MODE) {
            if (numberOfCards == 5) {
                player1[0].p = eight, player1[0].s = club;
                player1[1].p = eight, player1[1].s = spade;
                player1[2].p = jack, player1[2].s = diamond;
                player1[3].p = jack, player1[3].s = heart;
                player1[4].p = five, player1[4].s = spade;
                player2[0].p = two, player2[0].s = spade;
                player2[1].p = three, player2[1].s = club;
                player2[2].p = ace, player2[2].s = spade;
                player2[3].p = ten, player2[3].s = heart;
                player2[4].p = queen, player2[4].s = spade;
                player3[0].p = queen, player3[0].s = spade;
                player3[1].p = king, player3[1].s = club;
                player3[2].p = nine, player3[2].s = club;
                player3[3].p = six, player3[3].s = heart;
                player3[4].p = nine, player3[4].s = spade;
                player4[0].p = eight, player4[0].s = spade;
                player4[1].p = seven, player4[1].s = club;
                player4[2].p = five, player4[2].s = spade;
                player4[3].p = ten, player4[3].s = heart;
                player4[4].p = six, player4[4].s = spade;
            }
            else if (numberOfCards == 7) {
                player1[0].p = ten, player1[0].s = club;
                player1[1].p = king, player1[1].s = club;
                player1[2].p = six, player1[2].s = club;
                player1[3].p = seven, player1[3].s = club;
                player1[4].p = three, player1[4].s = club;
                player1[5].p = two, player1[5].s = club;
                player1[6].p = four, player1[6].s = club;
                player2[0].p = seven, player2[0].s = spade;
                player2[1].p = eight, player2[1].s = club;
                player2[2].p = nine, player2[2].s = spade;
                player2[3].p = two, player2[3].s = heart;
                player2[4].p = ten, player2[4].s = diamond;
                player2[5].p = jack, player2[5].s = heart;
                player2[6].p = three, player2[6].s = spade;
            }
        }
        else {
            // Use the current time as a seed for the random generators
            srand(time(0));

            // Deal enough cards for all players
            for (i = 0; i < (numberOfCards * numberOfPlayers); i++) {
                cardsInPlay[i] = DealCard(numberOfCards, numberOfPlayers);
            }

            // Assign the dealt cards to each player
            AssignCard(numberOfCards, numberOfPlayers);
        }

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

        }  // END OF WHILE(1) LOOP
    }
    else if (currentGameType == monteCarlo) {
        // Start the benchmark timer for the Monte Carlo simulation
        startTime = (float)clock()/CLOCKS_PER_SEC;

        handCount = 0;
        aceHighOrLessCount = 0;
        pairCount = 0;
        twoPairCount = 0;
        threeOfKindCount = 0;
        straightCount = 0;
        flushCount = 0;
        fullHouseCount = 0;
        fourOfKindCount = 0;
        straightFlushCount = 0;
        royalFlushCount = 0;
        handSamples = 0;
        
        // Deal hands until we reach our limit for the simulation
        for (handCount = 0; handCount < numberOfHandsForSim; handCount++) {
            // Add card to hand until there are enough cards
            for (i = 0; i < numberOfCardsForSim; i++) {
                handSim[i] = DealCard(numberOfCardsForSim, 1);
            }

            // Determine the type of hand that was dealt
            currentHandType = FindHandType(handSim, numberOfCardsForSim);

            // FOR DEBUGGING - Print a certain number of hands based on certain criteria
            //if ((currentHandType == fourOfKind) && (handSamples < 100)) {
            //    PrintHand(handSim, numberOfCardsForSim);
            //    handSamples++;
            //}

            // Keep track of how many times each hand type occurs during the simulation
            switch (currentHandType) {
                case aceHighOrLess: aceHighOrLessCount += 1; break;
                case singlePair: pairCount += 1; break;
                case twoPair: twoPairCount += 1; break;
                case threeOfKind: threeOfKindCount += 1; break;
                case stdStraight: straightCount += 1; break;
                case stdFlush: flushCount += 1; break;
                case fullHouse: fullHouseCount += 1; break;
                case fourOfKind: fourOfKindCount += 1; break;
                case straightFlush: straightFlushCount += 1; break;
                case royalFlush: royalFlushCount += 1; break;
                default: break;
            }
        }

        // Print the count and probaility data to the screen
        printf("\nMonte Carlo simulation ran with %d hands, %d cards per hand.\n\n", numberOfHandsForSim, numberOfCardsForSim);
        printf("Number of hands with an ace high or less = %d\n", aceHighOrLessCount);
        printf("Probability = %f\n\n", aceHighOrLessCount / (float)numberOfHandsForSim);
        printf("Number of hands with a single pair = %d\n", pairCount);
        printf("Probability = %f\n\n", pairCount / (float)numberOfHandsForSim);
        printf("Number of hands with two pairs = %d\n", twoPairCount);
        printf("Probability = %f\n\n", twoPairCount / (float)numberOfHandsForSim);
        printf("Number of hands with a three of a kind = %d\n", threeOfKindCount);
        printf("Probability = %f\n\n", threeOfKindCount / (float)numberOfHandsForSim);
        printf("Number of hands with a straight = %d\n", straightCount);
        printf("Probability = %f\n\n", straightCount / (float)numberOfHandsForSim);
        printf("Number of hands with a flush = %d\n", flushCount);
        printf("Probability = %f\n\n", flushCount / (float)numberOfHandsForSim);
        printf("Number of hands with a full houses = %d\n", fullHouseCount);
        printf("Probability = %f\n\n", fullHouseCount / (float)numberOfHandsForSim);
        printf("Number of hands with a four of a kind = %d\n", fourOfKindCount);
        printf("Probability = %f\n\n", fourOfKindCount / (float)numberOfHandsForSim);
        printf("Number of hands with a straight flush = %d\n", straightFlushCount);
        printf("Probability = %f\n\n", straightFlushCount / (float)numberOfHandsForSim);
        printf("Number of hands with a royal flush = %d\n", royalFlushCount);
        printf("Probability = %f\n\n", royalFlushCount / (float)numberOfHandsForSim);

        // Print the time elapsed for the Monte Carlo simulation
        endTime = (float)clock()/CLOCKS_PER_SEC;
        timeElapsed = endTime - startTime;
        printf("Simulation completed in %0.3f seconds\n\n", timeElapsed);
    }

    return 0;
}