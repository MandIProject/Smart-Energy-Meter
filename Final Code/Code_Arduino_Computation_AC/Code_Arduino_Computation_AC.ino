#include <Filters.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

SoftwareSerial s(5,6);

float testFrequency = 50;                     // test signal frequency (Hz)
float windowLength = 20.0/testFrequency;     // how long to average the signal, for statistic
int sensorValue = 0;
int sensorValue_volt = 0;
float intercept = -0.0419; // to be adjusted based on calibration testing
float slope = 0.0473; // to be adjusted based on calibration testing
float interceptVolts = -4.4924;// to be adjusted based on calibration testing
float slopeVolts = 2.6368;// to be adjusted based on calibration testing
float Volts;// estimated actual voltage in volts
float Volt;
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
  s.begin(9600);// starting serial port for serial communication with Wemos D1 Mini Pro
  Serial.begin( 115200 );// start the serial port
  pinMode(13,OUTPUT);
}

void loop() 
{
  CurrentAndVoltage();
}

float CurrentAndVoltage()
{
  RunningStatistics inputStats;                 // create statistics to look at the raw test signal
  inputStats.setWindowSecs( windowLength );

  RunningStatistics inputStats1;
  inputStats1.setWindowSecs( windowLength );    // create statistics to look at the raw test signal

  while(true)
  {
    sensorValue = analogRead(A5);  // read the analog in value
    inputStats.input(sensorValue);  // log to Stats function
    sensorValue_volt = analogRead(A0);  // read the analog in value
    inputStats1.input(sensorValue_volt);  // log to Stats function
        
    if((unsigned long)(millis() - previousMillis) >= printPeriod) 
    {
      previousMillis = millis();   // update time
      // convert signal sigma value to current in amps
      current_amps = intercept + slope * inputStats.sigma();  // finding out the current using line of best fit
      Volts = interceptVolts + slopeVolts * inputStats1.sigma();  // finding out the voltage using line of best fit
      Volt = round(Volts);
      previous_time = current_time;
      current_time = millis();
      power = current_amps*Volts; // calculating power
      energy = energy + power*((current_time - previous_time)/3600000.0); // calculating energy spent
      digitalWrite(13,HIGH);
      if(current_amps>=-0.05 && current_amps<=0.05)
      {
        Volts = 0;
        current_amps = 0;
        power = 0;
        energy = 0;
        digitalWrite(13,LOW);
      }
      Serial.print( "\n" );
      Serial.print( "\tsigma_current: " ); Serial.print( inputStats.sigma() );
      Serial.print( "\tamps: " ); Serial.print( current_amps );
      Serial.print( "\tsigma_voltage: " ); Serial.print( inputStats1.sigma() );
      Serial.print( "\tvolts: "); Serial.print( Volt );
      Serial.print( "\tWatts: " ); Serial.print( power );
      Serial.print( "\tWh: "); Serial.print( energy );
      // sending data to Wemos D1 Mini Pro V2 using JSON format
      StaticJsonBuffer<1000> jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();
      root["data1"] = current_amps;
      root["data2"] = Volts;
      root["data3"] = power;
      root["data4"] = energy;
      if(s.available()>0)
      {root.printTo(s);}
    }
  } 
}
