
#include <Arduino.h>
#include <iostream>
#include <WiFi.h>
#include <PubSubClient.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

/* Add WiFi and MQTT credentials to credentials.h file */
#include "Credentials.h"

// MQTT Broker

const char *mqtt_broker = "test.mosquitto.org"; 
const char *topic = "MOISES/ESP3";
const char *mqtt_username = ""; 
const char *mqtt_password = ""; 
const int mqtt_port = 1883; 

//Variável MQTT
bool mqttStatus = 0;

//Objetos
WiFiClient espClient;
PubSubClient client(espClient);

//Protótipos
bool connectMQTT();
void callback(char *topic, byte * payload, unsigned int lenght);

WiFiServer server(80);

using namespace std;

#define RSSI_BUFFER_SIZE 16

int rssiBuffer[RSSI_BUFFER_SIZE];
int rssiBufferPos = 0;
int rssiBufferLength = 0;

int scanTime = 2; //In seconds
BLEScan* pBLEScan;

//In  this part, will be done the avarage of the RSSI
int getAverageRSSI(int newRSSI)
{
  rssiBuffer[rssiBufferPos] = newRSSI;

  rssiBufferPos++;
  rssiBufferLength++;

  if (rssiBufferPos >= RSSI_BUFFER_SIZE) rssiBufferPos = 0;
  if (rssiBufferLength >= RSSI_BUFFER_SIZE) rssiBufferLength = RSSI_BUFFER_SIZE;

  int sum = 0;
  for(short i = 0; i < rssiBufferLength; i++){
    sum += rssiBuffer[i];
  }

  return sum / rssiBufferLength;
}

//Use this to get the RSSI sign about some specific address
 BLEAddress targetDeviceAddress = BLEAddress("f2:dc:b9:79:16:22");
 vector<string> addresses = {};
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  public:

  void onResult(BLEAdvertisedDevice advertisedDevice) {
   if (advertisedDevice.getAddress().equals(targetDeviceAddress)){
      int rssi = advertisedDevice.getRSSI();
      int averageRSSI = getAverageRSSI(rssi);

      Serial.printf("%s, %d, %d \n", advertisedDevice.getAddress().toString().c_str(), rssi, averageRSSI);

    float b;

     b = (-56 - (averageRSSI));

     float a = b/21;

     float distance = pow(10, a);

     Serial.printf("Distance = %.2f Meters \n", distance);

     client.publish(topic, String(distance).c_str());
   }
  }
};

void setup() {
  Serial.begin(9600);
   Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value

  Serial.println();                      //
  Serial.print("Conectando-se a ");      //
  Serial.println(ssid);                  //
  WiFi.begin(ssid, password);            //inicializa WiFi, passando o nome da rede e a senha

  while(WiFi.status() != WL_CONNECTED)   //aguarda conexão (WL_CONNECTED é uma constante que indica sucesso na conexão)
  {
    delay(741);                          //
    Serial.print(".");                   //vai imprimindo pontos até realizar a conexão...
  }

  Serial.println("");                    //mostra WiFi conectada
  Serial.println("WiFi conectada");      //
  Serial.println("Endereço de IP: ");    //
  Serial.println(WiFi.localIP()); 

  mqttStatus = connectMQTT();
}

void loop() {

    BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.println("Scan done!");
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(2000);

  if (mqttStatus) {
    client.loop();
  }

  while(WiFi.status() != WL_CONNECTED)   //aguarda conexão (WL_CONNECTED é uma constante que indica sucesso na conexão)
  {
    WiFi.begin(ssid, password);          //inicializa WiFi, passando o nome da rede e a senha
    Serial.print(".");                   //vai imprimindo pontos até realizar a conexão...

    delay(741);                          //741ms
  }
}

MyAdvertisedDeviceCallbacks callbacks;

bool connectMQTT() {
  byte tentativa = 0;
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);

  do {
    String client_id = "TESTE-";
    client_id += String(WiFi.macAddress());

    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Exito na conexão:");
      Serial.printf("Cliente %s conectado ao Broker\n", client_id.c_str());
    } else {
      Serial.print("Falha ao conectar: ");
      Serial.print(client.state());
      Serial.println();
      Serial.print("Tentativa: ");
      delay(2000);
    }
    tentativa++;
  } while (!client.connected() && tentativa < 5);

  if (tentativa < 5) {
    //Publish and Subscribe
                              //Parte em que publica a informações no tópico
    client.subscribe(topic); //Para fazer a leitura das informações do tópico
    return 1;
  } else {
  Serial.println("Não conectado");
  return 0;
  }
}

//Chama essa função toda vez que houver uma informação no tópico subscrito
void callback(char *topic, byte * payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("----------------------------");
}