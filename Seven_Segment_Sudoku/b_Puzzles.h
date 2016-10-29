const byte sudokus[][9] PROGMEM = {
  {0, 0, 0, 1, 0, 0, 0, 3, 0}, // Easy?
  {8, 0, 5, 0, 4, 0, 0, 6, 0},
  {3, 2, 0, 0, 0, 0, 0, 0, 5},
  {7, 0, 0, 0, 0, 6, 0, 5, 8},
  {0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 6, 0, 5, 1, 0, 0, 0, 0},
  {2, 0, 0, 0, 0, 0, 0, 0, 7},
  {0, 4, 9, 0, 5, 2, 0, 0, 1},
  {0, 0, 8, 0, 0, 0, 0, 0, 4},

  {0, 0, 0, 0, 0, 0, 8, 0, 0}, // Very Easy 0190 Sep 10,2016
  {0, 9, 8, 7, 0, 4, 3, 0, 0},
  {0, 6, 0, 8, 2, 0, 7, 1, 9},
  {0, 0, 0, 0, 7, 6, 9, 0, 0},
  {5, 0, 0, 0, 3, 0, 6, 4, 0},
  {0, 3, 6, 9, 0, 0, 1, 5, 0},
  {0, 8, 4, 3, 0, 0, 2, 0, 0},
  {3, 0, 0, 6, 9, 2, 0, 8, 1},
  {0, 0, 0, 0, 0, 0, 0, 0, 0},

  {0, 0, 3, 0, 2, 0, 7, 0, 0}, // Very Easy 0909 Sep 9
  {6, 0, 0, 0, 3, 5, 0, 9, 1},
  {0, 1, 7, 0, 0, 9, 8, 0, 0},
  {0, 7, 0, 0, 5, 0, 0, 8, 0},
  {0, 0, 4, 0, 6, 0, 9, 0, 5},
  {0, 0, 0, 0, 0, 7, 1, 0, 2},
  {7, 0, 1, 2, 0, 6, 0, 4, 8},
  {0, 0, 5, 3, 7, 0, 0, 0, 0},
  {8, 9, 2, 5, 1, 0, 0, 0, 0}
  //
  //  {0, 0, 0, 0, 0, 0, 0, 0, 0},
  //  {0, 0, 0, 0, 0, 0, 0, 0, 0},
  //  {0, 0, 0, 0, 0, 0, 0, 0, 0},
  //  {0, 0, 0, 0, 0, 0, 0, 0, 0},
  //  {0, 0, 0, 0, 0, 0, 0, 0, 0},
  //  {0, 0, 0, 0, 0, 0, 0, 0, 0},
  //  {0, 0, 0, 0, 0, 0, 0, 0, 0},
  //  {0, 0, 0, 0, 0, 0, 0, 0, 0},
  //  {0, 0, 0, 0, 0, 0, 0, 0, 0},
};

//-------------------------------------------------------

//== Numbers on Sudoku Board ==
volatile byte sudoku[9][9];

//volatile byte sudoku[][9] = { // Test Pattern
//  {1, 2, 3, 0, 0, 6, 7, 0, 9},
//  {2, 3, 4, 5, 6, 7, 8, 9, 1},
//  {3, 4, 5, 6, 7, 8, 9, 1, 2},
//
//  {4, 5, 6, 7, 8, 9, 1, 2, 3},
//  {5, 6, 7, 8, 9, 1, 2, 3, 4},
//  {6, 7, 8, 9, 1, 2, 3, 4, 5},
//
//  {7, 8, 9, 1, 2, 3, 4, 5, 6},
//  {8, 9, 1, 2, 3, 4, 5, 6, 7},
//  {9, 1, 2, 3, 4, 5, 6, 7, 8}
//};

//-------------------------------------------------------

void PrintSudoku()
{
  Serial.println();
  for (byte r = 0; r < 9; r++)
  {
    for (byte c = 0; c < 9; c++) {
      if (sudoku[r][c] == 0) Serial.print("."); else Serial.print( sudoku[r][c], DEC);
      Serial.print(" ");
    }
    Serial.println();
  }
}

void Swap(byte *a, byte *b)
{
  byte t = *a;
  *a = *b;
  *b = t;
}

void SudokuHFlip()
{
  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 4; c++) // horizontal swap left four with right four (middle stays)
      Swap(&(sudoku[r][c]), &(sudoku[r][8 - c]));
}

void SudokuVFlip()
{
  for (byte c = 0; c < 9; c++)
    for (byte r = 0; r < 4; r++) // vertical swap top four with bottom four (middle stays)
      Swap(&(sudoku[r][c]), &(sudoku[8 - r][c]));
}

void SudokuRotateClockwise()
{
  for (byte i = 0; i < 4; i++) {
    for (byte j = i; j < (8 - i); j++) {
      byte t = sudoku[i][j]; // Save top
      sudoku[i][j] = sudoku[8 - j][i]; // replace top with left
      sudoku[8 - j][i] = sudoku[8 - i][8 - j]; // replace left with bottom
      sudoku[8 - i][8 - j] = sudoku[j][8 - i]; // replace bottom with right
      sudoku[j][8 - i] = t; // replace right with top
    }
  }
}

void SudokuJumble()
{
  byte translation[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (byte i = 0; i < 9; i++) {
    byte randomIndex = random(0, 9);
    Swap(&translation[i], &(translation[randomIndex]));
  }

  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 9; c++)
      if (sudoku[r][c] != 0) {
        byte originalDigit = sudoku[r][c];
        sudoku[r][c] = translation[originalDigit - 1];
      }
}

void SudokuGenerate()
{
  noInterrupts(); // This takes time, so make sure refresh won't try to get the puzzle while we're generating it.

  // Randomly pick a starting puzzle
  int puzzleMax = sizeof(sudokus) / 81;
  int randomPuzzle = random(0, puzzleMax);
  Serial.println(randomPuzzle);
  memcpy_P (sudoku, &sudokus[randomPuzzle * 9][0], sizeof(byte) * 81);

  SudokuJumble();

  // Randomly flip horizontally, vertically, rotate
  if (random(0, 1)) SudokuHFlip();
  if (random(0, 1)) SudokuVFlip();
  for (byte r = 0; r < random(0,4); r++)
    SudokuRotateClockwise();

  //PrintSudoku();

  interrupts();
}




