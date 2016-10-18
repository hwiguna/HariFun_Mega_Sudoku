void setup() {
  pinMode(SEG_CLK_PIN, OUTPUT);
  pinMode(SEG_LATCH_PIN, OUTPUT);
  pinMode(SEG_SER_PIN, OUTPUT);

  for (byte i = 0; i < 9; i++)
    pinMode(COL_9_PIN - i, OUTPUT);

  pinMode(A4,OUTPUT);
  pinMode(A5,OUTPUT);
  
  Serial.begin(9600);
//  Serial.print("animFrameCount=");
//  Serial.println(animFrameCount);
  
  //EraseAll();
  //FillSudoku1();
  //FillAll8();
  //FillRandomly();
  SetupTimer();
}

//void SegmentTest()
//{
//  //== Scan through all the segments ===
//  for (byte seg = 0; seg < 8; seg++)
//  {
//    byte segByte = 0;
//    bitSet(segByte, seg);
//
//    digitalWrite(SEG_LATCH_PIN, LOW);
//    shiftOut(SEG_SER_PIN, SEG_CLK_PIN, MSBFIRST, ~segByte);
//    digitalWrite(SEG_LATCH_PIN, HIGH);
//
//    delay(100);
//  }
//}

