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
    digitalWrite(5, HIGH);
    //Serial.println("High");
  }
  else if (AryDW.which == 1)
   {
     AryDW.which = 0;
     //Serial.println("Low");
     digitalWrite(5, LOW);
   }
   //Serial.println(AryDW.which);
}

void setup() {
  // put your setup code here, to run once:
   attachInterrupt(digitalPinToInterrupt(BtnRead),ModeShift, CHANGE);
  pinMode(5, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

}
