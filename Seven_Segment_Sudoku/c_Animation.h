//== Marching ant animation ==
volatile byte antOffset = 0; // 0,1,2 then it cycles back to 0
volatile unsigned long animSpeed = 12; // Lower = faster
volatile unsigned long animCountdown = animSpeed;

volatile byte blinkSpeed = 12; // Lower = faster
volatile unsigned long blinkCountDown = blinkSpeed;
volatile bool blinkIsOn;

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

volatile byte animMax = sizeof(blinkBits) / (sizeof(byte) * 2);

//----------------------------------------------------------------------------

void PleaseSelectBox_MarchingAnts()
{
  if (--animCountdown == 0)
  {
    for (byte i = 0; i < 9; i++)
    {
      byte isOn = ! ((i - antOffset) % 3);
      SetDot(0, i, isOn); //dots[i][0] = isOn;
      SetDot(i, 8, isOn); //dots[8][i] = isOn;
      SetDot(8, 8 - i, isOn); //dots[8 - i][8] = isOn;
      SetDot(8 - i, 0, isOn); //dots[0][8 - i] = isOn;
    }
    antOffset++;
    if (antOffset > 2) antOffset = 0;
    animCountdown = animSpeed;
  }
}

void PleaseSelectCell_MarchingAnts()
{
  if (--animCountdown == 0)
  {
    for (byte i = 0; i < animMax; i++)
    {
      byte colOffset = (selectedBox % 3) * 3;
      byte rowOffset = (selectedBox / 3) * 3;
      byte c = colOffset + blinkBits[i][0];
      byte r = rowOffset + blinkBits[i][1];
      SetDot(r, c, !((i - antOffset) % 3)); //dots[c][r] = ! ((i - antOffset) % 3);
    }
    antOffset++;
    if (antOffset > 2) antOffset = 0;
    animCountdown = animSpeed;
  }
}

void PleaseSelectDigit_Blink()
{
  if (--animCountdown == 0)
  {
    byte boxColOffset = (selectedBox % 3) * 3;
    byte boxRowOffset = (selectedBox / 3) * 3;

    byte cellColOffset = (selectedCell % 3);
    byte cellRowOffset = (selectedCell / 3);

    byte isOn = (antOffset > 0);

    //dots[boxColOffset + cellColOffset][boxRowOffset + cellRowOffset] = isOn;
    SetDot(boxRowOffset + cellRowOffset, boxColOffset + cellColOffset, isOn);

    antOffset = isOn ? 0 : 1;
    animCountdown = animSpeed;
  }
}

void AnimateBlinks()
{
  if (--blinkCountDown == 0)
  {
    for (byte r = 0; r < 9; r++)
      for (byte c = 0; c < 9; c++)
        if (!bitRead(sudoku[r][c],IS_WRONG_BIT))
          bitSet(sudoku[r][c], IS_VISIBLE_BIT);
        else
          if (blinkIsOn) 
          bitSet(sudoku[r][c], IS_VISIBLE_BIT); 
          else 
          bitClear(sudoku[r][c], IS_VISIBLE_BIT);

    blinkIsOn = !blinkIsOn;
    blinkCountDown = blinkSpeed;
  }
}

