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
//  LoadSudoku();
//  gameMode = MODE_ASSIST;

  SetupTimer();

  gameMode = MODE_TEXT;
  ClearText();  

//0 Mega
//1 Sudoku
//2

  DrawText(3,3,   "The");
  DrawText(4,2,  "FinaL");
  DrawText(5,1, "Episode");

  DrawText(7,1,"Harl0Fun");
  DrawText(8,3,  "146");

//  DrawText(1,1,"Harl0Fun");
//  DrawText(2,3,"146");
//  delay(1000);
//    
//  DrawText(3,0,"ALphabet");
//  delay(1000);
//  DrawText(4,0,"Checker");
//  delay(1000);
//  DrawText(5,0,"Assistant");
//  delay(1000);
//  DrawText(6,0,"Generator");
//  delay(1000);
//  DrawText(7,0,"And___");
  
//  DrawText(3,0,"ALPHABET");
//  DrawText(4,0,"CHECKER");
//  DrawText(5,0,"ASSISTANT");
//  DrawText(6,0,"GENERATOR");
//  DrawText(7,0,"ALPHABET");

//  DrawText(1,0,"123456789");
//  
//  DrawText(3,0,"ABCDEFGHI");
//  DrawText(4,0,"JKLMNOPQR");
//  DrawText(5,0,"STUVWXYZ0");
//
//  DrawText(6,0,"abcdefghi");
//  DrawText(7,0,"jklmnopqr");
//  DrawText(8,0,"stuvwxyz0");

//  DrawText(4,1,"Pretty");
//  DrawText(5,2, "COOL");


//  DrawText(4,1, "HELLO");
//  DrawText(5,0,"YouTube");
  
//  DrawText(4,1,"r;EGA");
//  DrawText(5,1,"Sudoku");
}
