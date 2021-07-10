#include <Filters.h>

float testFrequency = 50;                     // test signal frequency (Hz)
float windowLength = 20.0/testFrequency;     // how long to average the signal, for statistist
int sensorValue = 0;
float intercept = -4.4924; // to be adjusted based on calibration testing
float slope = 2.6368; // to be adjusted based on calibration testing
float volts_rms; // estimated actual current in amps

unsigned long printPeriod = 1000; // in milliseconds
// Track time in milliseconds since last reading 
unsigned long previousMillis = 0;

void setup() 
{
  Serial.begin( 115200 );// start the serial port
  pinMode(13,OUTPUT);
}

void loop() 
{
  RunningStatistics inputStats;                 // create statistics to look at the raw test signal
  inputStats.setWindowSecs( windowLength );
   
  while( true ) 
  {   
    sensorValue = analogRead(A0);  // read the analog in value:
    inputStats.input(sensorValue);  // log to Stats function
        
    if((unsigned long)(millis() - previousMillis) >= printPeriod) 
    {
      previousMillis = millis();   // update time
      
      // display current values to the screen
      Serial.print( "\n" );
      // output sigma or variation values associated with the inputValue itsel
      Serial.print( "\tsigma: " ); Serial.print( inputStats.sigma() );
      // convert signal sigma value to current in amps
      volts_rms = intercept + slope * inputStats.sigma();
      volts_rms = round(volts_rms);
      digitalWrite(13,HIGH);
      if(volts_rms>=-2 && volts_rms<=3)
      {
        volts_rms = 0;
        digitalWrite(13,LOW);
      }
      Serial.print( "\tvolts: " ); Serial.print( volts_rms );
    }
  }
}
