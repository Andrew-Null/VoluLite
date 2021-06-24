//pins
enum LP {high = 4, low = 7, medium = 11}; //light pins
const int MIC = 14; //needs to be analog capable
enum BP {POWER = 0, GROUND, LOAD = 10}; // button/switch pins
enum debug {sp = false, dly = true, /*dl = 200,*/ vt = false}; //sp - serial print, dly - delay, dl - delay length, vt - variable testing
enum settings {nl = 522, nh = 620, hl = 558, hh = 714, fl = 750, dl = 400}; // normal low, normal high, high low, high high, for length, delay length
//other variables
//unsigned long long CycleCount = 0;



struct threshold
{
  int high, low;
};

struct modes
{
  threshold high, normal, current; // the high/presenter mode, normal mode, and what the current threshold is
  //volatile threshold temp;
  volatile bool nc; //new current
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
   ala.nc = true;
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
      delay(dl);
      break;
    case medium:
      digitalWrite(high, LOW);
      digitalWrite(medium, HIGH);
      digitalWrite(low, LOW);
      delay(dl * 1.25);
      break;
    case low:
      digitalWrite(high, LOW);
      digitalWrite(medium, LOW);
      digitalWrite(low, HIGH);
      delay(dl);
      break;
    default:
      digitalWrite(high, HIGH);
      digitalWrite(medium, HIGH);
      digitalWrite(low, HIGH);
  }
}

int Sample()
{
  if (sp == true)
  {
    Serial.println("GetAverage starts");
  }
  int SMax = 0; //sampled max
  //unsigned int vt; //volume totaL
  int volume;
  for (short fv = 0; fv <= fl; fv++)
  {
    volume = analogRead(MIC);
    if (SMax < volume)
    {
      SMax = volume;
      delay(10);
    }
      

    
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
    Serial.println(vt/200);
    Serial.println("Sample ends");
    delay(dl);
  }
  
  return SMax;
}

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
  //volume = analogRead(MIC);

  

  
  
 
  SwitchLights(PickLight(Sample(), ala.current));
  
  if (sp == true)
  {
    //Serial.println(volume);

    if (vt == true)
    {
      Serial.println(ala.current.high);
    }

    
  }
  

}
