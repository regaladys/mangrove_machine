// Version 12.10.b

// Test version for Gallery B.
// mangrove in dark room and bath
// Version 12.6.b didn't randomize restDown
// min height = 2 minutes rest
// max height = 20 minutes rest

#define PINMAIN 2
#define PINUP 3
#define PINDOWN 4

#define MOTORUP 9
#define MOTORDOWN 10 

long timeDebounceMain = 0;  // last time Main switch was pressed
long timeDebounceUp = 0;    // last time Up switch was pressed
long timeDebounceDown = 0;  // last time Down switch was pressed
long timeDebounceRef = 0;   // time of switch, reference for rest time

long delayDebounce = 100;   //

long restUp = 120000;        // 2 minute min rest before going UP
long restDown = 1200000;      // 20 minute min rest before going DOWN
long randUp;
long randDown;

int mainState = LOW;   
int upState = LOW;
int downState = LOW;

int debounceRefStateUP = LOW; // LOW - get a ref time, HIGH - no ref time
int debounceRefStateDOWN = LOW;

void setup() {
  Serial.begin(9600);
  
  pinMode(PINMAIN, INPUT_PULLUP);
  pinMode(PINUP, INPUT_PULLUP);
  pinMode(PINDOWN, INPUT_PULLUP);

  pinMode(MOTORUP, OUTPUT);
  pinMode(MOTORDOWN, OUTPUT);
}

void loop() {

  // main switch
  if (digitalRead(PINMAIN) == LOW && mainState == LOW) 
  { if((millis() - timeDebounceMain) > delayDebounce) 
    { timeDebounceMain = millis();
      Serial.println("BEGIN");
      
      while (mainState == LOW) 
      { 
        // monitor UP pin
        if (digitalRead(PINUP) == LOW)
        { if((millis() - timeDebounceUp) > delayDebounce)
          { 
            upState = HIGH;
            downState = LOW;
            if (debounceRefStateUP == LOW)
            { timeDebounceRef = millis();
              Serial.println("UP UP UP");
              //randUp = restUp + random(0,1)*60000;  // 1-2 minutes              
            }
          }
        }
        
        // monitor DOWN pin
        if (digitalRead(PINDOWN) == LOW)
        { if((millis() - timeDebounceDown) > delayDebounce)
          { 
            upState = LOW;
            downState = HIGH;
            if (debounceRefStateDOWN == LOW)
            { timeDebounceRef = millis();
              Serial.println("DOWN DOWN DOWN");
              //randDown = restDown + random(0,39)*60000;  // 1-40 minutes                         
            }
          }
        }

        if (upState == HIGH && downState == LOW)
        { if (millis() - timeDebounceRef < restUp)
          { debounceRefStateUP = HIGH;
            debounceRefStateDOWN = LOW;
            motorOff();
          }
          else
          { motorUp();
          }
        }

        if (upState == LOW && downState == HIGH)
        { if (millis() - timeDebounceRef < restDown)
          { debounceRefStateUP = LOW;
            debounceRefStateDOWN = HIGH;
            motorOff();
          }
          else
          { motorDown();
          }
        }
        
      }
    }
  }

}

void motorUp() 
{ Serial.println("UP");
  digitalWrite(MOTORUP, HIGH);
  digitalWrite(MOTORDOWN, LOW);
}

void motorDown() 
{ Serial.println("DOWN");
  digitalWrite(MOTORUP, LOW);
  digitalWrite(MOTORDOWN, HIGH);
}

void motorOff()
{ Serial.println("OFF");
  digitalWrite(MOTORUP, LOW);
  digitalWrite(MOTORDOWN, LOW);
}
