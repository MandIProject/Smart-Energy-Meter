#include <Filters.h>
#include <LiquidCrystal.h>

float testFrequency = 50;                     // test signal frequency (Hz)
float windowLength = 20.0/testFrequency;     // how long to average the signal, for statistist
int sensorValue = 0;
float current_amps; // actual measure current

unsigned long printPeriod = 1000; // in milliseconds
// Track time in milliseconds since last reading 
unsigned long previousMillis = 0;

void setup() {
  Serial.begin( 57600 );
}

void loop() {
  RunningStatistics inputStats;                 // create statistics to look at the raw test signal
  inputStats.setWindowSecs( windowLength );
   
  while( true ) {   
    sensorValue = analogRead(A5);  // read the analog in value:
    inputStats.input(sensorValue);  // log to Stats function
        
    if((unsigned long)(millis() - previousMillis) >= printPeriod) {
      previousMillis = millis();   // update time
      
      // display current values to the screen
      Serial.print( "\n" );
      // output sigma or variation values associated with the inputValue itsel
      Serial.print( inputStats.sigma() );
    }
  }
}
