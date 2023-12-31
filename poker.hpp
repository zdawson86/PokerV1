/*
Description:
This sets up the functions that are necessary for various poker games.

*/

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#include "carddeck.hpp"

const int FOUND = 1;
const int NOT_FOUND = 0;

typedef struct strength{pips p1; pips p2; pips p3; pips p4; pips p5;} strength;
typedef enum handtype{aceHighOrLess, singlePair, twoPair, threeOfKind, stdStraight, stdFlush, fullHouse, fourOfKind, straightFlush, royalFlush} handtype;
typedef enum action{checkCall, betRaise, fold} action;
typedef struct computerAction{action a; int betSize;} computerAction;

// Swap the values at 2 different pips addresses
void swapPips(pips* xp, pips* yp)
{
	pips temp = *xp;
	*xp = *yp;
	*yp = temp;
}

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
        if (straightDownPips[3] == acelow) {
            straightDownPips[3] = ace;  // Ace is low for a downward straight
        }
        else if (straightDownPips[3] < acelow) {
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
handtype findHandType(vector<card> hand)
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

// Find the strength of a hand with ace high or less
strength strengthAceHighOrLess(vector<card> hand)
{
    int i = 0;
    vector<pips> pipsVector; 
    strength cardStrength;
    // Reset the pips array to all zeros
    pipsVector.erase(pipsVector.begin(), pipsVector.end());
    // Get pips values from the hand
    for (i = 0; i < hand.size(); i++) {
        pipsVector.push_back(hand[i].p);
    }
    // Sort the hand's pips values into the card strength struct (p1 strongest, p5 weakest)
    sort(pipsVector.begin(), pipsVector.end(), greater<int>());
    cardStrength.p1 = pipsVector[0];
    cardStrength.p2 = pipsVector[1];
    cardStrength.p3 = pipsVector[2];
    cardStrength.p4 = pipsVector[3];
    cardStrength.p5 = pipsVector[4];

    return cardStrength;
}

// Find the strength of a hand with a single pair
strength strengthSinglePair(vector<card> hand)
{
    int i = 0, j = 0;
    pips pairPips = acelow;
    vector<pips> pipsVector;
    strength cardStrength;
    // Reset the pips vector to all zeros
    pipsVector.erase(pipsVector.begin(), pipsVector.end());
    // Get pips values from the hand
    for (i = 0; i < hand.size(); i++) {
        pipsVector.push_back(hand[i].p);
    }
    // Find the pair in the hand
    for (i = 0; i < hand.size(); i++) {
        for (j = i+1; j < hand.size(); j++) {
            if (hand[i].p == hand[j].p) {
                pairPips = hand[i].p;
                break;
            }
        }
    }
    // Remove the pair pips from the pips vector
    pipsVector.erase(pipsVector.begin(), pipsVector.end());
    for (i = 0; i < hand.size(); i++) {
        if (!(hand[i].p == pairPips)) {
            pipsVector.push_back(hand[i].p);
        }
    }
    // Sort the remaining pips values for the card strength struct
    sort(pipsVector.begin(), pipsVector.end(), greater<int>());
    cardStrength.p1 = pairPips;  // pips value for the pair
    cardStrength.p2 = pipsVector[0];  // highest single pips
    cardStrength.p3 = pipsVector[1];  // second highest single pips
    cardStrength.p4 = pipsVector[2];  // lowest single pips
    cardStrength.p5 = acelow;  // not used

    return cardStrength;
}

// Find the strength of a hand with two pairs
strength strengthTwoPair(vector<card> hand)
{
    int i = 0, j = 0, temp = 0;
    pips pairPipsHigh = acelow, pairPipsLow = acelow, thirdPairPips = acelow, singlePips =acelow;
    int firstPairFound = NOT_FOUND;
    vector<pips> pipsVector;
    strength cardStrength;
    // Reset the pips array to all zeros
    pipsVector.erase(pipsVector.begin(), pipsVector.end());
    // Get pips values from the hand
    for (i = 0; i < hand.size(); i++) {
        pipsVector.push_back(hand[i].p);
    }
    // Find the 2 pairs in the hand
    for (i = 0; i < hand.size(); i++) {
        for (j = i+1; j < hand.size(); j++) {
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
    for (i = 0; i < hand.size(); i++) {
        for (j = i+1; j < hand.size(); j++) {
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
    pipsVector.erase(pipsVector.begin(), pipsVector.end());
    for (i = 0; i < hand.size(); i++) {
        if ((!(hand[i].p == pairPipsHigh)) && (!(hand[i].p == pairPipsLow))) {
            pipsVector.push_back(hand[i].p);
        }   
    }
    sort(pipsVector.begin(), pipsVector.end(), greater<int>());
    // Check if the "high" pips value is actually higher than the "low" pips value
    // If not, swap them
    if (pairPipsHigh < pairPipsLow) {
        swapPips(&pairPipsHigh, &pairPipsLow);
    }
    // Assign the pips values to the card strength struct
    cardStrength.p1 = pairPipsHigh;  // high pair pips
    cardStrength.p2 = pairPipsLow;  // low pair pips
    cardStrength.p3 = pipsVector[0];  // single pips
    cardStrength.p4 = acelow;  // not used
    cardStrength.p5 = acelow;  // not used
    
    return cardStrength;
}

// Find the strength of a hand with three of a kind
strength strengthThreeOfKind(vector<card> hand)
{
    int i = 0, j = 0, threeOfKindTracker = 0;
    pips threeOfKindPips = acelow;
    vector<pips> pipsVector;
    strength cardStrength;
    // Reset the pips array to all zeros
    pipsVector.erase(pipsVector.begin(), pipsVector.end());
    // Get pips values from the hand
    for (i = 0; i < hand.size(); i++) {
        pipsVector.push_back(hand[i].p);
    }
    // Find the three of a kind pips value in the hand
    for (i = 0; i < hand.size(); i++) {
        threeOfKindTracker = 0;
        for (j = i+1; j < hand.size(); j++) {
            if ((hand[i].p == hand[j].p) && threeOfKindTracker == 1) {
                threeOfKindPips = hand[i].p;
            }
            else if (hand[i].p == hand[j].p) {
                threeOfKindTracker = 1;
            }
        }
    }
    // Remove the three of a kind pips from the pips array
    pipsVector.erase(pipsVector.begin(), pipsVector.end());
    for (i = 0; i < hand.size(); i++) {
        if (!(hand[i].p == threeOfKindPips)) {
            pipsVector.push_back(hand[i].p);
        }   
    }
    // Sort the remaining pips values for the card strength struct
    sort(pipsVector.begin(), pipsVector.end(), greater<int>());
    cardStrength.p1 = threeOfKindPips;  // pips value for the three of a kind
    cardStrength.p2 = pipsVector[0];  // highest single pips
    cardStrength.p3 = pipsVector[1];  // lowest single pips
    cardStrength.p4 = acelow;  // not used
    cardStrength.p5 = acelow;  // not used
    
    return cardStrength;
}

// Find the strength of a hand with a straight
strength strengthStraight(vector<card> hand)
{
    int i = 0;
    vector<pips> pipsVector;
    int fiveFound = NOT_FOUND, aceFound = NOT_FOUND;
    pips straightPipsHigh = acelow; 
    strength cardStrength;
    // Reset the pips array to all zeros
    pipsVector.erase(pipsVector.begin(), pipsVector.end());
    // Get pips values from the hand
    for (i = 0; i < hand.size(); i++) {
        pipsVector.push_back(hand[i].p);
        // If both a five and ace are found, convert the ace pips value to be low instead of high
        if (pipsVector[i] == five) {
            fiveFound = FOUND;
        }
        else if (pipsVector[i] == ace) {
            aceFound = FOUND;
        }
        if ((fiveFound == FOUND) && (aceFound == FOUND)) {
            for (i = 0; i < hand.size(); i++) {
                if (pipsVector[i] == ace) {
                    pipsVector[i] = acelow;  // ace is low for this particular straight
                    break;
                }
            }
        }
    }
    // Sort the pips values and determine if either of the top two pips values are part of the straight (applicable for 7 card hands)
    // Should only need to check the next two pips values directly below the pips value being checked to verify the straight
    sort(pipsVector.begin(), pipsVector.end(), greater<int>());
    if ((pipsVector[1] == (pipsVector[0]-1))
    && (pipsVector[2] == (pipsVector[0]-2))) {
        straightPipsHigh = pipsVector[0];
    }
    else if ((pipsVector[2] == (pipsVector[1]-1))
    && (pipsVector[3] == (pipsVector[1]-2))) {
        straightPipsHigh = pipsVector[1];
    }
    else {
        straightPipsHigh = pipsVector[2];
    }
    // Assign the hand's pips values into the card strength struct (p1 strongest, p5 weakest)
    cardStrength.p1 = straightPipsHigh;  // highest pips value of the straight
    cardStrength.p2 = acelow;  // not used
    cardStrength.p3 = acelow;  // not used
    cardStrength.p4 = acelow;  // not used
    cardStrength.p5 = acelow;  // not used
    
    return cardStrength;
}

// Find the strength of a hand with a flush
strength strengthFlush(vector<card> hand)
{
    int i = 0, j = 0; 
    vector<pips> pipsVector;
    int flushTracker = 0;
    suit flushSuit;
    strength cardStrength;
    // Find the flush suit (5 cards of same suit)
    for (i = 0; i < hand.size(); i++) {
        flushTracker = 0;
        for (j = i+1; j < hand.size(); j++) {
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
    pipsVector.erase(pipsVector.begin(), pipsVector.end());
    // Get pips values from the hand for only the flush suit
    for (i = 0; i < hand.size(); i++) {
        if (hand[i].s == flushSuit) {
            pipsVector.push_back(hand[i].p);
        }
    }
    // Sort the hand's pips values into the card strength struct (p1 strongest, p5 weakest)
    sort(pipsVector.begin(), pipsVector.end(), greater<int>());
    cardStrength.p1 = pipsVector[0];
    cardStrength.p2 = pipsVector[1];
    cardStrength.p3 = pipsVector[2];
    cardStrength.p4 = pipsVector[3];
    cardStrength.p5 = pipsVector[4];
    
    return cardStrength;
}

// Find the strength of a hand with a full house
strength strengthFullHouse(vector<card> hand)
{
    int i = 0, j = 0, threeOfKindTracker = 0, pairTracker = 0;
    pips threeOfKindPips = acelow, threeOfKindPips2 = acelow, pairPips = acelow, pairPips2 = acelow;
    strength cardStrength;
    // Find the three of a kind pips value
    for (i = 0; i < hand.size(); i++) {
        threeOfKindTracker = 0;
        for (j = i+1; j < hand.size(); j++) {
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
    for (i = 0; i < hand.size(); i++) {
        threeOfKindTracker = 0;
        for (j = i+1; j < hand.size(); j++) {
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
    for (i = 0; i < hand.size(); i++) {
        pairTracker = 0;
        for (j = i+1; j < hand.size(); j++) {
            if ((hand[i].p == hand[j].p) && (!(hand[i].p == threeOfKindPips))) {
                pairPips = hand[i].p;
                break;
            }
        }
    }
    pairTracker = 0;
    // Check to see if there's a second pair (possible for 7 card hands)
    for (i = 0; i < hand.size(); i++) {
        pairTracker = 0;
        for (j = i+1; j < hand.size(); j++) {
            if ((hand[i].p == hand[j].p) && (!(hand[i].p == threeOfKindPips)) && (!(hand[i].p == pairPips))) {
                pairPips2 = hand[i].p;
            }
        }
    }
    // If the second pair is bigger than the first pair, replace the first pair with the second pair's pips value
    if (pairPips2 > pairPips) {
        pairPips = pairPips2;
    }
    // Assign pips values for the card strength struct
    cardStrength.p1 = threeOfKindPips;  // pips value for the three of a kind
    cardStrength.p2 = pairPips;  // pips value for the pair
    cardStrength.p3 = acelow;  // not used
    cardStrength.p4 = acelow;  // not used
    cardStrength.p5 = acelow;  // not used

    return cardStrength;
}

// Find the strength of a hand with four of a kind
strength strengthFourOfKind(vector<card> hand)
{
    int i = 0, j = 0, fourOfKindTracker = 0;
    pips fourOfKindPips = acelow, singlePips = acelow;
    vector<pips> pipsVector;
    strength cardStrength;
    // Reset the pips array to all zeros
    pipsVector.erase(pipsVector.begin(), pipsVector.end());
    // Get pips values from the hand
    for (i = 0; i < hand.size(); i++) {
        pipsVector.push_back(hand[i].p);
    }
    // Find the four of a kind pips value in the hand
    for (i = 0; i < hand.size(); i++) {
        fourOfKindTracker = 0;
        for (j = i+1; j < hand.size(); j++) {
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
    pipsVector.erase(pipsVector.begin(), pipsVector.end());
    for (i = 0; i < hand.size(); i++) {
        if (!(hand[i].p == fourOfKindPips)) {
            pipsVector.push_back(hand[i].p);
        }   
    }
    sort(pipsVector.begin(), pipsVector.end(), greater<int>());
    // Assign pips values for the card strength struct
    cardStrength.p1 = fourOfKindPips;  // pips value for the four of a kind
    cardStrength.p2 = pipsVector[0];  // highest single pips
    cardStrength.p3 = acelow;  // not used
    cardStrength.p4 = acelow;  // not used
    cardStrength.p5 = acelow;  // not used
    
    return cardStrength;
}

// Find the strength of a hand with a straight flush
strength strengthStraightFlush(vector<card> hand)
{
    int i = 0, j = 0, flushTracker = 0;
    suit flushSuit; 
    vector<pips> pipsVector;
    pips straightPipsHigh = acelow;
    strength cardStrength;
    // Find the flush suit (5 cards of same suit)
    for (i = 0; i < hand.size(); i++) {
        flushTracker = 0;
        for (j = i+1; j < hand.size(); j++) {
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
    pipsVector.erase(pipsVector.begin(), pipsVector.end());
    // Get pips values from the hand for only the flush suit
    for (i = 0; i < hand.size(); i++) {
        if (hand[i].s == flushSuit) {
            pipsVector.push_back(hand[i].p);
            // If an ace is found, it has to be converted to be low instead of high (high would be royal flush)
            if (pipsVector.back() == ace) {
                pipsVector.back() = acelow;
            }
        }
    }
    // Sort the pips values and determine if either of the top two pips values are part of the straight (applicable for 7 card hands)
    // Should only need to check the next two pips values directly below the pips value being checked to verify the straight
    sort(pipsVector.begin(), pipsVector.end(), greater<int>());
    if ((pipsVector[1] == (pipsVector[0]-1))
    && (pipsVector[2] == (pipsVector[0]-2))) {
        straightPipsHigh = pipsVector[0];
    }
    else if ((pipsVector[2] == (pipsVector[1]-1))
    && (pipsVector[3] == (pipsVector[1]-2))) {
        straightPipsHigh = pipsVector[1];
    }
    else {
        straightPipsHigh = pipsVector[2];
    }
    // Assign the hand's pips values into the card strength struct (p1 strongest, p5 weakest)
    sort(pipsVector.begin(), pipsVector.end(), greater<int>());
    cardStrength.p1 = straightPipsHigh;  // highest pips value of the straight flush
    cardStrength.p2 = acelow;  // not used
    cardStrength.p3 = acelow;  // not used
    cardStrength.p4 = acelow;  // not usedv
    cardStrength.p5 = acelow;  // not used
    
    return cardStrength;
}

// Find the strength of the hand that was dealt based on the hand type
strength findHandStrength(vector<card> hand, handtype ht)
{
    strength result, cardStrength;
    switch (ht) {
        case aceHighOrLess: result = strengthAceHighOrLess(hand); break;
        case singlePair: result = strengthSinglePair(hand); break;
        case twoPair: result = strengthTwoPair(hand); break;
        case threeOfKind: result = strengthThreeOfKind(hand); break;
        case stdStraight: result = strengthStraight(hand); break;
        case stdFlush: result = strengthFlush(hand); break;
        case fullHouse: result = strengthFullHouse(hand); break;
        case fourOfKind: result = strengthFourOfKind(hand); break;
        case straightFlush: result = strengthStraightFlush(hand); break;
        default: break;
    }

    return result;
}

// Determine who has the best hand out of all of the players
vector<int> determineWinner(vector<vector<card>> playerHand)
{
    int bestHandCount = 0;
    handtype bestHandtype = aceHighOrLess;
    vector<handtype> playerHandtype;
    vector<strength> playerStrength;
    vector<int> playersWithBestHand;
    int playerWinner = 0;
    int playerCurrent = 0;
    vector<int> winnersOut;
    vector<int> playersWithTiedHand;
    int tiedHandIsBest = 0;
    int stopEvaluating = 0;

    // Get the hand types and strengths for each player
    for (int i = 0; i < playerHand.size(); i++) {
        playerHandtype.push_back(findHandType(playerHand[i]));
        playerStrength.push_back(findHandStrength(playerHand[i], playerHandtype[i]));
    }
    // Find the best hand type out of all the players
    for (int i = 0; i < playerHand.size(); i++) {
        if (i == 0) {  // For starting off, set the first player's hand to be the best
            bestHandtype = playerHandtype[i];
        }
        else if (playerHandtype[i] > bestHandtype) {
            bestHandtype = playerHandtype[i];
        }
    }
    
    // Determine which players have the best hand type
    for (int i = 0; i < playerHand.size(); i++) {
        if (playerHandtype[i] == bestHandtype) {
            playersWithBestHand.push_back(i);
            bestHandCount++;
        }
    }
    // If only 1 player has the best hand type, that player wins the hand
    if (bestHandCount == 1) {
        playerWinner = playersWithBestHand[0];
        if (playerWinner == 0) {
            cout << endl << endl << "You won the hand!" << endl << endl;
        }
        else {
            cout << endl << endl << "Player " << playerWinner+1 << " won the hand!" << endl << endl;
        }
        winnersOut.push_back(playerWinner);
    }
    // If 2 or more players have the best hand type, use the strength values to break the tie
    else if (bestHandCount >= 2) {
        for (int i = 0; i < bestHandCount; i++) {
            if (i == 0) {  // For starting off, set the first player within the tie to be the winner
                playerWinner = playersWithBestHand[i];
            }
            else {
                playerCurrent = playersWithBestHand[i];
                if (playerStrength[playerCurrent].p1 > playerStrength[playerWinner].p1) {
                    playerWinner = playerCurrent;
                    tiedHandIsBest = 0;
                }
                else if ((playerStrength[playerCurrent].p1 == playerStrength[playerWinner].p1)
                && (playerStrength[playerCurrent].p2 > playerStrength[playerWinner].p2)) {
                    playerWinner = playerCurrent;
                    tiedHandIsBest = 0;
                }
                else if ((playerStrength[playerCurrent].p1 == playerStrength[playerWinner].p1)
                && (playerStrength[playerCurrent].p2 == playerStrength[playerWinner].p2)
                && (playerStrength[playerCurrent].p3 > playerStrength[playerWinner].p3)) {
                    playerWinner = playerCurrent;
                    tiedHandIsBest = 0;
                }
                else if ((playerStrength[playerCurrent].p1 == playerStrength[playerWinner].p1)
                && (playerStrength[playerCurrent].p2 == playerStrength[playerWinner].p2)
                && (playerStrength[playerCurrent].p3 == playerStrength[playerWinner].p3)
                && (playerStrength[playerCurrent].p4 > playerStrength[playerWinner].p4)) {
                    playerWinner = playerCurrent;
                    tiedHandIsBest = 0;
                }
                else if ((playerStrength[playerCurrent].p1 == playerStrength[playerWinner].p1)
                && (playerStrength[playerCurrent].p2 == playerStrength[playerWinner].p2)
                && (playerStrength[playerCurrent].p3 == playerStrength[playerWinner].p3)
                && (playerStrength[playerCurrent].p4 == playerStrength[playerWinner].p4)
                && (playerStrength[playerCurrent].p5 > playerStrength[playerWinner].p5)) {
                    playerWinner = playerCurrent;
                    tiedHandIsBest = 0;
                }
                // If 2 or more players have both the same hand type and strength values, they are tied and the hand ends
                else if ((playerStrength[playerCurrent].p1 == playerStrength[playerWinner].p1)
                && (playerStrength[playerCurrent].p2 == playerStrength[playerWinner].p2)
                && (playerStrength[playerCurrent].p3 == playerStrength[playerWinner].p3)
                && (playerStrength[playerCurrent].p4 == playerStrength[playerWinner].p4)
                && (playerStrength[playerCurrent].p5 == playerStrength[playerWinner].p5)) {
                    if (playersWithTiedHand.size() == 0) {
                        playersWithTiedHand.push_back(playerWinner);
                        playersWithTiedHand.push_back(playerCurrent);
                        tiedHandIsBest = 1;
                    }
                    else {
                        playersWithTiedHand.push_back(playerCurrent);
                    }
                }
                stopEvaluating = 0;
            }
        }
        // Print the winner after evaluating the strength values
        if (tiedHandIsBest == 0) {
            if (playerWinner == 0) {
                cout << endl << endl << "You won the hand!" << endl << endl;
            }
            else {
                cout << endl << endl << "Player " << playerWinner+1 << " won the hand!" << endl << endl;
            }
            winnersOut.push_back(playerWinner);
        }
        // If there's still a tie after evaluating the strength values, print the players that are involved in the tie
        // Return a negative value to represent how many players are tied (-2 = 2 players tied)
        else {
            switch (playersWithTiedHand.size()) {
                case 2: 
                    cout << endl << endl << "Players " << playersWithTiedHand[0]+1;
                    cout << " and " << playersWithTiedHand[1]+1 << " are tied!" << endl << endl;
                    break;
                case 3:
                    cout << endl << endl << "Players " << playersWithTiedHand[0]+1;
                    cout << ", " << playersWithTiedHand[1]+1;
                    cout << ", and " << playersWithTiedHand[2]+1 << " are tied!" << endl << endl;
                    break;
                case 4:
                    cout << endl << endl << "Players " << playersWithTiedHand[0]+1;
                    cout << ", " << playersWithTiedHand[1]+1;
                    cout << ", " << playersWithTiedHand[2]+1;
                    cout << ", and " << playersWithTiedHand[3]+1 << " are tied!" << endl << endl;
                    break;
                case 5:
                    cout << endl << endl << "Players " << playersWithTiedHand[0]+1;
                    cout << ", " << playersWithTiedHand[1]+1;
                    cout << ", " << playersWithTiedHand[2]+1;
                    cout << ", " << playersWithTiedHand[3]+1;
                    cout << ", and " << playersWithTiedHand[4]+1 << " are tied!" << endl << endl;
                    break;
                case 6:
                    cout << endl << endl << "Players " << playersWithTiedHand[0]+1;
                    cout << ", " << playersWithTiedHand[1]+1;
                    cout << ", " << playersWithTiedHand[2]+1;
                    cout << ", " << playersWithTiedHand[3]+1;
                    cout << ", " << playersWithTiedHand[4]+1;
                    cout << ", and " << playersWithTiedHand[5]+1 << " are tied!" << endl << endl;
                    break;
                case 7:
                    cout << endl << endl << "Players " << playersWithTiedHand[0]+1;
                    cout << ", " << playersWithTiedHand[1]+1;
                    cout << ", " << playersWithTiedHand[2]+1;
                    cout << ", " << playersWithTiedHand[3]+1;
                    cout << ", " << playersWithTiedHand[4]+1;
                    cout << ", " << playersWithTiedHand[5]+1;
                    cout << ", and " << playersWithTiedHand[6]+1 << " are tied!" << endl << endl;
                    break;
                case 8:
                    cout << endl << endl << "Players " << playersWithTiedHand[0]+1;
                    cout << ", " << playersWithTiedHand[1]+1;
                    cout << ", " << playersWithTiedHand[2]+1;
                    cout << ", " << playersWithTiedHand[3]+1;
                    cout << ", " << playersWithTiedHand[4]+1;
                    cout << ", " << playersWithTiedHand[5]+1;
                    cout << ", " << playersWithTiedHand[6]+1;
                    cout << ", and " << playersWithTiedHand[7]+1 << " are tied!" << endl << endl;
                    break;
                case 9:
                    cout << endl << endl << "Players " << playersWithTiedHand[0]+1;
                    cout << ", " << playersWithTiedHand[1]+1;
                    cout << ", " << playersWithTiedHand[2]+1;
                    cout << ", " << playersWithTiedHand[3]+1;
                    cout << ", " << playersWithTiedHand[4]+1;
                    cout << ", " << playersWithTiedHand[5]+1;
                    cout << ", " << playersWithTiedHand[6]+1;
                    cout << ", " << playersWithTiedHand[7]+1;
                    cout << ", and " << playersWithTiedHand[8]+1 << " are tied!" << endl << endl;
                    break;
                case 10:
                    cout << endl << endl << "Players " << playersWithTiedHand[0]+1;
                    cout << ", " << playersWithTiedHand[1]+1;
                    cout << ", " << playersWithTiedHand[2]+1;
                    cout << ", " << playersWithTiedHand[3]+1;
                    cout << ", " << playersWithTiedHand[4]+1;
                    cout << ", " << playersWithTiedHand[5]+1;
                    cout << ", " << playersWithTiedHand[6]+1;
                    cout << ", " << playersWithTiedHand[7]+1;
                    cout << ", " << playersWithTiedHand[8]+1;
                    cout << ", and " << playersWithTiedHand[9]+1 << " are tied!" << endl << endl;
                    break;
                default: break;
            }
            for (int i = 0; i < playersWithTiedHand.size(); i++) {
                winnersOut.push_back(playersWithTiedHand[i]);
            }
        }
    }

    return winnersOut;
}

void printPokerHand(handtype ht, strength s) {
    switch (ht) {
        case aceHighOrLess:
            switch (s.p1) {
                case two: cout << "2 "; break;
                case three: cout << "3 "; break;
                case four: cout << "4 "; break;
                case five: cout << "5 "; break;
                case six: cout << "6 "; break;
                case seven: cout << "7 "; break;
                case eight: cout << "8 "; break;
                case nine: cout << "9 "; break;
                case ten: cout << "10 "; break;
                case jack: cout << "Jack "; break;
                case queen: cout << "Queen "; break;
                case king: cout << "King "; break;
                case ace: cout << "Ace "; break;
                default: break;
            }
            switch (s.p2) {
                case two: cout << "2 High"; break;
                case three: cout << "3 High"; break;
                case four: cout << "4 High"; break;
                case five: cout << "5 High"; break;
                case six: cout << "6 High"; break;
                case seven: cout << "7 High"; break;
                case eight: cout << "8 High"; break;
                case nine: cout << "9 High"; break;
                case ten: cout << "10 High"; break;
                case jack: cout << "Jack High"; break;
                case queen: cout << "Queen High"; break;
                case king: cout << "King High"; break;
                case ace: cout << "Ace High"; break;
                default: break;
            } break;
        case singlePair:
            switch (s.p1) {
                case two: cout << "Pair of 2's"; break;
                case three: cout << "Pair of 3's"; break;
                case four: cout << "Pair of 4's"; break;
                case five: cout << "Pair of 5's"; break;
                case six: cout << "Pair of 6's"; break;
                case seven: cout << "Pair of 7's"; break;
                case eight: cout << "Pair of 8's"; break;
                case nine: cout << "Pair of 9's"; break;
                case ten: cout << "Pair of 10's"; break;
                case jack: cout << "Pair of Jacks"; break;
                case queen: cout << "Pair of Queens"; break;
                case king: cout << "Pair of Kings"; break;
                case ace: cout << "Pair of Aces"; break;
                default: break;
            }
            switch (s.p2) {
                case two: cout << ", 2 High"; break;
                case three: cout << ", 3 High"; break;
                case four: cout << ", 4 High"; break;
                case five: cout << ", 5 High"; break;
                case six: cout << ", 6 High"; break;
                case seven: cout << ", 7 High"; break;
                case eight: cout << ", 8 High"; break;
                case nine: cout << ", 9 High"; break;
                case ten: cout << ", 10 High"; break;
                case jack: cout << ", Jack High"; break;
                case queen: cout << ", Queen High"; break;
                case king: cout << ", King High"; break;
                case ace: cout << ", Ace High"; break;
                default: break;
            } break;
        case twoPair:
            cout << "Two Pair, ";
            switch (s.p1) {
                case two: cout << "2's"; break;
                case three: cout << "3's"; break;
                case four: cout << "4's"; break;
                case five: cout << "5's"; break;
                case six: cout << "6's"; break;
                case seven: cout << "7's"; break;
                case eight: cout << "8's"; break;
                case nine: cout << "9's"; break;
                case ten: cout << "10's"; break;
                case jack: cout << "Jacks"; break;
                case queen: cout << "Queens"; break;
                case king: cout << "Kings"; break;
                case ace: cout << "Aces"; break;
                default: break;
            }
            cout << " and ";
            switch (s.p2) {
                case two: cout << "2's"; break;
                case three: cout << "3's"; break;
                case four: cout << "4's"; break;
                case five: cout << "5's"; break;
                case six: cout << "6's"; break;
                case seven: cout << "7's"; break;
                case eight: cout << "8's"; break;
                case nine: cout << "9's"; break;
                case ten: cout << "10's"; break;
                case jack: cout << "Jacks"; break;
                case queen: cout << "Queens"; break;
                case king: cout << "Kings"; break;
                case ace: cout << "Aces"; break;
                default: break;
            } break;
        case threeOfKind:
            cout << "Three of a kind with ";
            switch (s.p1) {
                case two: cout << "2's"; break;
                case three: cout << "3's"; break;
                case four: cout << "4's"; break;
                case five: cout << "5's"; break;
                case six: cout << "6's"; break;
                case seven: cout << "7's"; break;
                case eight: cout << "8's"; break;
                case nine: cout << "9's"; break;
                case ten: cout << "10's"; break;
                case jack: cout << "Jacks"; break;
                case queen: cout << "Queens"; break;
                case king: cout << "Kings"; break;
                case ace: cout << "Aces"; break;
                default: break;
            } break;
        case stdStraight:
            cout << "Straight, ";
            switch (s.p1) {
                case two: cout << "2 high"; break;
                case three: cout << "3 high"; break;
                case four: cout << "4 high"; break;
                case five: cout << "5 high"; break;
                case six: cout << "6 high"; break;
                case seven: cout << "7 high"; break;
                case eight: cout << "8 high"; break;
                case nine: cout << "9 high"; break;
                case ten: cout << "10 high"; break;
                case jack: cout << "Jack high"; break;
                case queen: cout << "Queen high"; break;
                case king: cout << "King high"; break;
                case ace: cout << "Ace high"; break;
                default: break;
            } break;
        case stdFlush:
            cout << "Flush, ";
            switch (s.p1) {
                case two: cout << "2 high"; break;
                case three: cout << "3 high"; break;
                case four: cout << "4 high"; break;
                case five: cout << "5 high"; break;
                case six: cout << "6 high"; break;
                case seven: cout << "7 high"; break;
                case eight: cout << "8 high"; break;
                case nine: cout << "9 high"; break;
                case ten: cout << "10 high"; break;
                case jack: cout << "Jack high"; break;
                case queen: cout << "Queen high"; break;
                case king: cout << "King high"; break;
                case ace: cout << "Ace high"; break;
                default: break;
            } break;
        case fullHouse:
            cout << "Full House, ";
            switch (s.p1) {
                case two: cout << "2's"; break;
                case three: cout << "3's"; break;
                case four: cout << "4's"; break;
                case five: cout << "5's"; break;
                case six: cout << "6's"; break;
                case seven: cout << "7's"; break;
                case eight: cout << "8's"; break;
                case nine: cout << "9's"; break;
                case ten: cout << "10's"; break;
                case jack: cout << "Jacks"; break;
                case queen: cout << "Queens"; break;
                case king: cout << "Kings"; break;
                case ace: cout << "Aces"; break;
                default: break;
            }
            cout << " over ";
            switch (s.p2) {
                case two: cout << "2's"; break;
                case three: cout << "3's"; break;
                case four: cout << "4's"; break;
                case five: cout << "5's"; break;
                case six: cout << "6's"; break;
                case seven: cout << "7's"; break;
                case eight: cout << "8's"; break;
                case nine: cout << "9's"; break;
                case ten: cout << "10's"; break;
                case jack: cout << "Jacks"; break;
                case queen: cout << "Queens"; break;
                case king: cout << "Kings"; break;
                case ace: cout << "Aces"; break;
                default: break;
            } break;
        case fourOfKind:
            cout << "Four of a kind with ";
            switch (s.p1) {
                case two: cout << "2's"; break;
                case three: cout << "3's"; break;
                case four: cout << "4's"; break;
                case five: cout << "5's"; break;
                case six: cout << "6's"; break;
                case seven: cout << "7's"; break;
                case eight: cout << "8's"; break;
                case nine: cout << "9's"; break;
                case ten: cout << "10's"; break;
                case jack: cout << "Jacks"; break;
                case queen: cout << "Queens"; break;
                case king: cout << "Kings"; break;
                case ace: cout << "Aces"; break;
                default: break;
            } break;
        case straightFlush:
            cout << "Straight Flush, ";
            switch (s.p1) {
                case two: cout << "2 high"; break;
                case three: cout << "3 high"; break;
                case four: cout << "4 high"; break;
                case five: cout << "5 high"; break;
                case six: cout << "6 high"; break;
                case seven: cout << "7 high"; break;
                case eight: cout << "8 high"; break;
                case nine: cout << "9 high"; break;
                case ten: cout << "10 high"; break;
                case jack: cout << "Jack high"; break;
                case queen: cout << "Queen high"; break;
                case king: cout << "King high"; break;
                case ace: cout << "Ace high"; break;
                default: break;
            } break;
        case royalFlush:
            cout << "Royal Flush"; break;
        default: break;
    }
}

void printNewRoundHeader(int roundNumber) {
    cout << endl << "----------------------------------------" << endl;
    cout << "Round " << roundNumber << endl;
}

// Calculate the value of the hand for determining how the computer should act
int calcHandValue(handtype ht, strength s) {
    int handValue = 0;

    switch (ht) {
        case aceHighOrLess:
            switch (s.p1) {
                case seven: handValue += 10000; break;
                case eight: handValue += 20000; break;
                case nine: handValue += 30000; break;
                case ten: handValue += 40000; break;
                case jack: handValue += 50000; break;
                case queen: handValue += 60000; break;
                case king: handValue += 70000; break;
                case ace: handValue += 80000; break;
                default: break;
            } 
            switch (s.p2) {
                case five: handValue += 1000; break;
                case six: handValue += 2000; break;
                case seven: handValue += 3000; break;
                case eight: handValue += 4000; break;
                case nine: handValue += 5000; break;
                case ten: handValue += 6000; break;
                case jack: handValue += 7000; break;
                case queen: handValue += 8000; break;
                case king: handValue += 9000; break;
                default: break;
            }
            switch (s.p3) {
                case four: handValue += 100; break;
                case five: handValue += 200; break;
                case six: handValue += 300; break;
                case seven: handValue += 400; break;
                case eight: handValue += 500; break;
                case nine: handValue += 600; break;
                case ten: handValue += 700; break;
                case jack: handValue += 800; break;
                case queen: handValue += 900; break;
                default: break;
            }
            switch (s.p4) {
                case three: handValue += 10; break;
                case four: handValue += 20; break;
                case five: handValue += 30; break;
                case six: handValue += 40; break;
                case seven: handValue += 50; break;
                case eight: handValue += 60; break;
                case nine: handValue += 70; break;
                case ten: handValue += 80; break;
                case jack: handValue += 90; break;
                default: break;
            }
            switch (s.p5) {
                case two: handValue += 1; break;
                case three: handValue += 2; break;
                case four: handValue += 3; break;
                case five: handValue += 4; break;
                case six: handValue += 5; break;
                case seven: handValue += 6; break;
                case eight: handValue += 7; break;
                case nine: handValue += 8; break;
                default: break;
            }
            break;
        case singlePair:
            switch (s.p1) {
                case two: handValue += 100000; break;
                case three: handValue += 105000; break;
                case four: handValue += 110000; break;
                case five: handValue += 115000; break;
                case six: handValue += 120000; break;
                case seven: handValue += 125000; break;
                case eight: handValue += 130000; break;
                case nine: handValue += 135000; break;
                case ten: handValue += 140000; break;
                case jack: handValue += 145000; break;
                case queen: handValue += 150000; break;
                case king: handValue += 155000; break;
                case ace: handValue += 160000; break;
                default: break;
            }
            switch (s.p2) {
                case four: handValue += 250; break;
                case five: handValue += 500; break;
                case six: handValue += 750; break;
                case seven: handValue += 1000; break;
                case eight: handValue += 1250; break;
                case nine: handValue += 1500; break;
                case ten: handValue += 1750; break;
                case jack: handValue += 2000; break;
                case queen: handValue += 2250; break;
                case king: handValue += 2500; break;
                case ace: handValue += 2750; break;
                default: break;
            }
            switch (s.p3) {
                case three: handValue += 20; break;
                case four: handValue += 40; break;
                case five: handValue += 60; break;
                case six: handValue += 80; break;
                case seven: handValue += 100; break;
                case eight: handValue += 120; break;
                case nine: handValue += 140; break;
                case ten: handValue += 160; break;
                case jack: handValue += 180; break;
                case queen: handValue += 200; break;
                case king: handValue += 220; break;
                default: break;
            }
            switch (s.p4) {
                case two: handValue += 1; break;
                case three: handValue += 2; break;
                case four: handValue += 3; break;
                case five: handValue += 4; break;
                case six: handValue += 5; break;
                case seven: handValue += 6; break;
                case eight: handValue += 7; break;
                case nine: handValue += 8; break;
                case ten: handValue += 9; break;
                case jack: handValue += 10; break;
                case queen: handValue += 11; break;
                default: break;
            }
            break;
        case twoPair:
            switch (s.p1) {
                case three: handValue += 200000; break;
                case four: handValue += 201000; break;
                case five: handValue += 202000; break;
                case six: handValue += 203000; break;
                case seven: handValue += 204000; break;
                case eight: handValue += 205000; break;
                case nine: handValue += 206000; break;
                case ten: handValue += 207000; break;
                case jack: handValue += 208000; break;
                case queen: handValue += 209000; break;
                case king: handValue += 210000; break;
                case ace: handValue += 211000; break;
                default: break;
            } 
            switch (s.p2) {
                case two: handValue += 20; break;
                case three: handValue += 40; break;
                case four: handValue += 60; break;
                case five: handValue += 80; break;
                case six: handValue += 100; break;
                case seven: handValue += 120; break;
                case eight: handValue += 140; break;
                case nine: handValue += 160; break;
                case ten: handValue += 180; break;
                case jack: handValue += 200; break;
                case queen: handValue += 220; break;
                case king: handValue += 240; break;
                default: break;
            }
            switch (s.p3) {
                case two: handValue += 1; break;
                case three: handValue += 2; break;
                case four: handValue += 3; break;
                case five: handValue += 4; break;
                case six: handValue += 5; break;
                case seven: handValue += 6; break;
                case eight: handValue += 7; break;
                case nine: handValue += 8; break;
                case ten: handValue += 9; break;
                case jack: handValue += 10; break;
                case queen: handValue += 11; break;
                case king: handValue += 12; break;
                case ace: handValue += 13; break;
                default: break;
            } 
            break;
        case threeOfKind:
            switch (s.p1) {
                case two: handValue += 220000; break;
                case three: handValue += 221000; break;
                case four: handValue += 222000; break;
                case five: handValue += 223000; break;
                case six: handValue += 224000; break;
                case seven: handValue += 225000; break;
                case eight: handValue += 226000; break;
                case nine: handValue += 227000; break;
                case ten: handValue += 228000; break;
                case jack: handValue += 229000; break;
                case queen: handValue += 230000; break;
                case king: handValue += 231000; break;
                case ace: handValue += 232000; break;
                default: break;
            } 
            switch (s.p2) {
                case three: handValue += 20; break;
                case four: handValue += 40; break;
                case five: handValue += 60; break;
                case six: handValue += 80; break;
                case seven: handValue += 100; break;
                case eight: handValue += 120; break;
                case nine: handValue += 140; break;
                case ten: handValue += 160; break;
                case jack: handValue += 180; break;
                case queen: handValue += 200; break;
                case king: handValue += 220; break;
                case ace: handValue += 240; break;
                default: break;
            }
            switch (s.p3) {
                case two: handValue += 1; break;
                case three: handValue += 2; break;
                case four: handValue += 3; break;
                case five: handValue += 4; break;
                case six: handValue += 5; break;
                case seven: handValue += 6; break;
                case eight: handValue += 7; break;
                case nine: handValue += 8; break;
                case ten: handValue += 9; break;
                case jack: handValue += 10; break;
                case queen: handValue += 11; break;
                case king: handValue += 12; break;
                default: break;
            }
            break;
        case stdStraight:
            switch (s.p1) {
                case five: handValue += 240000; break;
                case six: handValue += 241000; break;
                case seven: handValue += 242000; break;
                case eight: handValue += 243000; break;
                case nine: handValue += 244000; break;
                case ten: handValue += 245000; break;
                case jack: handValue += 246000; break;
                case queen: handValue += 247000; break;
                case king: handValue += 248000; break;
                case ace: handValue += 249000; break;
                default: break;
            }
            break;
        case stdFlush:
            switch (s.p1) {
                case seven: handValue += 250000; break;
                case eight: handValue += 260000; break;
                case nine: handValue += 270000; break;
                case ten: handValue += 280000; break;
                case jack: handValue += 290000; break;
                case queen: handValue += 300000; break;
                case king: handValue += 310000; break;
                case ace: handValue += 320000; break;
                default: break;
            } 
            switch (s.p2) {
                case five: handValue += 1000; break;
                case six: handValue += 2000; break;
                case seven: handValue += 3000; break;
                case eight: handValue += 4000; break;
                case nine: handValue += 5000; break;
                case ten: handValue += 6000; break;
                case jack: handValue += 7000; break;
                case queen: handValue += 8000; break;
                case king: handValue += 9000; break;
                default: break;
            }
            switch (s.p3) {
                case four: handValue += 100; break;
                case five: handValue += 200; break;
                case six: handValue += 300; break;
                case seven: handValue += 400; break;
                case eight: handValue += 500; break;
                case nine: handValue += 600; break;
                case ten: handValue += 700; break;
                case jack: handValue += 800; break;
                case queen: handValue += 900; break;
                default: break;
            }
            switch (s.p4) {
                case three: handValue += 10; break;
                case four: handValue += 20; break;
                case five: handValue += 30; break;
                case six: handValue += 40; break;
                case seven: handValue += 50; break;
                case eight: handValue += 60; break;
                case nine: handValue += 70; break;
                case ten: handValue += 80; break;
                case jack: handValue += 90; break;
                default: break;
            }
            switch (s.p5) {
                case two: handValue += 1; break;
                case three: handValue += 2; break;
                case four: handValue += 3; break;
                case five: handValue += 4; break;
                case six: handValue += 5; break;
                case seven: handValue += 6; break;
                case eight: handValue += 7; break;
                case nine: handValue += 8; break;
                default: break;
            }
            break;
        case fullHouse:
            switch (s.p1) {
                case two: handValue += 350000; break;
                case three: handValue += 350100; break;
                case four: handValue += 350200; break;
                case five: handValue += 350300; break;
                case six: handValue += 350400; break;
                case seven: handValue += 350500; break;
                case eight: handValue += 350600; break;
                case nine: handValue += 350700; break;
                case ten: handValue += 350800; break;
                case jack: handValue += 350900; break;
                case queen: handValue += 351000; break;
                case king: handValue += 351100; break;
                case ace: handValue += 351200; break;
                default: break;
            } 
            switch (s.p2) {
                case two: handValue += 1; break;
                case three: handValue += 2; break;
                case four: handValue += 3; break;
                case five: handValue += 4; break;
                case six: handValue += 5; break;
                case seven: handValue += 6; break;
                case eight: handValue += 7; break;
                case nine: handValue += 8; break;
                case ten: handValue += 9; break;
                case jack: handValue += 10; break;
                case queen: handValue += 11; break;
                case king: handValue += 12; break;
                case ace: handValue += 13; break;
                default: break;
            } break;
        case fourOfKind:
            switch (s.p1) {
                case two: handValue += 360000; break;
                case three: handValue += 360100; break;
                case four: handValue += 360200; break;
                case five: handValue += 360300; break;
                case six: handValue += 360400; break;
                case seven: handValue += 360500; break;
                case eight: handValue += 360600; break;
                case nine: handValue += 360700; break;
                case ten: handValue += 360800; break;
                case jack: handValue += 360900; break;
                case queen: handValue += 361000; break;
                case king: handValue += 361100; break;
                case ace: handValue += 361200; break;
                default: break;
            } 
            switch (s.p2) {
                case two: handValue += 1; break;
                case three: handValue += 2; break;
                case four: handValue += 3; break;
                case five: handValue += 4; break;
                case six: handValue += 5; break;
                case seven: handValue += 6; break;
                case eight: handValue += 7; break;
                case nine: handValue += 8; break;
                case ten: handValue += 9; break;
                case jack: handValue += 10; break;
                case queen: handValue += 11; break;
                case king: handValue += 12; break;
                case ace: handValue += 13; break;
                default: break;
            } break;
        case straightFlush:
            switch (s.p1) {
                case five: handValue += 370100; break;
                case six: handValue += 370200; break;
                case seven: handValue += 370300; break;
                case eight: handValue += 370400; break;
                case nine: handValue += 370500; break;
                case ten: handValue += 370600; break;
                case jack: handValue += 370700; break;
                case queen: handValue += 370800; break;
                case king: handValue += 370900; break;
                case ace: handValue += 371000; break;
                default: break;
            } break;
        case royalFlush: handValue += 380000; break;
    default: break;
    }

    return handValue;
};

// Calculate the action that the computer will make (check/call, bet/raise with amount, or fold)
computerAction calcCompAction(vector<card> hand, int chipStack, int betSize, int gameDiff) {
    computerAction ca;
    handtype ht;
    strength s;
    float chipRatio;
    int handValue;

    // Calculate the ratio between the current bet size and the player's chip size
    if (chipStack > 0) {
        chipRatio = (float)betSize / (float)chipStack;
    }
    else {
        chipRatio = betSize;
    }

    // Calculate value of hand based on handtype and strength
    ht = findHandType(hand);
    s = findHandStrength(hand, ht);
    handValue = calcHandValue(ht, s);

    /* Game difficulty 1 behavior
    chipRatio = 0%
        check if handtype is single pair or less
        bet 10% of chipStack if two pair
        bet 20% of chipStack if three of kind
        go all in if higher than three of kind
    chipRatio = 0% to 20%
        fold if handtype is ace high or less
        call if single pair
        raise 10% of betSize if two pair
        raise 30% of betSize if three of kind
        go all in if higher than three of kind
    chipRatio = 20% to 50%
        fold if handtype is single pair or less
        call if two pair
        raise 20% of betSize if three of kind
        go all in if higher than three of kind
    chipRatio = 50% to 100%
        fold if handtype is two pair or less
        call for all other handtypes
    */
    switch (gameDiff) {
        case 1:
            if (chipRatio == 0) {
                if (ht <= singlePair) {
                    ca.a = checkCall;
                }
                else if (ht == twoPair) {
                    ca.a = betRaise;
                    ca.betSize = 0.1 * (float)chipStack;
                }
                else if (ht == threeOfKind) {
                    ca.a = betRaise;
                    ca.betSize = chipStack + (0.2 * (float)chipStack);
                }
                else {  // GO ALL IN
                    ca.a = betRaise;
                    ca.betSize = chipStack;
                }
            }
            else if ((chipRatio > 0) && (chipRatio <= 0.2)) {
                if (ht == aceHighOrLess) {
                    ca.a = fold;
                }
                else if (ht == singlePair) {
                    ca.a = checkCall;
                }
                else if (ht == twoPair) {
                    ca.a = betRaise;
                    ca.betSize = 0.1 * (float)betSize;
                }
                else if (ht == threeOfKind) {
                    ca.a = betRaise;
                    ca.betSize = 0.3 * (float)betSize;
                }
                else {  // GO ALL IN
                    ca.a = betRaise;
                    ca.betSize = chipStack;
                }
            }
            else if ((chipRatio > 0.2) && (chipRatio <= 0.5)) {
                if (ht <= singlePair) {
                    ca.a = fold;
                }
                else if (ht == twoPair) {
                    ca.a = checkCall;
                }
                else if (ht == threeOfKind) {
                    ca.a = betRaise;
                    ca.betSize = 0.2 * (float)betSize;
                }
                else {  // GO ALL IN
                    ca.a = betRaise;
                    ca.betSize = chipStack;
                }
            }
            else {
                if (ht <= twoPair) {
                    ca.a = fold;
                }
                else {
                    ca.a = checkCall;
                }
            }
            break;
        default: break;
    }

    return ca;
}