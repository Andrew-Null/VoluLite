//pins
enum LP {high = 4, low = 7, medium = 11}; //light pins
const int MIC = 14; //needs to be analog capable
enum BP {POWER = 2, GROUND, LOAD = 3}; // button/switch pins

//other variables
unsigned long long CycleCount = 0;
int volume;

struct threshold
{
  int high, low;
};

struct modes
{
  threshold high, normal, current; // the high/presenter mode, normal mode, and what the current threshold is
} ala; // ala modes



void ModeShift()
{

  //check BP LOAD 
 // auto mode = digitalRead(LOAD);

  //assign ala.current thresholds
  if (digitalRead(LOAD) == HIGH)
  {
    ala.current.high = ala.high.high;
    ala.current.low = ala.high.low;
  }
  else
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
    default:
      digitalWrite(high, HIGH);
      digitalWrite(medium, HIGH);
      digitalWrite(low, HIGH);
  }
}

void setup() {

  //serial 
  Serial.begin(9600);
  
  //set thresholds
  ala.high.high = 1000;
  ala.high.low = 500;

  ala.normal.high = 499;
  ala.normal.low = 0;

  ala.current.high = ala.high.high;
  ala.current.low = ala.high.low;

  //set pins
  pinMode(high, OUTPUT);
  pinMode(medium, OUTPUT);
  pinMode(low, OUTPUT);

  pinMode(LOAD, INPUT);
  pinMode(POWER, OUTPUT);
  digitalWrite(POWER, HIGH);

  //interrupt
  attachInterrupt(digitalPinToInterrupt(LOAD),ModeShift, CHANGE);
  interrupts();

  //debug prints
  int debug = HIGH;
  //Serial.println(debug);

}

void loop() {
  //get volume
  interrupts();
  volume = analogRead(MIC);
  SwitchLights(PickLight(volume, ala.current));
  Serial.println(ala.current.high);

}
