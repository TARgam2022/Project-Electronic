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

void dhtRead(){ 
  h = dht.readHumidity();  
    t = dht.readTemperature();
   hic = dht.computeHeatIndex(t, h, false);
  }

void soilRead(){
  humidityRaw = analogRead(fork);
  humidity = map(humidityRaw, 940, 490, 100, 0); 
  }

void resistorRead(){
     brightnessRaw = analogRead(resistor);
  brightness = map(brightnessRaw, 940, 490, 100, 0); 

  }

void loop() {

  dhtRead();
  soilRead();
  resistorRead();

  Serial.print("{'temp': ");
  Serial.print(t);
  Serial.print(",'humd': ");
  Serial.print(h);
  Serial.print(",'bright': ");
  Serial.print( brightness);
  Serial.print
  (",'s_humd': ");
  Serial.print(humidity);
  Serial.println("}");

  delay(100);

}
