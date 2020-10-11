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
<ul>
  <li>Arduino Nano</li>
  <li>D1 Mini Pro node MCU V2</li>
  <li>ACS712 20A Sensor</li>
  <li>ZMPT101B-sinlge phase AC voltage sensor</li>
  <li>Connecting Wires</li>
  <li>20 SWG Wire</li>
  <li>12 V DC Adapter</li>
  <li>Multi-meter</li>
  <li>Soldering Iron and Solder Wire</li>
  <li>1kΩ and 2kΩ resistors</li>
  <li>60W, 100W and 200W bulbs</li>
</ul>

# Software stack
<ul>
  <li>Arduino IDE</li>
  <li>MIT App Inventor</li>
  <li>Google Firebase</li>
  <li>Microsoft Excel</li>
</ul>

# Circuit Diagram
<div>
  <img src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/2020_10_03%207_17%20pm%20Office%20Lens.jpg">
  <img src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/Circuit.jpg">
</div>

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
<div>
  <img src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/pic-5.jpg">
</div>

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

# 2nd problem and it's solution
In our project, we are sending the data through the internet to our database, so that our website and the mobile app can access that data. Arduino as such does not have a WIFI chip inbuilt. So, we had to use a separate WIFI chip (ESP8266 node MCU) in serial communication with the Arduino board, to send the data to the database. Here, one could ask why didn’t we just connect the sensor to the node MCU. The problem was, node MCU provides 3.3V max as output voltage for peripherals but the sensor needed at least 5V DC as input for proper working. Therefore, we used two boards here, Arduino for reading and computing the sensor values, and node MCU for sending data to the database.

# Communication between Arduino and Wemos D1 Mini Pro
The data from the Arduino is sent to the node MCU in JSON format (JavaScript Object Notation) through serial communication. It is a format that uses key-value pairs just like a dictionary. JSON is platform-independent and is lightweight. Hence, communication between Arduino and node MCU becomes faster. Since the system works in real-time, it needs to run fast as possible.

# Code for 2nd problem
```Arduino
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
```

# 3rd problem and it's solution
Connecting an LCD to the circuit, dropped the voltage input to the sensor from 5V to 4.7V DC. Hence, there were significant errors in the data obtained.
So, for now, we have removed the LCD screen from the circuit. To access the output values, we are directly using the serial monitor provided in Arduino IDE.

# Final Code
```Arduino
//For Arduino nano
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

//For Wemos D1 mini pro
#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "Your project name on firebase"
#define FIREBASE_AUTH "Your auth key"
#define WIFI_SSID "Your wifi ssid"
#define WIFI_PASSWORD "Your wifi password"
String apiKey = "Your thingspeak api key";     //  Enter your Write API key from ThingSpeak
const char* server1 = "api.thingspeak.com";

WiFiClient client;

float data1;
float data2;
float data3;
float data4;

void setup() 
{
  s.begin(9600);
  Serial.begin(9600);
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() 
{
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(s);
  if (root == JsonObject::invalid())
  {return;}
  Serial.println("JSON received and parsed");
  root.prettyPrintTo(Serial);
  Serial.print("Data 1 ");
  Serial.println("");
  data1=root["data1"];
  Serial.print(data1);
  Serial.print("   Data 2 ");
  data2=root["data2"];
  Serial.print(data2);
  Serial.print("          Data 3 ");
  data3=root["data3"];
  Serial.print(data3);
  Serial.print("                 Data 4 ");
  data4=root["data4"];
  Serial.print(data4);
  Serial.println("");
  Serial.println("---------------------xxxxx--------------------");
  Firebase.setFloat("Current", data1);
  Firebase.setFloat("Power", data2);
  Firebase.setFloat("RawVal", data3);
  Firebase.setFloat("Energy", data4);
  if(data1==0.00)
  {
    Firebase.setString("Status", "OFF");
  }
  else
  {
    Firebase.setString("Status", "ON");
  }
  if (client.connect(server1,80))   //   "184.106.153.149" or api.thingspeak.com
  {
         String postStr = apiKey;
         postStr += "&field1=";
         postStr += double(data1);
         postStr += "\r\n\r\n";
         postStr += "&field2=";
         postStr += double(data3);
         postStr += "\r\n\r\n";
         postStr += "&field3=";
         postStr += double(data2);
         postStr += "\r\n\r\n";
         postStr += "&field4=";
         postStr += double(data4);
         postStr += "\r\n\r\n";

         client.print("POST /update HTTP/1.1\n");
         client.print("Host: api.thingspeak.com\n");
         client.print("Connection: close\n");
         client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
         client.print("Content-Type: application/x-www-form-urlencoded\n");
         client.print("Content-Length: ");
         client.print(postStr.length());
         client.print("\n\n");
         client.print(postStr);
  }
  client.stop();
}
```

<div>
  <img src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/IMG_20200903_005648.jpg">
  <img src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/IMG_20200828_225250.jpg">
  <img src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/IMG_20200828_223402.jpg">
  <img src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/IMG_20200828_223356.jpg">
</div>

# Our app made using MIT App Inventor
<div>
  <img src="https://raw.githubusercontent.com/MandIProject/Smart-Energy-Meter/main/Screenshot_2020-10-05-22-34-49-120_appinventor.ai_routpratik2001.Smart_Current_and_Power_Monitoring_System.jpg">
</div>

# Next Steps
As of now the prototype can measure current and power of resistive loads on AC supply accurately as well as DC loads but that is not our goal. We are currently trying to design a circuit to measure power factor, so that we will be able to measure power drawn by inductive and capacitive loads. The final system will be able to measure current draw, power and energy consumption of all types of loads as well as power factor. The system will show the apparent power and the real power. Then we will expand this sytem to three phase systems and loads.
