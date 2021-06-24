const int RED = 4;
const int BLUE = 7;
const int GREEN = 11;
const int ANALOG = A0;
const int BUTTON = 9;
const unsigned long GRACE = 500; // Max grace time in milliseconds
const int MAX = 685;//400;  // Max volume. 500 is about 100 dB
const int MIN = 681;//360;  // Min volume. 350 is about 61 dB
unsigned long oldMillis = 0;
unsigned long long cycle = GRACE + 1;

boolean powered;
boolean pastButton = LOW; // Whether the button was pressed one cycle before

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BUTTON, INPUT);
  powered = true;
}

void loop() {
  // put your main code here, to run repeatedly:
  if(powered)
  {
    int loudness = analogRead(A0);
    
    Serial.println(loudness);
  
    
    if(loudness < MIN && cycle > GRACE) // Too quiet
    {
      digitalWrite(BLUE, HIGH);
      digitalWrite(GREEN, LOW);
      digitalWrite(RED, LOW);
      //Serial.println("blue ");
    }
    else // Just right
    {
      if(loudness > MAX)
      {
        digitalWrite(BLUE, LOW);
        digitalWrite(RED, HIGH);
        digitalWrite(GREEN, LOW);
        //Serial.println("red");
      }
      else
      {
        digitalWrite(BLUE, LOW);
        digitalWrite(GREEN, HIGH);
        digitalWrite(RED, LOW);
        //Serial.println("green");
      }

      if(loudness >= MIN)
      {
        cycle = 0;
      }
      //delay(GRACE);
    }

    if(cycle <= GRACE)
    {
      cycle++;
    }
  }
  else
  {
    digitalWrite(BLUE, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, LOW);
  }

/*
  if(digitalRead(BUTTON) == HIGH && pastButton == LOW)
  {
    powered = !powered;
  }

  pastButton = digitalRead(BUTTON); */
}
