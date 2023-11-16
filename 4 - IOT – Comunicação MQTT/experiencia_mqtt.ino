#include "Arduino.h"
#include <LiquidCrystal.h>
#include <Adafruit_MPU6050.h>    //You need to install this library
#include <Adafruit_SSD1306.h>    //You need to install this library
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

#define BUZZER  12
#define LED_R   2      //LED RGB-red
#define LED_G   15     //LED RGB-verde
#define LED_B   0      //LED RGB-azul
#define LED_W   17     //LED branco
#define LED_V   16     //LED vermelho 
#define LED_v   4      //LED verde
#define DHT_PIN   14   
#define TRIGGER   33   
#define ECHO  27       
#define LDR 34         
#define SERVO_1 25
#define SERVO_2 26  
#define OLED_MOSI  23
#define OLED_CLK   18
#define OLED_DC    19
#define OLED_CS    5
#define OLED_RESET -1
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display heigh, in pixels
#define DHTTYPE DHT11

// WiFi
const char *ssid = "PipeWiFi"; // Nome da rede WiFi
const char *password = "pipe2000";  // Senha da rede WiFi 

// MQTT Broker
const char *mqtt_broker = ""; // Endereço do broker

// DEFINIÇÃO DOS TÓPICOS
const char *tLEDR = "EmbSys/NODERED/LEDR";
const char *tLEDG = "EmbSys/NODERED/LEDG";
const char *tLEDB = "EmbSys/NODERED/LEDB";
const char *tLEDW = "EmbSys/NODERED/LEDW";
const char *tLEDV = "EmbSys/NODERED/LEDV";
const char *tLEDv = "EmbSys/NODERED/LEDv";
const char *tBUZZ = "EmbSys/NODERED/BUZZ";
const char *tMOT1 = "EmbSys/NODERED/MOT1";
const char *tMOT2 = "EmbSys/NODERED/MOT2";
const char *tOLED = "EmbSys/NODERED/OLED";

const char *tTEMP = "EmbSys/ESP32/TEMP";
const char *tHUM = "EmbSys/ESP32/HUM";
const char *tLDR =  "EmbSys/ESP32/LDR";
const char *tGIRX = "EmbSys/ESP32/GIRX";
const char *tGIRY = "EmbSys/ESP32/GIRY";
const char *tGIRZ = "EmbSys/ESP32/GIRZ";
const char *tACLX = "EmbSys/ESP32/ACLX";
const char *tACLY = "EmbSys/ESP32/ACLY";
const char *tACLZ = "EmbSys/ESP32/ACLZ";
const char *tUDIS = "EmbSys/ESP32/UDIS";

// AUTENTICAÇÃO MQTT
const char *mqtt_username = "";   // Usuário do broker mqtt
const char *mqtt_password = "";   // password for authentication

WiFiClient espClient;                   // objeto manipulador do WiFi
PubSubClient client(espClient);  

Adafruit_MPU6050 mpu;                   // objeto do acelerômetro
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,  // objeto da tela OLED
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
DHT dht(DHT_PIN, DHTTYPE);              // objeto do sensor DHT
Servo servoMotor1;                      // objeto do servo1
Servo servoMotor2;                      // objeto do servo2

int findDistance(){
  // Essa função determina a distância a partir 
  // dos dados do sensor ultrassônico. 
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);
  long duration = pulseIn(ECHO, HIGH);
  return duration * 0.034 / 2;;
}

void buzzerOn(int delay_ms){
  //Função que ativa o BUZZER por um determinado número de milissegundos. 

  digitalWrite(BUZZER, HIGH);
  delay(delay_ms);
  digitalWrite(BUZZER, LOW);
}

void ledOnOff(char color, int on_off){
  //Função que liga e desliga os diferentes LEDs. 

  switch (color) {
  case 'R':
    digitalWrite(LED_R, on_off);
    break;
  case 'G':
    digitalWrite(LED_G, on_off);
    break;
  case 'B':
    digitalWrite(LED_B, on_off);
    break;
  case 'V':
    digitalWrite(LED_V, on_off);
    break;
  case 'v':
    digitalWrite(LED_v, on_off);
    break;
  case 'W':
    digitalWrite(LED_W, on_off);
    break;
  }
}

void printDisplay(String text){
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(text);
  display.display();
}


void setup_wifi() {
  delay(50);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String payloadR = "";
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    payloadR+=(char)payload[i];
  }
  if (strcmp(topic,tLEDR)==0){
    if(payloadR == "on") ledOnOff('R', 1);
    if(payloadR == "off") ledOnOff('R', 0);
  }

  //------------------------------------------------------------------------

  // Nesta seção, complete para todos os LEDs restantes 

  //------------------------------------------------------------------------
  if (strcmp(topic,tBUZZ)==0){
    buzzerOn(payloadR.toInt());
  }
  if (strcmp(topic,tMOT1)==0){
    servoMotor1.write(payloadR.toInt());
  }
  //------------------------------------------------------------------------

  // Nessa seção, complete para o segundo servomotor, bem como para o buzzer, usando a função "buzzerOn(delay)".

  //------------------------------------------------------------------------
  if (strcmp(topic,tOLED)==0){
    printDisplay(payloadR);
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client",mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe(tLEDR);
      client.subscribe(tLEDG);
      client.subscribe(tLEDB);
      client.subscribe(tLEDW);
      client.subscribe(tLEDV);
      client.subscribe(tLEDv);
      client.subscribe(tBUZZ);
      client.subscribe(tMOT1);
      client.subscribe(tMOT2);
      client.subscribe(tOLED);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void initPins(){
  pinMode(BUZZER, OUTPUT);
  pinMode(ECHO, INPUT); 
  pinMode(TRIGGER, OUTPUT);
  pinMode(OLED_CS, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(LED_W, OUTPUT);
  pinMode(LED_V, OUTPUT);
  pinMode(LED_v, OUTPUT);
}


void setup() {
  initPins();
  Serial.begin(115200);
  delay(1000);
  servoMotor1.attach(SERVO_1);
  servoMotor2.attach(SERVO_2);

  //------------------------------------------------------------------------

  // Nessa parte, inicie a conexão Wifi 
  // selecione o Broker e a porta a serem usados
  // adicionar a função de retorno de chamada

  //------------------------------------------------------------------------
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  if (!mpu.begin(0x69)) {
    Serial.println("Sensor mpu init failed");
    while (1)
      yield();
  }
  dht.begin();  
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  sensors_event_t a, g, temp, a2, g2, temp2;
  mpu.getEvent(&a, &g, &temp);
  int distance = findDistance(); 
  int ldrValue = analogRead(LDR);
  float h = dht.readHumidity();
  float t = dht.readTemperature();      //Em graus Celsius

  client.publish(tLDR, String(ldrValue).c_str());
  delay(100);
  //------------------------------------------------------------------------

  //Preencher a publicação de dados com os valores ausentes

  //------------------------------------------------------------------------
  delay(100);
  client.publish(tGIRX, String(g.gyro.x).c_str());
  delay(100);
  //------------------------------------------------------------------------

  // Completar a publicação dos dados do giroscópio (lembre-se de que o acelerômetro tem 3 eixos).

  //------------------------------------------------------------------------
  client.publish(tACLX, String(a.acceleration.x).c_str());
  delay(100);
  //------------------------------------------------------------------------

  // Completar a publicação dos dados do acelerômetro (lembre-se de que o acelerômetro tem 3 eixos).

  //------------------------------------------------------------------------
}

