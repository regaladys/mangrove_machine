// Version 12.6.a
// min height = 1 - 5 minutes
// max height = 1 - 10 minutes

// Working version.

// -- start -- 15.g notes
// Solves the V 15.d problem, timeDebounceRef = millis()
// as long as button is pressed.
// Get a timeDebounceRef that does not change until reset.

//problem with 600000 (10 minutes) and 300000 (5 minutes).

// timeDebounceRef state
// -- end -- 15.g notes


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

long restUp = 60000;        // 1 minute min rest before going UP, min height
long restDown = 60000;      // 1 minute min rest before going DOWN, max height
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
              randUp = restUp + random(0,4)*60000;  // 1-5 minutes              
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
              randDown = restDown + random(0,9)*60000;  // 1-10 minutes                         
            }
          }
        }

        if (upState == HIGH && downState == LOW)
        { if (millis() - timeDebounceRef < randUp)
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
