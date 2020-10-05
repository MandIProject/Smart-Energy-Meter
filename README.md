# Smart-Energy-Meter
Smart alternative to traditional energy meters currently in use. First prototype, a lot of work is to be done. Initial prototype is successful and works accurately for resistive loads.
# Abstract
Measurement of AC/DC current drawn and power consumed by devices using ACS712 sensor and Arduino nano and calculating the energy used during operation by recording the time for which the device was working.
For AC - power can be measured easily once current is calculated since normal household supply voltage in the sockets is 220V - 240V. For DC we will use a voltage divider circuit using resistors or a voltage sensor connected to the Arduino analog pin to measure the voltage.
Then we will send the measured data to a smartphone and a website(thingspeak.com) over WIFI and internet.
Visualization of the recorded data will be in the form of graphs and numeric displays on the website using MATLAB analytics.
Data will also be displayed on an LCD screen connected to the circuit.
The data can also be recorded in excel sheet using MATLAB analytics, after it is sent to the website.
# Scope
Can be used in DC machines to monitor the power generated/used and current drawn in real time and diagnose any fault happening during operation of the machine.
In solar panels to monitor the power generated, the incoming voltage and current.
In homes we can measure the ac current drawn and power consumed by appliances.
Can also be used as smart over-current protection device.
Smart alternative to traditional energy meters
# Components Used
Arduino Nano
D1 Mini Pro node MCU V2
ACS712 20A Sensor
Connecting Wires
20 SWG Wire
12 V DC Adapter
Multi-meter
Soldering Iron and Solder Wire
1kΩ and 2kΩ resistors
60W, 100W and 200W bulbs
# Software stack
<ul>
  <li>Arduino IDE</li>
  <li>MIT App Inventor</li>
  <li>Google Firebase</li>
  <li>Microsoft Excel</li>
</ul>
# ACS712 sensor
Works on the principle of hall effect. An output voltage is generated proportional to the current flowing through the sensor. Our system can measure current up to 20A.
<div>
  <img src="https://upload.wikimedia.org/wikipedia/en/1/19/Hall_Effect_Measurement_Setup_for_Electrons.png">
  <img src="http://luckyretail.com/Uploadfile/2015.1.30/036694/036694.jpg" height="184" width="285">
</div>

# 1st Problem
The alternating current generates a sine wave. The values vary over a period of time from negative to positive. Therefore, the RMS value of the current is considered. Now, the ACS712 sensor needs 5V DC to function properly. The sensor keeps 2.5V as the mid-point and measures the proportional voltage around this mid-point. So, the range varies from 0-5V in the output of the sensor with 2.5V as mid-point, which corresponds to the 0-point.
When we give alternating current as input to the sensor, the sensor measures the proportional analog voltage around the mid-point of 2.5V. So, we get all varying values of the current, this data is useless to us. Suppose we applied 8A as input then changed it to 10A, the change in value won’t reflect in the analog output of the sensor.
<div><img src="https://cdn.instructables.com/ORIG/FNO/T49W/IM3OBJTZ/FNOT49WIM3OBJTZ.jpg?auto=webp&frame=1&fit=bounds&md=1d0e924348a811382f415302bb6e85a1"></div>
Image source - https://www.instructables.com/Simplified-Arduino-AC-Current-Measurement-Using-AC/#:~:text=%20Simplified%20Arduino%20AC%20Current%20Measurement%20Using%20ACS712,6%20Step%206%3A%20Optional%20Sensor%20Calibration%20More%20
Project on Instructables By https://www.instructables.com/member/e024576/

# Solution to 1st problem
To overcome this problem, we tried to measure the standard deviation of input current around the 2.5V midpoint. We took 20 readings per second, then measured their standard deviations and then averaged them. In this way, we measured the fluctuations in the analog output of the sensor.
Then we took 3 blubs of 60W, 100W, and 200W and then connected them to the sensor one by one, to measure the output standard deviations which the sensor was sending to the Arduino. Each bulb was switched on for about a minute and the respective values were recorded. Then we copied the values to an excel sheet. A digital multimeter was connected in series with the bulbs to know the actual RMS currents. Similarly, the current values were noted from the digital multimeter and copied to the excel sheet. In the excel sheet, we plotted a graph between the current and the standard deviations and found the line of best fit. The graph gave us an approximate equation relating to the two data variable.

# Code for finding the standard deviations
```Arduino
#include <Filters.h>
#include <LiquidCrystal.h>

float testFrequency = 50;                     // test signal frequency (Hz)
float windowLength = 20.0/testFrequency;     // how long to average the signal, for statistist
int sensorValue = 0;
float current_amps; // actual measure current

unsigned long printPeriod = 1000; // in milliseconds
// Track time in milliseconds since last reading 
unsigned long previousMillis = 0;

void setup() 
{
  Serial.begin( 57600 );
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
      Serial.print( inputStats.sigma() );
    }
  }
}
```
