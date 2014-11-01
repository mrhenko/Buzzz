// int buttons[2] = { 6, 7 };

boolean debugMode = false;

int digitalButtons[2]                = { 6,     7 };
boolean digitalButtonsIsDepressed[2] = { false, false };
byte noteNumber[2]                   = { 60,    62 };
int numberOfButtons                  = 2;

byte noteOn  = 0x90;
byte noteOff = 0x80;
byte midiChannel = 0;

boolean anyButtonIsPressed = false;

int midiBytes[3] = { 0, -1, -1 };

unsigned long lastAction = 0;
int holdFor = 150; // ms from one command to the next

void setup() {
  
  if ( debugMode ) {
    Serial.begin( 9600 );
  } else {
    Serial.begin( 31250 );
  }
  
  for ( int i = 0; i < numberOfButtons; i++ ) {
    pinMode( digitalButtons[ i ], INPUT );
    if ( debugMode ) {
      Serial.print( "Setup pin " );
      Serial.println( digitalButtons[ i ] );
    }
  }
  
}

void loop() {
  
  if ( ( millis() - holdFor ) > lastAction ) { // If lastAction hasn't changed for 200 ms
    lastAction = 0; // Reset it
  }
  
  anyButtonIsPressed = false;
  for ( int i = 0; i < numberOfButtons; i++ ) {
     if ( digitalButtonsIsDepressed[ i ] ) {
       anyButtonIsPressed = true;
       break;
     }
  }
  
  checkButtons();  
}


/**
 * This functions loops over the arrays containing the
 * inputs (digital and analog) used for buttons to see
 * what or which buttons are pressed.
 *
 * Any pressed button will get a TRUE value in its corresponding
 * place in the digitalButtonsIsDepressed array. Buttons not
 * being pressed will get a false.
 **/
void checkButtons() {
  for ( int i = 0; i < numberOfButtons; i++ ) {
    
    // Digital
    if ( digitalRead( digitalButtons[ i ] ) == HIGH ) {
      if ( debugMode ) {
        Serial.print( digitalButtons[ i ] );
        Serial.println( " currently recieves electricity.");
      }
      if ( digitalButtonsIsDepressed[ i ] == false ) { // If the button isn't already registrered as pressed

        if ( lastAction == 0 ) { // This means nothing has happened for a while, so go ahead
        
          lastAction = millis();
        
          digitalButtonsIsDepressed[ i ] = true;
          
          if ( debugMode ) {
            Serial.print( digitalButtons[ i ] );
            Serial.println( " is currently being pressed.");
          } else {
            if ( anyButtonIsPressed == false ) {
              sendMidi( i, true, midiChannel );
            }
          }
        }
      }
    } else {
      
      if ( digitalButtonsIsDepressed[ i ] ) { // The button was just released
      
        if ( lastAction == 0 ) { // This means nothing has happened for a while, so go ahead
        
          lastAction = millis();
      
          digitalButtonsIsDepressed[ i ] = false;
        
          if ( debugMode ) {
            Serial.print( digitalButtons[ i ] );
            Serial.println( " was just released.");
          } else {    
            sendMidi( i, false, midiChannel );
          }
          
        }
      }
    }
  }
}

void sendMidi( int buttonNumber, boolean isOn, int channel ) {

  if ( isOn ) {
    Serial.write( noteOn );
    Serial.write( noteNumber[ buttonNumber ] );
    Serial.write( 127 );
  } else {
    Serial.write( noteOn );
    Serial.write( noteNumber[ buttonNumber ] );
    Serial.write( 0 );
  }
  
}
