//== Numbers on Sudoku Board ==
volatile byte sudoku[][9] = { // Easy Puzzle
  {0, 0, 0, 1, 0, 0, 0, 3, 0},
  {8, 0, 5, 0, 4, 0, 0, 6, 0},
  {3, 2, 0, 0, 0, 0, 0, 0, 5},

  {7, 0, 0, 0, 0, 6, 0, 5, 8},
  {0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 6, 0, 5, 1, 0, 0, 0, 0},

  {2, 0, 0, 0, 0, 0, 0, 0, 7},
  {0, 4, 9, 0, 5, 2, 0, 0, 1},
  {0, 0, 8, 0, 0, 0, 0, 0, 4},
};

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

//== high four bits of sudoku array ==
#define IS_PUZZLE_BIT   4 // Set if cell is defined in original puzzle
#define IS_VISIBLE_BIT  5 // Set when cell is displayed
#define IS_WRONG_BIT    6 // Set when cell value breaks Sudoku rule
#define IS_DOT_ON_BIT   7 // Set when we want the decimal point to be on

volatile int8_t gRow = 0;
volatile int8_t gCol = 0;
volatile byte refreshRate = 2; // Higher = slower refresh
volatile byte refreshCounter = 0;

//-----------------------------------------------------------------

//== Functions ==

void SetupBoard()
{
  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 9; c++) {
      if (sudoku[r][c] != 0) bitSet(sudoku[r][c], IS_PUZZLE_BIT); // Set cell's "Is Puzzle bit" on cells containing the original puzzle digits
      bitSet(sudoku[r][c], IS_VISIBLE_BIT); // Set all cells to be visible.
    }
}

byte GetDigit(byte cellValue)
{
  return cellValue & B00001111; // Low 4 bits are the actual digit of that cell
}

void SetDot(byte row, byte col, byte state)
{
  if (state) bitSet(sudoku[row][col], IS_DOT_ON_BIT); else bitClear(sudoku[row][col], IS_DOT_ON_BIT);
}

void ClearSelection()
{
  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 9; c++)
      SetDot(r, c, 0); //dots[r][c] = 0;
}

byte GetSelectedRow(byte selectedBox, byte selectedCell)
{
  byte boxRowOffset = (selectedBox / 3) * 3;
  byte cellRowOffset = (selectedCell / 3);
  return boxRowOffset + cellRowOffset;
}

byte GetSelectedCol(byte selectedBox, byte selectedCell)
{
  byte boxColOffset = (selectedBox % 3) * 3;
  byte cellColOffset = (selectedCell % 3);
  return boxColOffset + cellColOffset;
}

byte GetSelectedCell(byte selectedBox, byte selectedCell)
{
  byte row = GetSelectedRow(selectedBox, selectedCell);
  byte col = GetSelectedCol(selectedBox, selectedCell);
  byte cellValue = sudoku[row][col];
  return cellValue;
}

void SetSelectedCell(byte selectedBox, byte selectedCell, byte cellValue)
{
  byte row = GetSelectedRow(selectedBox, selectedCell);
  byte col = GetSelectedCol(selectedBox, selectedCell);
  sudoku[row][col] = cellValue;
}

void SetDigit(byte selectedBox, byte selectedCell, byte selectedDigit)
{
  byte cellValue = GetSelectedCell(selectedBox, selectedCell);
  if (!bitRead(cellValue, IS_PUZZLE_BIT)) {
    byte newCellValue = (cellValue & B11110000) | selectedDigit;
    SetSelectedCell(selectedBox, selectedCell, newCellValue);
  }
}

void ClearIsWrongs()
{
  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 9; c++) {
      bitClear(sudoku[r][c], IS_WRONG_BIT);
    }
}

void MarkAsWrong(byte row, byte col)
{
  bitSet(sudoku[row][col], IS_WRONG_BIT);
}

void ValidateCell(byte row, byte col)
{
  ClearIsWrongs();

  bool hasWrong = false;
  byte guessDigit = GetDigit(sudoku[row][col]);

  //-- Mark duplicate digits on each row --
  for (byte c = 0; c < 9; c++) // Loop through all the columns of that row...
  {
    // Is it a duplicate of the cell we're validating?
    if ( c != col && GetDigit(sudoku[row][c]) == guessDigit) {
      MarkAsWrong(row, c);
      hasWrong = true;
    }
  }

  //-- Mark duplicate digits on each column --
  for (byte r = 0; r < 9; r++) // Loop through all the rows of that column...
  {
    // Is it a duplicate of the cell we're validating?
    if ( r != row && GetDigit(sudoku[r][col]) == guessDigit) {
      MarkAsWrong(r, col);
      hasWrong = true;
    }
  }

  //-- Mark duplicate digits on each box --
  byte r0 = (row / 3) * 3;
  byte c0 = (col / 3) * 3;
  for (byte r1 = 0; r1 < 3; r1++)
    for (byte c1 = 0; c1 < 3; c1++)
    {
      byte r = r0 + r1;
      byte c = c0 + c1;
      // Is it a duplicate of the cell we're validating?
      if ( r != row && c != col && GetDigit(sudoku[r][c]) == guessDigit) {
        MarkAsWrong(r, c);
        hasWrong = true;
      }
    }

  if (hasWrong) MarkAsWrong(row, col);
}
