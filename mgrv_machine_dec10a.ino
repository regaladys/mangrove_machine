// Version 12.6.a
// min height = 2 minutes rest
// max height = 20 minutes rest

// Temporary version for Gallery A.
// - 22 mangroves in the main gallery.
// - This version is to test at fixed times.
// - Timing requested by Jon.
// - 22 mangroves malfunctioned on first night
//  after 8 or more hours of running.


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

long restUp = 120000;        // 2 minutes min rest before going UP, min height
long restDown = 1200000;      // 20 minutes min rest before going DOWN, max height
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
              //randUp = restUp + random(0,4)*60000;  // fix timings              
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
              //randDown = restDown + random(0,9)*60000;  // fix timings                         
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
