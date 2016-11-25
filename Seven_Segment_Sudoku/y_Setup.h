void setup() {
  pinMode(SEG_CLK_PIN, OUTPUT);
  pinMode(SEG_LATCH_PIN, OUTPUT);
  pinMode(SEG_SER_PIN, OUTPUT);

  for (byte i = 0; i < 9; i++)
    pinMode(COL_9_PIN - i, OUTPUT);

  pinMode(A4,OUTPUT);
  pinMode(A5,OUTPUT);
  
  Serial.begin(9600);
  
  randomSeed(analogRead(5));
  //LoadSudoku();

  gameMode = MODE_TEXT;
  ClearText();  
  DrawText(0,0,"123456789");
  DrawText(1,0,"ABCDEFGHI");
  DrawText(2,0,"JKLMNOPQR");
  DrawText(3,0,"STUVWXYZ0");

  DrawText(5,0,"0MMEGA");
  DrawText(6,0,"0SUDOKU");
  SetupTimer();
}
