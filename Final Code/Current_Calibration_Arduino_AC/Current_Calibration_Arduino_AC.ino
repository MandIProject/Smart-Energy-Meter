#include <Filters.h>

float testFrequency = 50;                     // test signal frequency (Hz)
float windowLength = 20.0/testFrequency;     // how long to average the signal, for statistist
int sensorValue = 0;
float current_amps; // actual measure current
float intercept = -0.0419; // to be adjusted based on calibration testing
float slope = 0.0473; // to be adjusted based on calibration testing

unsigned long printPeriod = 1000; // in milliseconds
// Track time in milliseconds since last reading 
unsigned long previousMillis = 0;

void setup() 
{
  Serial.begin( 115200 );
  pinMode(13,OUTPUT);
}

void loop() 
{
  RunningStatistics inputStats;                 // create statistics to look at the raw test signal
  inputStats.setWindowSecs( windowLength );
   
  while( true ) 
  {   
    sensorValue = analogRead(A5);  // read the analog in value:
    inputStats.input(sensorValue);  // log to Stats function
        
    if((unsigned long)(millis() - previousMillis) >= printPeriod) 
    {
      previousMillis = millis();   // update time
      
      // display current values to the screen
      Serial.print( "\n" );
      // output sigma or variation values associated with the inputValue itself
      Serial.print( "\tsigma: " ); Serial.print( inputStats.sigma() );

      current_amps = intercept + slope * inputStats.sigma();
      digitalWrite(13,HIGH);
      if(current_amps>=-0.05 && current_amps<=0.05)
      {
        current_amps = 0;
        digitalWrite(13,LOW);
      }
      Serial.print( "\tamps: " ); Serial.print( current_amps );
    }
  }
}
