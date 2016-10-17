
//== Numbers on Sudoku Board ==
volatile byte sudoku[][9] = {
  {1, 2, 3, 0, 0, 6, 7, 0, 9},
  {2, 3, 4, 5, 6, 7, 8, 9, 1},
  {3, 4, 5, 6, 7, 8, 9, 1, 2},

  {4, 5, 6, 7, 8, 9, 1, 2, 3},
  {5, 6, 7, 8, 9, 1, 2, 3, 4},
  {6, 7, 8, 9, 1, 2, 3, 4, 5},

  {7, 8, 9, 1, 2, 3, 4, 5, 6},
  {8, 9, 1, 2, 3, 4, 5, 6, 7},
  {9, 1, 2, 3, 4, 5, 6, 7, 8}
};

//== Dot states on Sudoku Board ==
volatile byte dots[9][9];

//== Functions ==

void TopLeftDots()
{
  for (byte r = 0; r < 3; r++)
    for (byte c = 0; c < 3; c++)
      dots[r][c] = 1;
}

void ClearDots()
{
  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 9; c++)
      dots[r][c] = 0;
}



void SegmentTest()
{
  //== Scan through all the segments ===
  for (byte seg = 0; seg < 8; seg++)
  {
    byte segByte = 0;
    bitSet(segByte, seg);

    digitalWrite(SEG_LATCH_PIN, LOW);
    shiftOut(SEG_SER_PIN, SEG_CLK_PIN, MSBFIRST, ~segByte);
    digitalWrite(SEG_LATCH_PIN, HIGH);

    delay(100);
  }
}

void EraseAll()
{
  for (byte r = 0; r < 9; r++)
  {
    for (byte c = 0; c < 9; c++)
    {
      sudoku[r][c] = 0;
    }
  }
}


void FillRandomly()
{
  for (byte r = 0; r < 9; r++)
  {
    for (byte c = 0; c < 9; c++)
    {
      sudoku[c][r] = random(0, 10);
    }
  }
  sudoku[0][0] = 7; 
  sudoku[1][0] = 7; 
  sudoku[2][0] = 7; 
}

void FillVertically()
{
  byte n = 1;
  for (byte c = 0; c < 9; c++)
  {
    delay(100);
    for (byte r = 0; r < 9; r++)
    {
      sudoku[r][c] = c + 1;
    }
  }
}

void FillHorizontally()
{
  for (byte c = 0; c < 9; c++)
  {
    delay(100);
    sudoku[0][c] = c + 1;
    sudoku[1][8 - c] = c + 1;
    sudoku[2][c] = c + 1;
  }
}

void FillSudoku1()
{
  for (byte t = 0; t < 3; t++)
  {
    for (byte c = 0; c < 9; c++)
    {
      //delay(100);
      sudoku[t * 3 + 0][c] = c + 1;
      sudoku[t * 3 + 1][c] = ((c + 3) % 9) + 1;
      sudoku[t * 3 + 2][c] = ((c + 6) % 9) + 1;
    }
  }
}

void FillAll(byte number)
{
  for (byte t = 0; t < 9; t++)
  {
    for (byte c = 0; c < 9; c++)
    {
      sudoku[t][c] = number;
    }
  }
}


void FillCount()
{
  for (byte i = 1; i <= 9; i++)
  {
    for (byte t = 0; t < 9; t++)
      for (byte c = 0; c < 9; c++)
        sudoku[t][c] = i;
    delay(200);
  }
}

