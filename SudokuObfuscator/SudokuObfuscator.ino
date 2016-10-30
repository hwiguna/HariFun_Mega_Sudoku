#include <avr/pgmspace.h>

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

volatile byte sudoku[9][9];

void PrintSudoku()
{
  Serial.println();
  for (byte r = 0; r < 9; r++)
  {
    for (byte c = 0; c < 9; c++) {
      Serial.print( sudoku[r][c], DEC);
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
  randomSeed(analogRead(0));

  // Randomly pick a starting puzzle
  int puzzleMax = sizeof(sudokus) / 81;
  int randomPuzzle = random(0, puzzleMax);
  char puzzle[82];
  memcpy_P (puzzle, sudokus[randomPuzzle], 81);
  //  puzzle[81] = 0;
  //  Serial.println(puzzle);
  for (byte i = 0; i < 81; i++)
  {
    byte r = i / 9;
    byte c = i % 9;
    sudoku[r][c] = puzzle[i] == '.' ? 0 : puzzle[i] - '0';
  }

  //SudokuJumble();

  // Randomly flip horizontally, vertically, rotate
  //  if (random(0, 1)) SudokuHFlip();
  //  if (random(0, 1)) SudokuVFlip();
  //  for (byte r = 0; r < random(4); r++)
  //    SudokuRotateClockwise();
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  SudokuGenerate();
  PrintSudoku();
}

void loop() {
  // put your main code here, to run repeatedly:

}
