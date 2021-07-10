#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "YOUR FIREBASE PROJECT DATABASE URL" // firebase project name
#define FIREBASE_AUTH "DATABASE AUTH KEY" // firebase realtime database auth key
#define WIFI_SSID "YOUR WIFI SSID" // my home wifi ssid
#define WIFI_PASSWORD "YOUR WIFI PASSWORD" // my home wifi password
//String apiKey = "THINGSPEAK API KEY";     //  Enter your Write API key from ThingSpeak
//const char* server1 = "api.thingspeak.com"; // thingspeak server

WiFiClient client;

int ledPin = D7;
WiFiServer server(80);

float data1;
float data2;
float data3;
float data4;

void setup() 
{
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);//We start with the led turned off.
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

  server.begin();
  Serial.println("NodeMCU Server started");//server has started
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() 
{
  webPage();
  firebaseGoogle();
}

void firebaseGoogle()
{
  // receiving data from arduino nano through serial communication
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(s);
  if (root == JsonObject::invalid())
  {return;}
  Serial.println("JSON received and parsed");
  root.prettyPrintTo(Serial);
  Serial.println("");
  Serial.print("Data 1 ");
  data1=root["data1"];
  Serial.print(data1);
  Serial.print(" Data 2 ");
  data2=root["data2"];
  Serial.print(data2);
  Serial.print("  Data 3 ");
  data3=root["data3"];
  Serial.print(data3);
  Serial.print("   Data 4 ");
  data4=root["data4"];
  Serial.print(data4);
  Serial.println("");
  Serial.println("---------------------xxxxx--------------------");
  // sending data to google firebase;
  Firebase.setFloat("Current", data1);
  Firebase.setFloat("Voltage", round(data2));
  Firebase.setFloat("Power", data3);
  Firebase.setFloat("Energy", data4);
  if(data1==0.00)
  {
    Firebase.setString("Status", "OFF");
  }
  else
  {
    Firebase.setString("Status", "ON");
  }
}

//void thingSpeak()
//{
//  //  thingspeak connection for realtime graphs and other information
//  if (client.connect(server1,80))   //   "184.106.153.149" or api.thingspeak.com
//  {
//         String postStr = apiKey;
//         postStr += "&field1=";
//         postStr += double(data1);
//         postStr += "\r\n\r\n";
//         postStr += "&field2=";
//         postStr += double(data3);
//         postStr += "\r\n\r\n";
//         postStr += "&field3=";
//         postStr += double(data2);
//         postStr += "\r\n\r\n";
//         postStr += "&field4=";
//         postStr += double(data4);
//         postStr += "\r\n\r\n";
//
//         client.print("POST /update HTTP/1.1\n");
//         client.print("Host: api.thingspeak.com\n");
//         client.print("Connection: close\n");
//         client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
//         client.print("Content-Type: application/x-www-form-urlencoded\n");
//         client.print("Content-Length: ");
//         client.print(postStr.length());
//         client.print("\n\n");
//         client.print(postStr);
//  }
//  client.stop();
//}

void webPage()
{
  // receiving data from arduino nano through serial communication
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(s);
  if (root == JsonObject::invalid())
  {return;}
  Serial.println("JSON received and parsed");
  root.prettyPrintTo(Serial);
  Serial.println("");
  Serial.print("Data 1 ");
  data1=root["data1"];
  Serial.print(data1);
  Serial.print(" Data 2 ");
  data2=root["data2"];
  Serial.print(data2);
  Serial.print("  Data 3 ");
  data3=root["data3"];
  Serial.print(data3);
  Serial.print("   Data 4 ");
  data4=root["data4"];
  Serial.print(data4);
  Serial.println("");
  Serial.println("---------------------xxxxx--------------------");
  
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  while(!client.available()){
    delay(1);
  }

   // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

   // Match the request
 
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
 
  // Set ledPin according to the request
  //digitalWrite(ledPin, value);
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta charset='UTF-8'>");
  client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
  client.println("<link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.5.3/css/bootstrap.min.css' integrity='sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2' crossorigin='anonymous'>");
  client.println("</head>");
  client.println("<body>");
  client.println("<center>");
  client.println("<h1>Appliance Status</h1>");
  client.println("<p>M&I Project: Smart Power and Current Measurement System</p>");
 
  client.print("Relay is now: ");
 
  if(value == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button type='button' class='btn btn-success'>Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button type='button' class='btn btn-danger'>Turn Off </button></a>");
  client.println("<p>Current: ");
  client.print(data1);
  client.println("</p>"); 
  client.println("<p>Voltage: ");
  client.print(data2);
  client.println("</p>");
  client.println("<p>Power: ");
  client.print(data2);
  client.println("</p>"); 
  client.println("<p>Energy: ");
  client.print(data4);
  client.println("</p>");
  client.println("</center>"); 
  client.println("<script src='https://code.jquery.com/jquery-3.5.1.slim.min.js' integrity='sha384-DfXdz2htPH0lsSSs5nCTpuj/zy4C+OGpamoFVy38MVBnE+IbbVYUew+OrCXaRkfj' crossorigin='anonymous'></script>");
  client.println("<script src='https://stackpath.bootstrapcdn.com/bootstrap/4.5.3/js/bootstrap.bundle.min.js' integrity='sha384-ho+j7jyWK8fNQe+A12Hb8AhRq26LrZ/JpcUGGOn+Y7RsweNrtN/tE3MoK7ZeZDyx' crossorigin='anonymous'></script>");
  client.println("</body>");
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
