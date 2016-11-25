
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

void SaveSudoku()
{
  //-- Let's not save the board when we have wrong digits on them --
  bool hasWrong = false;
  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 9; c++) {
      if (bitRead(sudoku[r][c], IS_WRONG_BIT)) hasWrong = true;
      bitClear(sudoku[r][c], IS_DOT_ON_BIT); // Let's not save the dot either.
    }

  if (!hasWrong)
    EEPROM.put(0, sudoku);
}

void LoadSudoku()
{
  EEPROM.get(0, sudoku);
}

void SetupBoard()
{
  SudokuGenerate();

  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 9; c++) {
      if (sudoku[r][c] != 0) bitSet(sudoku[r][c], IS_PUZZLE_BIT); // Set cell's "Is Puzzle bit" on cells containing the original puzzle digits
      bitSet(sudoku[r][c], IS_VISIBLE_BIT); // Set all cells to be visible.
    }

  SaveSudoku();
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

void RemoveWrong()
{
  byte r = GetSelectedRow(selectedBox, selectedCell);
  byte c = GetSelectedCol(selectedBox, selectedCell);
  if (bitRead(sudoku[r][c], IS_WRONG_BIT)) sudoku[r][c] = 0;
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

void MarkRow(byte row, byte digit)
{
  for (byte c = 0; c < 9; c++)
    if (GetDigit(sudoku[row][c]) == 0) sudoku[row][c] = 10;
}

void MarkCol(byte col, byte digit)
{
  for (byte r = 0; r < 9; r++)
    if (GetDigit(sudoku[r][col]) == 0) sudoku[r][col] = 10;
}


void MarkBox(byte r0, byte c0, byte digit)
{
  for (byte r1 = 0; r1 < 3; r1++)
    for (byte c1 = 0; c1 < 3; c1++)
    {
      byte r = r0 + r1;
      byte c = c0 + c1;
      if (GetDigit(sudoku[r][c]) == 0) sudoku[r][c] = 10;
    }
}

void ClearAssists()
{
  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 9; c++) {
      if (GetDigit(sudoku[r][c]) == 10 || bitRead(sudoku[r][c], IS_WRONG_BIT)) sudoku[r][c] = 0;
    }
}

void Clear()
{
  ClearAssists();
  ClearSelection();
}

void Assist(byte selectedDigit)
{
  ClearAssists();

  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 9; c++)
    {
      byte cellDigit = GetDigit(sudoku[r][c]);
      if (cellDigit == selectedDigit) {
        byte r0 = (r / 3) * 3;
        byte c0 = (c / 3) * 3;
        MarkRow(r, selectedDigit);
        MarkCol(c, selectedDigit);
        MarkBox(r0, c0, selectedDigit);
      }
    }

  //-- Mark what's left as "wrong", so we could undo them later --
  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 9; c++)
    {
      byte cellDigit = GetDigit(sudoku[r][c]);
      if (cellDigit == 0) {
        sudoku[r][c] = selectedDigit;
        MarkAsWrong(r, c);
      }
    }
}

void RedoGame()
{
  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 9; c++)
    {
      byte cellDigit = GetDigit(sudoku[r][c]);
      if (bitRead(sudoku[r][c], IS_PUZZLE_BIT))
      {
        sudoku[r][c] = cellDigit;
        bitSet(sudoku[r][c], IS_PUZZLE_BIT);
        bitSet(sudoku[r][c], IS_VISIBLE_BIT);
      }
      else
        sudoku[r][c] = 0;
    }
  SaveSudoku();
}

void NewGame()
{
  SetupBoard();
}


void ClearText()
{
  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 9; c++)
      sudoku[r][c] = '0'; // 0 = blank space
}

void DrawText(byte row, byte col, String txt)
{
  for (byte c=0; c<txt.length(); c++)
  {
    sudoku[row][col+c] = txt[c];
  }
}

