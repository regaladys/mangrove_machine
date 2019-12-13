// Version 12.13.a
// min height = 1 - 5 minutes rest
// max height = 1 - 15 minutes rest

// Exhibition version for Gallery A.
// - 22 mangroves in the main gallery.

#define PINMAIN 2
#define PINUP 3
#define PINDOWN 4

#define MOTORUP 9
#define MOTORDOWN 10 

long timeDebounceMain = 0;  // last time Main switch was pressed
long timeDebounceUp = 0;    // last time Up switch was pressed
long timeDebounceDown = 0;  // last time Down switch was pressed
long timeDebounceRef = 0;   // time of switch, reference for rest time

long delayDebounce = 100; //

long restUp = 60000;      // 1 minutes min rest before going UP, min height
long restDown = 60000;    // 1 minutes min rest before going DOWN, max height
long randUp;              // randomize rest time before going UP
long randDown;            // randomize rest time before going DOWN
//long moveUp = 10000;    // 10 second maximum motor movement UP
//long moveDown = 10000;  // 10 second maximum motor movement DOWN 

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

void loop() 
  { 
  if (digitalRead(PINMAIN) == LOW && mainState == LOW)  // --- monitor PINMAIN main switch
  
  { // --- start PINMAIN monitor
    if((millis() - timeDebounceMain) > delayDebounce)   // --- if PINMAIN is active after a long time
    { timeDebounceMain = millis();                      // --- time of PINMAIN activation = current time stamp
      mainState = HIGH;                                 // --- main state is set as active, routine begins.
      //Serial.println("BEGIN");
    }
  } // --- end PINMAIN monitor
  
  
  while (mainState == HIGH) 
  
  { // --- main routine begins when PINMAIN is activated
    //machineState();                                   // --- sends machine state via Serial
    if (digitalRead(PINUP) == LOW)                      // --- monitor UP pin
    
    { if((millis() - timeDebounceUp) > delayDebounce)   // --- UP pin is clicked
      { upState = HIGH;
        downState = LOW;
        if (debounceRefStateUP == LOW)                  // --- RefStateUP == LOW, get a reference time
        { timeDebounceRef = millis();
          Serial.println("UP UP UP");
          randUp = restUp + random(0,4)*60000;          // --- 1 - 5 minutes
          Serial.println(randUp);
        }
       }
     }
        
    // monitor DOWN pin
    if (digitalRead(PINDOWN) == LOW)
     
    { if((millis() - timeDebounceDown) > delayDebounce)
      { upState = LOW;
        downState = HIGH;
        if (debounceRefStateDOWN == LOW)
        { timeDebounceRef = millis();
          Serial.println("DOWN DOWN DOWN");
          randDown = restDown + random(0,14)*60000;      // --- 1 - 15 minutes
          Serial.println(randDown);
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
        
     { if (millis() - timeDebounceRef < randDown)
       { debounceRefStateUP = LOW;
         debounceRefStateDOWN = HIGH;
         motorOff();
       }
          
       else
       { motorDown();
       }
     } // upState == LOW && downState == HIGH 
  }

}

void motorUp()
{ //Serial.println("UP");
  digitalWrite(MOTORUP, HIGH);
  digitalWrite(MOTORDOWN, LOW);
}

void motorDown() 
{ //Serial.println("DOWN");
  digitalWrite(MOTORUP, LOW);
  digitalWrite(MOTORDOWN, HIGH);
}

void motorOff()
{ //Serial.println("OFF");
  digitalWrite(MOTORUP, LOW);
  digitalWrite(MOTORDOWN, LOW);
}

void machineState(){
  Serial.print(digitalRead(PINUP));
  Serial.print(digitalRead(PINDOWN));
  Serial.print(upState);
  Serial.print(downState);
  Serial.print(debounceRefStateUP);
  Serial.println(debounceRefStateDOWN);
}
