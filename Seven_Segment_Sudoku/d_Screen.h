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
void SetColumn(byte col, byte state)
{
  digitalWrite(COL_9_PIN - col, state);
}

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

void DisplayDigit(byte digit, byte dotState)
{
  byte segByte = digitBits[digit] | dotState;
  shiftOut(SEG_SER_PIN, SEG_CLK_PIN, LSBFIRST, ~segByte);
}

void SetDot(byte row, byte col, byte state)
{
  dots[row][col] = state;
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
        DisplayDigit( sudoku[gCol][gRow], dots[gCol][gRow] ); // Shift the digit bits, last row first
      }
      // We've shifted all 9 vertical digits
      digitalWrite(SEG_LATCH_PIN, HIGH); // Slam all 9 vertical digits to output pins
      FastSetColumn(gCol);
    }
    digitalWrite(A5, LOW); // Debug
  } else
  {
    if (gameMode == MODE_UNKNOWN) ClearSelection();
    if (gameMode == MODE_PICK_BOX) PleaseSelectBox();
    if (gameMode == MODE_PICK_CELL) PleaseSelectCell_nonAnimated();
    if (gameMode == MODE_PICK_DIGIT) PleaseSelectDigit();
  }
}

void RefreshUsingCounters(void)
{
  //noInterrupts();

  if (gRow == 0) digitalWrite(SEG_LATCH_PIN, LOW); // Do not reflect bit changes as we're setting up 9 rows of digits for upcoming column

  DisplayDigit( sudoku[gRow][gCol], dots[gRow][gCol] ); // Shift the digit bits, last row first

  if (gRow == 8) {
    // We've shifted all 9 vertical digits
    //SetColumn(gPrevCol, LOW); // Turn off previous column
    digitalWrite(SEG_LATCH_PIN, HIGH); // Slam all 9 vertical digits to output pins
    //SetColumn(gCol, HIGH); // Turn on JUST the "current" colum
    FastSetColumn(gCol);
    //gPrevCol = gCol;
    if (++gCol > 8) gCol = 0;
  }

  if (++gRow > 8) gRow = 0;

  //interrupts();
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

//void SetupTimer()
//{
//  cli();
//  // Reset any PWM that arduino may have setup automatically
//  TCCR2A = 0;
//  TCCR2B = 0;
//
//  TCCR2A |= (1 << WGM21); // CTC mode. Reset counter when OCR2 is reached
//
//  TCCR2B |= (1 << CS21) | (1 << CS22); // Prescaler = 256
//  //TCCR2B |= (1 << CS20) | (1 << CS22); // Prescaler = 128
//  OCR2A = 200; // Fire interrupt when timer2 has looped 14 times
//
//  TCNT2 = 0; // initial counter value = 0;
//  TIMSK2 |= (1 << OCIE2A); // Enable CTC interrupt
//  sei();
//}
//
//ISR (TIMER2_COMPA_vect)
//{
//  Refresh();
//}


