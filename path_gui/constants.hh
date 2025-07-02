#ifndef CONSTANTS_HH
#define CONSTANTS_HH

#include "gameboard.hh"

const int BUTTON_WIDTH = 50;
const int BUTTON_HEIGHT = 50;
const int DELAY = 1000;
const Point START_AT = {100, 100};
const char LEGAL_MOVE[30] = "IsMoveLegal (yes/no): yes";
const char NOT_LEGAL_MOVE[30] = "IsMoveLegal (yes/no): no";
const char EMPTY_LEGAL_MOVE[30] = "IsMoveLegal (yes/no): ";
const char EMPTY_POINTS[8] = "Point: ";
const char EMPTY_MOVES[15] = "MovesSpent: ";
const char FALSE_PAUSE_TEXT[17] = "isPaused: false";
const char TRUE_PAUSE_TEXT[16] = "isPaused: true";
const char HAS_NOT_WON[19] = "haveYouWon: false";
const char HAS_WON[18] = "haveYouWon: true";

#endif // CONSTANTS_HH
