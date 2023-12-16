/*
Description:
This sets up the functions that are necessary for various poker games.

*/

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#include "carddeck.h"

const int FOUND = 1;
const int NOT_FOUND = 0;

typedef struct strength{pips p1; pips p2; pips p3; pips p4; pips p5;} strength;
typedef enum handtype{aceHighOrLess, singlePair, twoPair, threeOfKind, stdStraight, stdFlush, fullHouse, fourOfKind, straightFlush, royalFlush} handtype;
typedef enum action{check, bet, fold} action;

// FOR DEBUGGING - Print the hand type to the screen in a readable format
void PrintHandType(handtype ht)
{
    cout << "hand type is ";
    // Print the hand type value
    switch (ht) {
        case royalFlush: cout << "royal flush"; break;
        case straightFlush: cout << "straight flush"; break;
        case fourOfKind: cout << "four of a kind"; break;
        case fullHouse: cout << "full house"; break;
        case stdFlush: cout << "flush"; break;
        case stdStraight: cout << "straight"; break;
        case threeOfKind: cout << "three of a kind"; break;
        case twoPair: cout << "two pair"; break;
        case singlePair: cout << "pair"; break;
        case aceHighOrLess: cout << "ace high or less"; break;
        default: cout << "ERROR"; break;
    }
    cout << endl;
}

// Determine if the hand contains a single pair
int determineSinglePair(vector<card> hand)
{
    int i = 0, j = 0;
    for (i = 0; i < hand.size(); i++) {
        for (j = i+1; j < hand.size(); j++) {
            if (hand[i].p == hand[j].p) {
                return FOUND;  // Found a single pair in the hand
            }   
        }    
    }
    return NOT_FOUND;  // Did not find any pairs in the hand
}

// Determine if the hand contains two pairs
int determineTwoPair(vector<card> hand)
{
    int i = 0, j = 0, pairTracker = 0;
    for (i = 0; i < hand.size(); i++) {
        for (j = i+1; j < hand.size(); j++) {
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
int determineThreeOfKind(vector<card> hand)
{
    int i = 0, j = 0, threeOfKindTracker = 0;
    for (i = 0; i < hand.size(); i++) {
        threeOfKindTracker = 0;
        for (j = i+1; j < hand.size(); j++) {
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
int determineStraight(vector<card> hand)
{
    int i = 0, j = 0;
    int straightUpPossible = NOT_FOUND, straightDownPossible = NOT_FOUND;
    int straightUpPips[4], straightDownPips[4];
    for (i = 0; i < hand.size(); i++) {
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
        for (j = 0; j < hand.size(); j++) {
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
int determineFlush(vector<card> hand)
{
    int i = 0, j = 0, flushTracker = 0;
    for (i = 0; i < hand.size(); i++) {
        flushTracker = 0;
        for (j = i+1; j < hand.size(); j++) {
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
int determineFullHouse(vector<card> hand)
{
    int i = 0, j = 0, threeOfKindTracker = 0, threeOfKindPips = 0;
    for (i = 0; i < hand.size(); i++) {
        threeOfKindTracker = 0;
        for (j = i+1; j < hand.size(); j++) {
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
        for (i = 0; i < hand.size(); i++) {
            // Only check for pairs with a different pips value than the three of a kind pips
            if (!(hand[i].p == threeOfKindPips)) {
                for (j = i+1; j < hand.size(); j++) {
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
int determineFourOfKind(vector<card> hand)
{
    int i = 0, j = 0, fourOfKindTracker = 0;
    for (i = 0; i < hand.size(); i++) {
        fourOfKindTracker = 0;
        for (j = i+1; j < hand.size(); j++) {
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
int determineStraightFlush(vector<card> hand)
{
    int straightResult = 0, flushResult = 0;
    straightResult = determineStraight(hand);
    if (straightResult == FOUND) {
        flushResult = determineFlush(hand);
        if (flushResult == FOUND) {
            return FOUND;
        }
    }
    return NOT_FOUND;
}

// Determine if the hand contains a royal flush
int determineRoyalFlush(vector<card> hand)
{
    int i = 0, result = NOT_FOUND, foundTen = NOT_FOUND, foundAce = NOT_FOUND;
    result = determineStraightFlush(hand);
    if (result == FOUND) {
        for (i = 0; i < hand.size(); i++) {
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
handtype FindHandType(vector<card> hand, int cardCount)
{
    int result = NOT_FOUND;

    // Determine if the hand contains a royal flush
    result = determineRoyalFlush(hand);
    if (result == FOUND) {
        return royalFlush;
    }

    // Determine if the hand contains a straight flush
    result = determineStraightFlush(hand);
    if (result == FOUND) {
        return straightFlush;
    }

    // Determine if the hand contains four of a kind
    result = determineFourOfKind(hand);
    if (result == FOUND) {
        return fourOfKind;
    }

    // Determine if the hand contains a full house
    result = determineFullHouse(hand);
    if (result == FOUND) {
        return fullHouse;
    }

    // Determine if the hand contains a flush
    result = determineFlush(hand);
    if (result == FOUND) {
        return stdFlush;
    }

    // Determine if the hand contains a straight
    result = determineStraight(hand);
    if (result == FOUND) {
        return stdStraight;
    }

    // Determine if the hand contains three of a kind
    result = determineThreeOfKind(hand);
    if (result == FOUND) {
        return threeOfKind;
    }

    // Determine if the hand contains two pairs
    result = determineTwoPair(hand);
    if (result == FOUND) {
        return twoPair;
    }

    // Determine if the hand contains a single pair
    result = determineSinglePair(hand);
    if (result == FOUND) {
        return singlePair;
    }
    
    // At this point, assume the hand is an ace high or less
    return aceHighOrLess;

}