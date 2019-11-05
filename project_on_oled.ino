#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#define SSD1306_LCDHEIGHT 64

// OLED display TWI address
#define OLED_ADDR   0x3C

Adafruit_SSD1306 display(-1);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
const char* ssid = "mounica";
const char* password = "mounica7";


#define ORG "pj6mh5"
#define DEVICE_TYPE "nodemcu"
#define DEVICE_ID "123456"
#define TOKEN "123456789"


//-------------------------

//----------------------------
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char pubtopic[] = "iot-2/cmd/switch/fmt/Number";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

//-------------------------------------

WiFiClient wifiClient;
PubSubClient client(server, 1883, wifiClient);

//---------------------------------------------------


void setup()
{
  pinMode(D3,INPUT);//switch1
  pinMode(D4,INPUT);//switch2
  Serial.begin(115200);

  // initialize and clear display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  // display a line of text
  display.setTextSize(1);
  display.setTextColor(WHITE);
 Serial.println();

 Serial.print("Connecting to ");
 Serial.print(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 } 
 Serial.println("");
 
 Serial.print("WiFi connected, IP address: ");
 Serial.println(WiFi.localIP());
}

void loop() 
{
 int switch1=digitalRead(D3);
 int switch2=digitalRead(D4);
 display.setCursor(0,1);
 if(switch1==0)
 {
  Serial.print("KFC 50% off");
  display.print("KFC 50% off");
  display.display();
  display.clearDisplay();
 }
 if(switch2==0)
 {
  Serial.println("fbb sale");
  display.print("fbb sale");
  display.display();
  display.clearDisplay();
 }
 PublishData(switch1,switch2);
 //delay(1000);
}
int PublishData(int switch1, int switch2){
  if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"Switch1\":";
  payload += switch1;
  payload +=" ," " \"Switch2\":";
  payload += switch2;
  payload += "}}";
 Serial.print("Sending payload: ");   
 Serial.println(payload);
  
 if (client.publish(pubtopic, (char*) payload.c_str())) 
 {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}
 
