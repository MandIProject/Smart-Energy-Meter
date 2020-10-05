#include <Filters.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

SoftwareSerial s(5,6);

float testFrequency = 50;                     // test signal frequency (Hz)
float windowLength = 20.0/testFrequency;     // how long to average the signal, for statistist
int sensorValue = 0;
float intercept = -0.0419; // to be adjusted based on calibration testing
float slope = 0.0473; // to be adjusted based on calibration testing
float current_amps; // estimated actual current in amps
float power;
float energy;
unsigned long current_time = 0;
unsigned long previous_time = 0;

unsigned long printPeriod = 1000; // in milliseconds
// Track time in milliseconds since last reading 
unsigned long previousMillis = 0;

void setup() 
{
  s.begin(9600);
  Serial.begin( 115200 );// start the serial port
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
      // output sigma or variation values associated with the inputValue itsel
      Serial.print( "\tsigma: " ); Serial.print( inputStats.sigma() );
      // convert signal sigma value to current in amps
      current_amps = intercept + slope * inputStats.sigma();
      power = (current_amps)*240;
      previous_time = current_time;
      current_time = millis();
      energy = energy + power*((current_time - previous_time)/3600000.0);
      digitalWrite(13,HIGH);
      if(current_amps>=-0.05 && current_amps<=0.05)
      {
        power = 0;
        current_amps = 0;
        energy = 0;
        digitalWrite(13,LOW);
      }
      Serial.print( "\tamps: " ); Serial.print( current_amps );
      Serial.print( "\tWatts: " ); Serial.print( power );
      Serial.print( "\tWh: "); Serial.print( energy );
      StaticJsonBuffer<1000> jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();
      root["data1"] = current_amps;
      root["data2"] = power;
      root["data3"] = inputStats.sigma();
      root["data4"] = energy;
      if(s.available()>0)
      {root.printTo(s);}
    }
  }
}
