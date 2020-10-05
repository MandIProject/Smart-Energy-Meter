#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "smart-meter-bc378.firebaseio.com"
#define FIREBASE_AUTH "cmLhyNXjNPwCWvaqNKTKu5e3ANmKFAoU9T5MAgMI"
#define WIFI_SSID "Home_wifi2ghz"
#define WIFI_PASSWORD "Qwerty7861;"
String apiKey = "M4H5Y1U303ET9WUD";     //  Enter your Write API key from ThingSpeak
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
