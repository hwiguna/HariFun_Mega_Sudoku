//== Keypad ==
int thresholds[16] = {0, 77, 144, 202, 244, 290, 331, 368, 394, 424, 452, 477, 496, 518, 538, 556};
char keypad[16] = {1, 2, 3, 10, 4, 5, 6, 11, 7, 8, 9, 12, 14, 0, 15, 13}; // A=10,B=11,C=12,D=13,*=14,#=15

//== Functions ==
void HandleKeypress(char keyPress)
{
  switch (keyPress) {
    case 10:
      ClearSelection();
      gameMode = MODE_PICK_BOX;
      break;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      ClearSelection();
      gameMode = MODE_UNKNOWN;
      break;
    default:
      switch (gameMode) {
        case MODE_PICK_BOX:
          ClearSelection();
          selectedBox = keyPress - 1;
          gameMode = MODE_PICK_CELL;
          break;
        case MODE_PICK_CELL:
          ClearSelection();
          selectedCell = keyPress - 1;
          gameMode = MODE_PICK_DIGIT;
          break;
        case MODE_PICK_DIGIT:
          selectedDigit = keyPress;
          SetDigit(selectedBox, selectedCell, selectedDigit);
          break;
      }
      break;
  }
}

void WaitForKeypress()
{
  int value = analogRead(A0);
  Serial.println(value);
  for (int i = 0; i < 16; i++)
  {
    // Is A0 close enough to one of the keypad values?
    if ( abs(value - thresholds[i]) < 4)
    {
      // Yes, translate the index of that value to the actual name of the key
      Serial.println(keypad[i]);
      //FillAll(keypad[i]);
      HandleKeypress(keypad[i]);
      // Wait until they release the button
      while (analogRead(A0) < 1000) {
        delay(100);
      }
    }
  }
  //delayMicroseconds(10);
}


