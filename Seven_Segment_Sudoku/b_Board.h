
//== Numbers on Sudoku Board ==
//volatile byte sudoku[][9] = {
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

volatile byte sudoku[][9] = {
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


//== Dot states on Sudoku Board ==
#define FLAG_IS_ORIGINAL 4
#define FLAG_IS_WRONG 5
#define FLAG_IS_SELECTED 6
//#define FLAG_I_HAVE_NOT_THOUGHT_OF_A_USE_FOR_THIS_BIT 7
volatile byte dots[9][9];

//== Marching ant animation ==
volatile byte antOffset = 0; // 0,1,2 then it cycles back to 0
volatile unsigned long timeToMove = 10;
volatile byte isOn;

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



// Outer Perimeter
volatile byte blinkBits[][2] = {
  {0, 1},
  {0, 0},
  {1, 0},
  {2, 0},
  {2, 1},
  {2, 2},
  {1, 2},
  {0, 2},
};

//volatile byte blinkBits[][2] = {
//  {1, 0},
//  {2, 0},
//  {2, 1},
//  {2, 2},
//  {1, 2},
//  {0, 2},
//  {0, 1},
//  {0, 0}
//};

//volatile byte blinkBits[][4] = {
//  {1,0, 1,0},
//  {0,0, 2,0},
//  {0,1, 2,1},
//  {0,2, 2,2},
//  {1,2, 1,2}
//};
volatile byte animMax = sizeof(blinkBits) / (sizeof(byte) * 2);

volatile int8_t gRow = 0;
volatile int8_t gCol = 0;
volatile byte refreshRate = 2; // Higher = slower refresh
volatile byte refreshCounter = 0;

//-----------------------------------------------------------------

void PleaseSelectBox()
{
  for (byte i = 0; i < 9; i++) {
    dots[0][i] = 1;
    dots[8][i] = 1;
    dots[i][0] = 1;
    dots[i][8] = 1;
  }
}

void ClearSelection()
{
  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 9; c++)
      dots[r][c] = 0;
}

void PleaseSelectCell_Wipe()
{
  if (--timeToMove == 0)
  {
    byte offset = antOffset;
    byte colOffset = (selectedBox % 3) * 3;
    byte rowOffset = (selectedBox / 3) * 3;
    byte c = colOffset + blinkBits[offset][0];
    byte r = rowOffset + blinkBits[offset][1];
    dots[c][r] = isOn;
    if (antOffset++ > animMax) {
      antOffset = 0;
      isOn = 1 - isOn;
    }

    timeToMove = 6;
  }
}
void PleaseSelectCell_Wipe2()
{
  if (--timeToMove == 0)
  {
    byte offset = antOffset;
    byte colOffset = (selectedBox % 3) * 3;
    byte rowOffset = (selectedBox / 3) * 3;
    byte x1 = colOffset + blinkBits[offset][0];
    byte y1 = rowOffset + blinkBits[offset][1];
    byte x2 = colOffset + blinkBits[offset][2];
    byte y2 = rowOffset + blinkBits[offset][3];
    dots[x1][y1] = isOn;
    dots[x2][y2] = isOn;
    if (antOffset++ >= animMax) {
      antOffset = 0;
      isOn = 1 - isOn;
    }

    timeToMove = 6;
  }
}

void PleaseSelectBox_Wipe()
{
  if (--timeToMove == 0)
  {
    byte offset = antOffset;
    for (byte rowOffset = 0; rowOffset < 3; rowOffset++)
      for (byte colOffset = 0; colOffset < 3; colOffset++) {
        byte c = colOffset * 3 + blinkBits[offset][0];
        byte r = rowOffset * 3 + blinkBits[offset][1];
        dots[c][r] = isOn;
      }

    if (antOffset++ > animMax) {
      antOffset = 0;
      isOn = 1 - isOn;
    }
    timeToMove = 6;
  }
}

void PleaseSelectCell_Blink()
{
  if (--timeToMove == 0)
  {
    timeToMove = 15;
    isOn = 1 - isOn;
    byte colOffset = (selectedBox % 3) * 3;
    byte rowOffset = (selectedBox / 3) * 3;
    for (byte i = 0; i < 8; i++) {
      byte c = colOffset + blinkBits[i][0];
      byte r = rowOffset + blinkBits[i][1];
      dots[c][r] = isOn;
    }
  }
}

void PleaseSelectBox_Blink()
{
  if (--timeToMove == 0)
  {
    timeToMove = 15;
    isOn = 1 - isOn;
    for (byte i = 0; i < 9; i++) {
      dots[i][3] = isOn;
      dots[i][6] = isOn;
      dots[3][i] = isOn;
      dots[6][i] = isOn;
    }
  }
}

void PleaseSelectBox_CrossEdge()
{
  if (--timeToMove == 0)
  {
    timeToMove = 5;
    byte i = antOffset++;
    if (antOffset > 8) {
      antOffset = 0;
      isOn = 1 - isOn;
    }
    dots[i][3] = isOn;
    dots[i][6] = isOn;
    dots[3][i] = isOn;
    dots[6][i] = isOn;
  }
}

void PleaseSelectBox_Cross()
{
  if (--timeToMove == 0)
  {
    timeToMove = 5;
    byte plus = 4 - antOffset;
    byte minus = 4 + antOffset;
    dots[minus][3] = isOn;
    dots[plus][3] = isOn;
    dots[minus][6] = isOn;
    dots[plus][6] = isOn;
    dots[3][minus] = isOn;
    dots[3][plus] = isOn;
    dots[6][minus] = isOn;
    dots[6][plus] = isOn;
    if (++antOffset > 4) {
      antOffset = 0;
      isOn = 1 - isOn;
    }
  }
}

void PleaseSelectCell_MarchingAnts()
{
  if (--timeToMove == 0)
  {
    for (byte i = 0; i < animMax; i++)
    {
      byte colOffset = (selectedBox % 3) * 3;
      byte rowOffset = (selectedBox / 3) * 3;
      byte c = colOffset + blinkBits[i][0];
      byte r = rowOffset + blinkBits[i][1];
      dots[c][r] = ! ((i - antOffset) % 3);
    }
    antOffset++;
    if (antOffset > 2) antOffset = 0;
    timeToMove = 12;
  }
}

void PleaseSelectBox_MarchingAnts()
{
  if (--timeToMove == 0)
  {
    for (byte rowOffset = 0; rowOffset < 3; rowOffset++)
      for (byte colOffset = 0; colOffset < 3; colOffset++)
        for (byte i = 0; i < animMax; i++) {
          byte c = colOffset * 3 + blinkBits[i][0];
          byte r = rowOffset * 3 + blinkBits[i][1];
          dots[c][r] = ! ((i - antOffset) % 3);;
        }

    antOffset++;
    if (antOffset > 2) antOffset = 0;
    timeToMove = 12;
  }
}

//void PleaseSelectCell_computed()
//{
//  byte colOffset = (selectedBox % 3) * 3;
//  byte rowOffset = (selectedBox / 3) * 3;
//  for (byte i = 0; i < 3; i++) {
//    byte offset;
//    offset = colOffset + animOffset; if (offset < 3) dots[offset][rowOffset + 0] = 1; // Top
//    offset = rowOffset + i + animOffset; if (offset < 3) dots[colOffset + 2][offset] = 1; // Right
//    offset = colOffset - i - animOffset; if (offset > 0) dots[offset][rowOffset + 2] = 1; // Bottom
//    offset = rowOffset - i - animOffset; if (offset > 0) dots[colOffset + 0][offset] = 1; // Left
//  }
//}

void PleaseSelectCell_nonAnimated()
{
  byte colOffset = (selectedBox % 3) * 3;
  byte rowOffset = (selectedBox / 3) * 3;
  for (byte i = 0; i < 3; i++) {
    dots[colOffset + i][rowOffset + 0] = 1; // Top
    dots[colOffset + 0][rowOffset + i] = 1; // Left
    dots[colOffset + 2][rowOffset + i] = 1; // Right
    dots[colOffset + i][rowOffset + 2] = 1; // Bottom
  }
}

void PleaseSelectDigit()
{
  byte boxColOffset = (selectedBox % 3) * 3;
  byte boxRowOffset = (selectedBox / 3) * 3;

  byte cellColOffset = (selectedCell % 3);
  byte cellRowOffset = (selectedCell / 3);

  dots[boxColOffset + cellColOffset][boxRowOffset + cellRowOffset] = 1;
}

void SetDigit(byte selectedBox, byte selectedCell, byte selectedDigit)
{
  byte boxColOffset = (selectedBox % 3) * 3;
  byte boxRowOffset = (selectedBox / 3) * 3;

  byte cellColOffset = (selectedCell % 3);
  byte cellRowOffset = (selectedCell / 3);

  // Negative numbers in sudoku array indicates user entered digits, positive numbers digits in original puzzle
  sudoku[boxRowOffset + cellRowOffset][boxColOffset + cellColOffset] = selectedDigit;
}

//
//void AnimateDots()
//{
//    byte r = blinkBits[prevBlinkCount][0];
//    byte c = blinkBits[prevBlinkCount][1];
//    dots[r][c] = 0;
//
//    if (++blinkCount > 9) blinkCount = 0;
//
//    r = blinkBits[blinkCount][0];
//    c = blinkBits[blinkCount][1];
//    dots[r][c] = 1;
//
//    prevBlinkCount = blinkCount-2;
//    if (prevBlinkCount<0) prevBlinkCount+8;
//}


void ValidateSudoku()
{
  if (--timeToMove == 0)
  {
    timeToMove = 15;
    isOn = 1 - isOn;

    for (gRow = 0; gRow < 9; gRow++)
    {
      for (gCol = 0; gCol < 9; gCol++)
      {
        if ( (sudoku[gRow][gCol] < 0) && !isOn) {
        }
      }
    }
  }
}
