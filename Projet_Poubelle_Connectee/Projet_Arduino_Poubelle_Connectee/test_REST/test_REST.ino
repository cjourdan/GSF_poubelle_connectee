
#include <PubSubClient.h>
#include "WiFiEsp.h"

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(10, 11); // RX, TX
#endif

// Initialisation des entrées et des sorties de la carte Arduino
#define trigPin 13
#define echoPin 12
#define ledverte 5
#define ledrouge 7
#define ledjaune 6
#define button 3

long EMPTY = 1;                       //Empty distance, in CM
long SECOND_TIER = EMPTY / 3;
long FIRST_TIER = 2*SECOND_TIER;

char ssid[] = "AndroidAP";            // your network SSID (name)
char pass[] = "jsjr0226";
/*
char ssid[] = "GSF_WifiAssistanceMicro";            // your network SSID (name)
char pass[] = "jeveuxmettrecettemachineajour:)";        // your network password*/
int status = WL_IDLE_STATUS;     // the Wifi radio's status
int ultrason_state = 0;
int ultrason_previousState = 0;

// Paramètres concernant bluemix
#define ORG "2cdq0k"
#define DEVICE_TYPE "UltraSoundSensorGSF"
#define DEVICE_ID "SensorGSF01"
#define TOKEN "tokenGSF01Sensor01"
//-------- Customise the above values --------

//char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/status/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

int counter = 0;
String ID = "";

char server[] = "192.168.43.164";

WiFiEspClient wifiClient;


void setup() {
  // initialize serial for debugging
  Serial.begin(115200);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledverte, OUTPUT);
  pinMode(ledrouge, OUTPUT);
  pinMode(ledjaune, OUTPUT);

   if (wifiClient.connect(server, 8080)) {
    Serial.println("sending to server");
    // Make a HTTP request:
    wifiClient.println("GET /WebAppGSF/home?subscribe=true HTTP/1.1");
    wifiClient.println("Host: 10.10.20.177");
    wifiClient.println("Connection: close");
    wifiClient.println();
  }

  int counter = 0;
  while (wifiClient.available()) {
    char c = wifiClient.read();
    if(counter>=6){
      ID+=c;
    }
    if(c == '\n'){
      counter++;
    }
  }
  Serial.println(ID);
}

void loop() {
  // if there are incoming bytes available 
  // from the server, read them and print them:
  while (wifiClient.available()) {
    char c = wifiClient.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client:
  if (!wifiClient.connected()) {
    wifiClient.stop();
  }

  
 /////////////////////////////// ULTRASON ///////////////////////////////
 long duration, distance;
  digitalWrite(trigPin, LOW);  
  delayMicroseconds(1); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  
  long switchState = digitalRead(button);

   if(switchState == 1){
      EMPTY = distance;
      SECOND_TIER = EMPTY / 3;
      FIRST_TIER = 2*SECOND_TIER;
      
      Serial.print(EMPTY);
      Serial.println(" CLICK ETALONNAGE");

      digitalWrite(ledverte,LOW);               
      digitalWrite(ledjaune,LOW);
      digitalWrite(ledrouge,LOW);

      delay(100);

      digitalWrite(ledverte,HIGH);               
      digitalWrite(ledjaune,HIGH);
      digitalWrite(ledrouge,HIGH);

      delay(100);

      digitalWrite(ledverte,LOW);                
      digitalWrite(ledjaune,LOW);
      digitalWrite(ledrouge,LOW);
   }

  if (distance >= EMPTY) {                      // Si distance est plus petite que 20 cm
    digitalWrite(ledverte,LOW);                	// on allume la led verte
    digitalWrite(ledjaune,LOW);					// on allume la led jaune
    digitalWrite(ledrouge,LOW);                 // on éteint la led rouge

    ultrason_state = 0;
    
    Serial.print(distance);
    Serial.println(" poubelle vide");
  
  }else if( distance < EMPTY && distance >= FIRST_TIER){

    digitalWrite(ledverte,HIGH);                // on allume la led verte
    digitalWrite(ledjaune,LOW);					// on éteint la led jaune
    digitalWrite(ledrouge,LOW);                 // on éteint la led rouge

    ultrason_state = 1;
    
    Serial.print(distance);
    Serial.println(" poubelle 1st tier");
       
  }else if(distance < FIRST_TIER && distance >= SECOND_TIER){
    digitalWrite(ledverte,HIGH);                // on allume la led verte
    digitalWrite(ledjaune,HIGH);				// on allume la led jaune
    digitalWrite(ledrouge,LOW);                 // on éteint la led rouge

    ultrason_state = 2;
    
    Serial.print(distance);
    Serial.println(" poubelle 2nd tier");
       

  }else{

    Serial.print(distance);
    Serial.println(" poubelle pleine");

    ultrason_state = 3;
    
    digitalWrite(ledverte,HIGH);                // on allume la led verte
    digitalWrite(ledjaune,HIGH);				// on allume la led jaune
    digitalWrite(ledrouge,HIGH);  				// on allume la led rouge
              
  }

   /////////////////////////////// ULTRASON ///////////////////////////////
 delay(3000);

 if(ultrason_state != ultrason_previousState){
     ultrason_previousState = ultrason_state;

    if (wifiClient.connect(server, 8080)) {
    Serial.println("sending to server");
    // Make a HTTP request:
    Serial.println("GET /WebAppGSF/home?upload="+ID+":"+String(distance)+":"+String(EMPTY)+" HTTP/1.1");
    wifiClient.println("GET /WebAppGSF/home?upload="+ID+":"+String(distance)+":"+String(EMPTY)+" HTTP/1.1");
    wifiClient.println("Host: 10.10.20.177");
    wifiClient.println("Connection: close");
    wifiClient.println();
  }
  }
}
