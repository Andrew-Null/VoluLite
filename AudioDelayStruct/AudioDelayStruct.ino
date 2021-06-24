const int RED = 4;
const int BLUE = 5;
const int GREEN = 2;
const int ANALOG = A0;
const unsigned long GRACE = 150; // Max grace time in milliseconds
const int MAX = 500;  // Max volume. 500 is about 100 dB
const int MIN = 350;  // Min volume. 350 is about 58 dB
unsigned long oldMillis = 0;



struct DW
{
  int thresholds[2][2] = {{350, 450}, {385, 495}}; // {lower, higher}
  volatile int which = 0;
};

DW AryDW;

//interrupt
// https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
const byte BtnRead = 3;
const int BtnOut = 9;


void ModeShift()
{
  Serial.begin(9600);
  //Serial.println(AryDW.which);
  if (AryDW.which == 0)
  {
    AryDW.which = 1;
    //Serial.println("High");
  }
  else if (AryDW.which == 1)
   {
     AryDW.which = 0;
     //Serial.println("Low");
   }
   //Serial.println(AryDW.which);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BtnRead, INPUT_PULLUP);

  //pinMode(BtnOut, OUTPUT);
  //digitalWrite(BtnOut, HIGH);
  
  attachInterrupt(digitalPinToInterrupt(BtnRead),ModeShift, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  int loudness = analogRead(14);
  unsigned long cycle = GRACE + 1;
  //Serial.println(loudness);

  if(loudness < AryDW.thresholds[AryDW.which][0] && cycle > GRACE) // Too quiet
  {
    digitalWrite(BLUE, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, LOW);
  }
  else // Just right
  {
    if(loudness > AryDW.thresholds[AryDW.which][1])
    {
      digitalWrite(BLUE, LOW);
      digitalWrite(RED, HIGH);
      digitalWrite(GREEN, LOW);
    }
    else
    {
      digitalWrite(BLUE, LOW);
      digitalWrite(GREEN, HIGH);
      digitalWrite(RED, LOW);
    }
    
    
    delay(GRACE);
  }

}
