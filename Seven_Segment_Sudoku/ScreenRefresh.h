void SetColumn(byte col, byte state)
{
  digitalWrite(COL_9_PIN-col, state);
}

void FastSetColumn(byte col)
{
  int bits = 0;
  if (col<6) {
    //bitSet(bits,col);
    bits = 1 << (col+2);
    PORTD = (PORTD & 0x00000011) | bits; // set appropriate bit of port D (D2..D7)
    PORTB = PORTB & 0x11111000; // clear lowest three bits of Port B (D8,D9,D10)
  } else
  {
    bits = 1 << (col-6);
    PORTD = PORTD & 0x00000011; // clear all but the bottom two bits (TX/RX) of port D (D2..D7)
    PORTB = (PORTB & 0x11111000) | bits; // set appropriate bit of port B (D8,D9,D10)
  }
}

void DisplayDigit(byte digit, byte dotState)
{
  byte segByte = digitBits[digit] | dotState;
  shiftOut(SEG_SER_PIN, SEG_CLK_PIN, LSBFIRST, ~segByte);
}

void SetDot(byte row, byte col, byte state)
{
  dots[row][col]=state;
}

void Refresh(void)
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
  cli();
  // Reset any PWM that arduino may have setup automatically
  TCCR2A = 0;
  TCCR2B = 0;

  TCCR2A |= (1 << WGM21); // CTC mode. Reset counter when OCR2 is reached

  TCCR2B |= (1 << CS21) | (1 << CS22); // Prescaler = 256
  //TCCR2B |= (1 << CS20) | (1 << CS22); // Prescaler = 128
  OCR2A = 10; // Fire interrupt when timer2 has looped 14 times

  TCNT2 = 0; // initial counter value = 0;
  TIMSK2 |= (1 << OCIE2A); // Enable CTC interrupt
  sei();
}

ISR (TIMER2_COMPA_vect)
{
  Refresh();
}


