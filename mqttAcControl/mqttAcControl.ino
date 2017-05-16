#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <IRremoteESP8266.h>

int khz = 38; // 38kHz carrier frequency for both NEC and Samsung

IRsend irsend(D2);

int AC_INPUT = 1; // digital input pin, active LOW logic
int AC_VAL = 0;    // variale to store status of AC_INPUT pin
int AC_STATE = 1; // initializing  ac_state variable such that when RPi code is started, it helps arduino to turn the IDUs OFF.

int ledPin = 1;
// Hitachi VRF - SEIL Lab
unsigned int Signal_ON[] = {3488, 1584, 548, 1184, 520, 348, 520, 348, 544, 348, 520, 344, 524, 344, 520, 372, 524, 344, 520, 348, 544, 348, 544, 324, 520, 344, 548, 1188, 544, 320, 548, 320, 572, 320, 548, 320, 544, 324, 544, 348, 544, 320, 548, 320, 572, 320, 548, 320, 544, 320, 524, 372, 544, 320, 548, 320, 548, 344, 520, 348, 520, 348, 544, 1188, 520, 348, 548, 1156, 524, 1212, 544, 1160, 548, 1160, 548, 1184, 548, 1160, 548, 320, 544, 1188, 520, 1188, 548, 1160, 544, 1188, 520, 1188, 520, 1188, 520, 1212, 544, 1164, 520, 1184, 548, 348, 544, 320, 548, 320, 548, 344, 548, 320, 548, 320, 544, 348, 544, 324, 544, 320, 572, 1164, 516, 1188, 520, 348, 544, 348, 520, 1188, 520, 1188, 544, 1188, 544, 1164, 544, 320, 548, 348, 544, 1160, 548, 1160, 572, 320, 548, 320, 544, 324, 544, 1188, 520, 348, 520, 344, 548, 1188, 520, 344, 520, 348, 520, 372, 520, 1188, 520, 344, 524, 1212, 520, 1188, 544, 320, 576, 1160, 520, 1184, 548, 1160, 544, 348, 520, 1188, 520, 348, 544, 348, 544, 324, 520, 344, 520, 376, 520, 344, 520, 348, 544, 348, 520, 1188, 520, 1188, 544, 1188, 544, 1168, 516, 1188, 544, 1188, 544, 1160, 548, 1160, 576, 1160, 520, 1184, 524, 1184, 548, 1188, 544, 1160, 520, 348, 544, 348, 520, 348, 544, 320, 528, 368, 520, 344, 548, 320, 548, 344, 548, 1160, 544, 1164, 572, 1160, 548, 1160, 520, 1188, 544, 1188, 520, 1188, 520, 344, 548, 348, 544, 320, 520, 348, 520, 372, 520, 348, 520, 348, 544, 348, 544, 1164, 520, 1184, 544, 1188, 548, 320, 548, 1160, 520, 1212, 548, 320, 548, 320, 520, 372, 520, 1184, 524, 344, 548, 1188, 520, 344, 524, 344, 548, 1184, 548, 1160, 520, 1188, 544, 348, 544, 1164, 544, 1164, 572, 320, 544, 320, 548, 320, 548, 344, 520, 348, 520, 344, 548, 348, 520, 344, 548, 1160, 520, 1212, 524, 1188, 544, 1160, 520, 1212, 520, 1188, 572, 1136, 548, 344, 548, 320, 548, 316, 548, 348, 544, 1160, 524, 344, 572, 320, 548, 1160, 520, 1188, 520, 1212, 520, 1188, 520, 1188, 544, 348, 544, 1164, 544, 1160, 548, 320, 548};
unsigned int Signal_OFF[] = {3488, 1608, 544, 1160, 524, 372, 544, 320, 548, 320, 520, 372, 520, 348, 548, 320, 544, 348, 520, 344, 524, 344, 548, 344, 520, 348, 548, 1160, 544, 348, 544, 324, 520, 344, 548, 348, 544, 320, 572, 296, 520, 372, 520, 348, 544, 320, 548, 348, 544, 320, 520, 348, 548, 344, 520, 348, 520, 348, 544, 348, 544, 324, 520, 1184, 548, 348, 516, 1192, 544, 1160, 520, 1212, 548, 1160, 544, 1164, 520, 1212, 520, 348, 548, 1160, 544, 1188, 520, 1188, 520, 1184, 524, 1212, 544, 1164, 520, 1184, 524, 1212, 520, 1188, 544, 320, 548, 344, 548, 320, 548, 320, 544, 348, 544, 324, 544, 320, 548, 344, 520, 348, 548, 1160, 520, 1212, 520, 348, 548, 320, 520, 1212, 548, 1160, 544, 1160, 548, 1188, 520, 344, 524, 344, 548, 1184, 520, 1188, 548, 320, 548, 344, 520, 352, 544, 1160, 520, 372, 520, 344, 524, 1184, 548, 344, 520, 348, 520, 348, 544, 1188, 520, 348, 544, 1160, 524, 1212, 520, 344, 548, 1160, 548, 1188, 544, 1160, 548, 320, 520, 1212, 520, 348, 520, 348, 520, 372, 524, 340, 548, 320, 524, 372, 520, 344, 520, 348, 544, 1188, 520, 1188, 520, 1188, 544, 1188, 520, 1188, 520, 1188, 572, 1160, 520, 1184, 548, 1160, 524, 1212, 520, 1184, 524, 1184, 548, 1188, 516, 348, 548, 320, 544, 348, 520, 348, 520, 344, 548, 348, 520, 344, 548, 320, 548, 1184, 548, 1160, 548, 1160, 548, 1184, 520, 348, 548, 320, 520, 372, 520, 1188, 520, 344, 544, 348, 520, 348, 520, 348, 544, 1188, 520, 1188, 520, 1188, 544, 348, 520, 1184, 548, 1160, 524, 372, 520, 1184, 548, 1160, 520, 372, 548, 320, 544, 324, 520, 1212, 520, 348, 520, 1184, 524, 372, 544, 320, 544, 1164, 544, 1188, 520, 1188, 520, 348, 572, 1160, 520, 1188, 548, 320, 544, 348, 516, 348, 548, 320, 520, 372, 520, 348, 544, 324, 520, 372, 544, 1164, 520, 1184, 520, 1216, 516, 1192, 544, 1160, 548, 1188, 544, 1160, 520, 348, 548, 344, 520, 348, 548, 320, 544, 348, 520, 1184, 524, 344, 544, 1188, 524, 1184, 520, 1188, 544, 1188, 520, 1188, 520, 1188, 572, 320, 544, 1164, 548, 320, 544};




WiFiServer server(80);


void turn_on_AC()
{
  Serial.println("Turning ON");
  irsend.sendRaw(Signal_ON, sizeof(Signal_ON) / sizeof(int), khz); // Sends ON code
}

void turn_off_AC()
{
  Serial.println("Turning OFF");
  irsend.sendRaw(Signal_OFF, sizeof(Signal_OFF) / sizeof(int), khz); // Sends OFF Code
}



// Update these with values suitable for your network.
const char* ssid = "Redmi";
const char* password = "dashdash";
const char* mqtt_server = "m12.cloudmqtt.com";
//const char* mqtt_server = "iot.eclipse.org";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {
   delay(100);
  // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


    // Start the server
  server.begin();
  Serial.println("Server started");

}

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Command is : [");
  Serial.print(topic);
  int p =(char)payload[0]-'0';
Serial.print(p);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(800);
  digitalWrite(LED_BUILTIN, LOW);
if(p==1){
    digitalWrite(ledPin, HIGH);
    turn_on_AC();
  
  }

 if(p==0){
  
  digitalWrite(ledPin, LOW);
    turn_off_AC();
  }
  Serial.println();
} //end callback

void reconnect() {
  // Loop until we're reconnected

if(WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      wifiConnectBlink();
      Serial.print(".");
    }
  
  
  }

  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    //if you MQTT broker has clientID,username and password
    //please change following line to    if (client.connect(clientId,userName,passWord))
    if (client.connect(clientId.c_str(),"uyjzyogt","dZg-yLbxqqcW"))
    {
      Serial.println("connected");
     //once connected to MQTT broker, subscribe command if any
      client.subscribe("testaccontrol");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 6 seconds before retrying
      delay(6000);
    }
  }
} //end reconnect()

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 12296);
  client.setCallback(callback);

  irsend.begin();
  

  //WiFiServer server(80);

  delay(10);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
 
}

void wifiConnectBlink(){
//  digitalWrite(LED_BUILTIN, HIGH);
//  delay(400);
//  digitalWrite(LED_BUILTIN, LOW);
  
  }


void mqttConnectBlink(){
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  
  }

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();




    // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
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
    turn_on_AC();
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, LOW);
    turn_off_AC();
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

  client.print("Led pin is now: ");

  if (value == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");
  client.println("</html>");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

}
