/*
Description:
Monte Carlo simulation for determining the probability of each poker hand.

*/

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
using namespace std;

#include "poker.h"

const int VALID = 1;
const int NOT_VALID = 0;

int main() {
    vector<card> handSim;
    vector<card> mainDeck;
    array<vector<card>,2> cardStacks;
    handtype currentHandType;
    int cardCount = 0;
    int numberOfHandsForSim = 0;
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
    int userInput = NOT_VALID;

    while (userInput == NOT_VALID) {
        cout << endl << "How many cards per hand?" << endl;
        cout << "Enter a number between '5' and '9'" << endl;
        cin >> cardCount;
        if ((cardCount <= 9) && (cardCount >= 5)) {
            userInput = VALID;
        }
        else {
            cout << "NOT A VALID ENTRY!" << endl << endl;
        }
    }
    userInput = NOT_VALID;
    while (userInput == NOT_VALID) {
        cout << endl << "How many total hands for running the simulation?" << endl;
        cout << "Enter a number between '1' and '10000000'" << endl;
        cin >> numberOfHandsForSim;
        if ((numberOfHandsForSim <= 10000000) && (numberOfHandsForSim >= 1)) {
            userInput = VALID;
        }
        else {
            cout << "NOT A VALID ENTRY!" << endl << endl;
        }
    }

    // Start the benchmark timer for the Monte Carlo simulation
    float startTime = (float)clock()/CLOCKS_PER_SEC;
    
    // Deal hands until we reach our limit for the simulation
    for (handCount = 0; handCount < numberOfHandsForSim; handCount++) {
        // Create a single deck, shuffle, and deal out cards
        mainDeck = createDeck(singleDeck, 0);
        mainDeck = shuffleDeck(mainDeck);
        cardStacks = moveCard(mainDeck, handSim, 0, cardCount);
        handSim = cardStacks[1];

        // Determine the type of hand that was dealt, then reset the hand
        currentHandType = FindHandType(handSim, cardCount);
        handSim.erase(handSim.begin(), handSim.end());

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
    cout << endl << "Monte Carlo simulation ran with " << numberOfHandsForSim << " hands, " << cardCount << " cards per hand." << endl << endl;
    cout << setprecision(4);
    cout << "Number of hands with an ace high or less = " << aceHighOrLessCount << endl;
    cout << "Probability = " << aceHighOrLessCount / (float)numberOfHandsForSim << endl << endl;
    cout << "Number of hands with a single pair = " << pairCount << endl;
    cout << "Probability = " << pairCount / (float)numberOfHandsForSim << endl << endl;
    cout << "Number of hands with two pairs = " << twoPairCount << endl;
    cout << "Probability = " << twoPairCount / (float)numberOfHandsForSim << endl << endl;
    cout << "Number of hands with a three of a kind = " << threeOfKindCount << endl;
    cout << "Probability = " << threeOfKindCount / (float)numberOfHandsForSim << endl << endl;
    cout << "Number of hands with a straight = " << straightCount << endl;
    cout << "Probability = " << straightCount / (float)numberOfHandsForSim << endl << endl;
    cout << "Number of hands with a flush = " << flushCount << endl;
    cout << "Probability = " << flushCount / (float)numberOfHandsForSim << endl << endl;
    cout << "Number of hands with a full houses = " << fullHouseCount << endl;
    cout << "Probability = " << fullHouseCount / (float)numberOfHandsForSim << endl << endl;
    cout << "Number of hands with a four of a kind = " << fourOfKindCount << endl;
    cout << "Probability = " << fourOfKindCount / (float)numberOfHandsForSim << endl << endl;
    cout << "Number of hands with a straight flush = " << straightFlushCount << endl;
    cout << "Probability = " << straightFlushCount / (float)numberOfHandsForSim << endl << endl;
    cout << "Number of hands with a royal flush = " << royalFlushCount << endl;
    cout << "Probability = " << royalFlushCount / (float)numberOfHandsForSim << endl << endl;

    // Print the time elapsed for the Monte Carlo simulation
    float endTime = (float)clock()/CLOCKS_PER_SEC;
    float timeElapsed = endTime - startTime;
    cout << "Simulation completed in " << timeElapsed << " seconds" << endl << endl;

    return 0;
}