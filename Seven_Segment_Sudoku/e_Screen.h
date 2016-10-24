//== Digit bitmaps ==
volatile byte digitBits[] = {
  B00000000, // 0
  B00001100, // 1
  B11011010, // 2
  B10011110, // 3
  B00101110, // 4
  B10110110, // 5
  B11110110, // 6
  B00011100, // 7
  B11111110, // 8
  B10111110, // 9
};

//-----------------------------------------------------------------

void FastSetColumn(byte col)
{
  col = 8 - col;
  byte bits = 0;
  if (col < 6) {
    bits = 1 << (col + 2); // Col0 = Port D bit 2
    PORTD = (PORTD & B00000011) | bits; // set appropriate bit of port D (D2..D7)
    PORTB = PORTB & B11111000; // clear lowest three bits of Port B (D8,D9,D10)
  } else
  {
    bits = 1 << (col - 6); // col6 = Port B bit 0
    PORTD = PORTD & B00000011; // clear all but the bottom two bits (TX/RX) of port D (D2..D7)
    PORTB = (PORTB & B11111000) | bits; // set appropriate bit of port B (D8,D9,D10)
  }
}

void DisplayCell(byte cellValue)
{
  byte cellDigit = GetDigit(cellValue);
  byte showDot = bitRead(cellValue, IS_DOT_ON_BIT);
  byte segByte =  bitRead(cellValue,IS_VISIBLE_BIT) ? digitBits[cellDigit] | showDot : digitBits[0];
  shiftOut(SEG_SER_PIN, SEG_CLK_PIN, LSBFIRST, ~segByte);
}

void Refresh(void)
{
  if (--refreshCounter == 0) {
    refreshCounter = refreshRate;
    digitalWrite(A5, HIGH); // Debug
    for (gCol = 0; gCol < 9; gCol++)
    {
      digitalWrite(SEG_LATCH_PIN, LOW); // Do not reflect bit changes as we're setting up 9 rows of digits for upcoming column
      for (gRow = 0; gRow < 9; gRow++)
      {
        byte cellValue = sudoku[gRow][gCol];
        DisplayCell( cellValue ); // Shift the digit bits, last row first
      }
      // We've shifted all 9 vertical digits
      digitalWrite(SEG_LATCH_PIN, HIGH); // Slam all 9 vertical digits to output pins
      FastSetColumn(gCol);
    }
    digitalWrite(A5, LOW); // Debug
  } else
  {
    if (gameMode == MODE_UNKNOWN) ClearSelection();
    if (gameMode == MODE_PICK_BOX) PleaseSelectBox_MarchingAnts();
    if (gameMode == MODE_PICK_CELL) PleaseSelectCell_MarchingAnts();
    if (gameMode == MODE_PICK_DIGIT) PleaseSelectDigit_Blink();
    //if (gameMode == MODE_VALIDATE) ValidateSudoku();
  }
}

void SetupTimer()
{
  OCR0A = 0xFF; // When timer0 reaches this value, fire TIMER0_COMPA_vect
  TIMSK0 |= _BV(OCIE0A);
}

SIGNAL(TIMER0_COMPA_vect)
{
  digitalWrite(A4, HIGH); // Debug
  digitalWrite(A4, LOW); // Debug
  Refresh();
  //RefreshUsingCounters();
}

