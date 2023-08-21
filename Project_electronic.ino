#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define fork A0

#define resistor A1

void setup() {
  Serial.begin(9600);
  pinMode(resistor, INPUT);
  pinMode(fork, INPUT);
   dht.begin();
}

int t;
int h;
int hic;
int humidityRaw;
int humidity;
int brightnessRaw;
int brightness;
int brightnessRawR;

bool switchSerial = false;

void dhtRead(){ 
  h = dht.readHumidity();  
  t = dht.readTemperature();
  hic = dht.computeHeatIndex(t, h, false);
  }

void soilRead(){
  humidityRaw = analogRead(fork);
  humidity = map(humidityRaw, 512, 0, 100, 0); 
  }

void resistorRead(){

     brightnessRaw = analogRead(resistor);
  brightness = map(brightnessRaw, 900, 0, 100, 0); 
  if(brightness > 100){
    brightness = 100;
  }
  }

void loop() {
 
   dhtRead();
  soilRead();
  resistorRead();
while (Serial.available()){}
String s = Serial.readString();
s.trim();
Serial.print(s);
if( s == "START"){
  switchSerial = true;
  }
 
 
if(switchSerial == true){
  
  Serial.print("{\"temp\": \"");
  Serial.print(t);
  Serial.print("\",\"humi\": \"");
  Serial.print(h);
  Serial.print("\", \"bright\": \"");
  Serial.print( brightness);
  Serial.print("\", \"sHumi\": \"");
  Serial.print(humidity);
  Serial.println("\"}");
  delay(1000);
}
}
