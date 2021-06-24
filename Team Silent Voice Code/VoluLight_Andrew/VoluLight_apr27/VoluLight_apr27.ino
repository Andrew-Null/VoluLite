//pins
enum LP {high = 4, low = 7, medium = 11}; //light pins
const int MIC = 14; //needs to be analog capable
enum BP {POWER = 0, GROUND, LOAD = 10}; // button/switch pins

//other
enum debug {sp = true, dly = true, /*dl = 200,*/ vt = true}; //sp - serial print, dly - delay, dl - delay length, vt - variable testing
enum settings {nl = 532, nh = 565, hl = 565, hh = 690, dl = 900}; // normal low, normal high, high low, high high, for length, delay length

//other variables
int cycleCount; // Keeps track of delay time
int volume;

struct threshold
{
  unsigned int high : 11;
  unsigned int low : 11;
};

struct modes
{
  threshold high, normal, current; // the high/presenter mode, normal mode, and what the current threshold is
  //volatile threshold temp;
  // volatile bool nc; //new current
} ala; // ala modes



void ModeShift()
{

  //check BP LOAD
  // auto mode = digitalRead(LOAD);

  //assign ala.current thresholds
  digitalWrite(high, HIGH);
  digitalWrite(low, HIGH);

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

// Check whether light is high, low, or medium
LP PickLight(int volume, threshold limits)
{
  if (volume > limits.high)
  {
    cycleCount = 0;
    return high;
  }
  else if (volume < limits.low && cycleCount > dl)
  {
    return low;
  }
  else
  {
    if (volume >= limits.low)
    {
      cycleCount = 0;
    }

    return medium;
  }
}

// Use parameter value to determine which light to turn on
void SwitchLights(LP pick) //pass picklight to SwitchLights
{
  switch (pick)
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

void checkToggle()
{
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

/*int Sample(threshold limits)
  {
  // DEBUG
  if (sp == true)
  {
    Serial.println("GetAverage starts");
  }

  int SMax = 0; //sampled max
  int volume;
  for (short fv = 0; fv <= fl; fv++)
  {
    volume = analogRead(MIC);
    if (SMax < volume)
    {
      SMax = volume;
      if(volume > limits.low)
      {
        delay(10);
      }
    }

    // DEBUG
    if (sp == true)
    {
      Serial.println(volume);
    }
  }

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

  if (sp == true)
  {

    Serial.println("Sample ends");
    delay(dl);
  }

  return SMax;
  }*/

void setup() {

  //serial
  if (sp == true)
  {
    Serial.begin(9600);
  }

  //set thresholds
  ala.high.high = hh;
  ala.high.low = hl;

  ala.normal.high = nh;
  ala.normal.low = nl;

  ala.current.high = ala.normal.high;
  ala.current.low = ala.normal.low;

  // set cycle
  cycleCount = 0;

  //set pins
  pinMode(high, OUTPUT);
  pinMode(medium, OUTPUT);
  pinMode(low, OUTPUT);

  pinMode(LOAD, INPUT);
}

void loop() {

  checkToggle();
  volume = analogRead(MIC);
  SwitchLights(PickLight(volume, ala.current));

  if (cycleCount <= dl)
  {
    cycleCount++;
  }

  // DEBUG
  if (sp == true)
  {
    //Serial.println(volume);

    if (vt == true)
    {
      Serial.println(volume);
    }
  }
}
