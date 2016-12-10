const char sudokus[][81] PROGMEM = {
  "...1...3."
  "8.5.4..6."
  "32......5"
  "7....6.58"
  "........."
  ".6.51...."
  "2.......7"
  ".49.52..1"
  "..8.....4",

  "......8.." // Very Easy 0190 Sep 10,201
  ".987.43.."
  ".6.82.719"
  "....769.."
  "5...3.64."
  ".369..15."
  ".843..2.."
  "3..692.81"
  ".........",

  "..3.2.7.." // Very Easy 0909 Sep 9
  "6...35.91"
  ".17..98.."
  ".7..5..8."
  "..4.6.9.5"
  ".....71.2"
  "7.12.6.48"
  "..537...."
  "89251....",
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
  char puzzle[82];
  memcpy_P (puzzle, sudokus[randomPuzzle], 81);
  for (byte i = 0; i < 81; i++)
  {
    byte r = i / 9;
    byte c = i % 9;
    sudoku[r][c] = puzzle[i] == '.' ? 0 : puzzle[i] - '0';
  }

  // Randomly flip vertically, horizontally, rotate
  if (random(0, 2)) SudokuVFlip();
  if (random(0, 2)) SudokuHFlip();
  for (byte r = 0; r < random(0,4); r++)
    SudokuRotateClockwise();
  
  SudokuJumble();
  
  interrupts();
}




