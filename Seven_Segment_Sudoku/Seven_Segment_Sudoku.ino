
// Seven Segment Sudoku - Hari Wiguna, 2016

// [x] keypad handling
// [ ] play game
//  [ ] Select Mode
//  [ ] marching ants
//  [ ] Select Ternary
//  [ ] Select Cell
//  [ ] Set Digit in Cell

//A - After (show original and guessed numbers)
//B - Before (show only original numbers)
//C - 
//D - 
//0 - Erase
//* - Pick Cell mode (toggles)
//# -
//
//Generate puzzle
//show puzzle
//quit
//reset
//check win
//fanfare
//help modes
//enter puzzle
//enter possibles
//show possibles
//Dimming
//save puzzle state to flash
//Show bad move


// v0.05 - Remove Column shift register.
//           Code now works with PCB version. digitBits is now inverted 180 degrees so the decimal point is at top.
// v0.04 - Draw one digit per interrupt.
//         Drawing the whole column (9 vertical digits) took longer than interrupt cycle.
//         Now we leave the column alone while we draw a new row for the upcoming column at each interrupt.
//         When we're done with all the rows, we turn off the column, slam the segments to the display, turn on the correct column.

// v0.03 - Use POV (Persistence Of Vision) to draw all digits "simultaneously".
//         Refresh function that is periodically called by interrupt draws the sudoku independent of main loop.

// v0.02 - Create an array to represent Sudoku grid and display it.

// v0.01 - Display actual digits 1..9

// v0.00 - Proof that we could control every segment of NINE digits.
//        One Shift Register drives the segments + decimal point.
//        Another Shift Register drives 8 columns (digits).
//        D8 drives the 9th column (digit).

//== Arduino Pin configuration ==
const byte COL_9_PIN = 10; // D10..D2 controls column 0..9

const byte SEG_CLK_PIN = 11;
const byte SEG_LATCH_PIN = 12;
const byte SEG_SER_PIN = 13;

#include "Game.h"
#include "Board.h"
#include "Screen.h"
#include "Keypad.h"
#include "Setup.h"
#include "Loop.h"


