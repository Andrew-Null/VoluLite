//pins
enum LP {high = 4, low = 7, medium = 11}; //light pins
const int MIC = 14; //needs to be analog capable
enum BP {POWER = 0, GROUND, LOAD = 1}; // button/switch pins
enum debug {sp = true, dly = true, dl = 1000, vt = true}; //sp - serial print, dly - delay, dl - delay length, vt - variable testing
//other variables
//unsigned long long CycleCount = 0;
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
  if (sp == true)
  {
  Serial.begin(9600);
  }
  
  //set thresholds
  ala.high.high = 900;
  ala.high.low = 450;

  ala.normal.high = 600;
  ala.normal.low = 300;

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
  //int debug = HIGH;
  //Serial.println(debug);

}

void loop() {
  //get volume
  interrupts();
  volume = analogRead(MIC);
  SwitchLights(PickLight(volume, ala.current));
  
  if (sp == true)
  {
    Serial.println(volume);

    if (vt == true)
    {
      Serial.println(ala.current.high);
    }
    if (dly == true)
    {
      delay(dl);
    }
    
  }
  

}
