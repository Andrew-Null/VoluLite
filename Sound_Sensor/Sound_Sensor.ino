int RED = 4;
int BLUE = 5;
int GREEN = 2;
unsigned long GRACE = 999999999; // Max grace time in milliseconds
int MAX = 500;  // Max volume. 500 is about 100 dB
int MIN = 350;  // Min volume. 350 is about 58 dB

unsigned long oldMillis = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int loudness = analogRead(A0);
  unsigned long cycle = GRACE + 1;
  Serial.println(loudness);

  if(loudness < MIN && cycle > GRACE) // Too quiet
  {
    digitalWrite(BLUE, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, LOW);
  }
  else if(loudness > MAX) // Too loud
  {
    digitalWrite(BLUE, LOW);
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
  }
  else // Just right
  {
    digitalWrite(BLUE, LOW);
    digitalWrite(GREEN, HIGH);
    digitalWrite(RED, LOW);
      
      cycle = 0;
    //Serial.println(cycle);
  }

  if(cycle <= GRACE)
  {
    Serial.println(millis() - oldMillis);
    cycle += millis() - oldMillis;
    oldMillis = millis();
  }
}
