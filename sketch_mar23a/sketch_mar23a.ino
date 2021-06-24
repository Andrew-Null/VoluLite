//pins
enum LP {high = 4, low = 7, medium = 11}; //light pins
const int MIC = 14; //needs to be analog cpable
enum BP {POWER, GROUND, LOAD}; // button/switch pins

//other variables
unsigned long long CycleCount = 0;
int volume;

struct threshold
{
  int high, low;
};

struct modes
{
  threshold high, normal, current;
} ala;

//void DetermineMode()

void ModeShift()
{

  //check load
  auto mode = digitalRead(LOAD);

  //assign ala.current thresholds
  if (mode == HIGH)
  {
    ala.current.high = ala.high.high;
    ala.current.low = ala.high.low;
  }
  else if (mode == LOW)
   {
    ala.current.high = ala.normal.high;
    ala.current.low = ala.normal.low;
   }
}

LP PickLight(int volume, threshold limits)
{
  if (volume > limits.high)
    return high;
  else if (volume < limits.low)
    return low;
  else
    return medium;
}

void SwitchLights(LP pick) //pass picklight to SwitchLights
{
  switch(pick)
  {
    case high:
      digitalWrite(high, HIGH);
      digitalWrite(medium, LOW);
      digitalWrite(low, LOW);
      break;
    case medium:
      digitalWrite(high, LOW);
      digitalWrite(medium, HIGH);
      digitalWrite(low, LOW);
      break;
    case low:
      digitalWrite(high, LOW);
      digitalWrite(medium, LOW);
      digitalWrite(low, HIGH);
      break;
  }
}

void setup() {

  //serial 
  Serial.begin(9600);
  
  //set thresholds
  ala.high.high = 685;
  ala.high.low = 681;

  ala.normal.high = 400;
  ala.normal.low = 360;

  //set pins
  pinMode(high, OUTPUT);
  pinMode(medium, OUTPUT);
  pinMode(low, OUTPUT);

  pinMode(LOAD, INPUT);
  pinMode(POWER, OUTPUT);
  digitalWrite(POWER, HIGH);

}

void loop() {
  //get volume
  volume = analogRead(MIC);
  SwitchLights(PickLight(volume, ala.current));

}
