#include <StringSplitter.h>

#include <DHT.h>
#include <DHT_U.h>


#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define fork_1 A0
#define fork_2 A2

#define pump 3

#define fan_1 4
#define fan_2 5

#define resistor A1

int t;
int h;
int hic;
int humidityRaw1;
int humidityRaw2;
int humidityAvg;
int humidity;
int brightnessRaw;
int brightness;
int brightnessRawR;
int delayCount ;

void setup() {
  Serial.begin(9600);
  pinMode(resistor, INPUT);
  pinMode(fork_1, INPUT);
  pinMode(fork_2, INPUT);
  pinMode(pump, OUTPUT);
  pinMode(fan_1, OUTPUT);
  pinMode(fan_2, OUTPUT);
   dht.begin();

   digitalWrite(pump, HIGH);
   delayCount = 0;
}



bool switchSerial = false;

void dhtRead(){ 
  h = dht.readHumidity();  
  t = dht.readTemperature();
  hic = dht.computeHeatIndex(t, h, false);
  }

int soilRead(){
  humidityRaw1 = analogRead(fork_1);
  humidityRaw2 = analogRead(fork_2);
  
  humidityAvg = humidityRaw1 + humidityRaw2 / 2;
  humidity = map(humidityAvg, 150, 1000, 100, 0); 
  
  return humidity;
  }

void resistorRead(){

     brightnessRaw = analogRead(resistor);
  brightness = map(brightnessRaw, 950, 0, 100, 0); 
  if(brightness > 100){
    brightness = 100;
  }
  }

void loop() {

  dhtRead();
  soilRead();
  resistorRead();


  if(Serial.available() > 0){
    String s = Serial.readString();
    s.trim();
    Serial.println(s);
    if( s == "START"){
      switchSerial = true;
    }else{
      StringSplitter *splitter = new StringSplitter(s, ';', 9);
      int itemCount = splitter->getItemCount();  

      int fanState = splitter->getItemAtIndex(0).toInt();
      int pumpState = splitter->getItemAtIndex(1).toInt();

      /*Serial.println(pumpState);
      Serial.println(fanState);*/
  
      if(pumpState == 1){
          digitalWrite(pump, LOW);
      }else{
          digitalWrite(pump, HIGH);
        }

        
      if(fanState == 1){
          digitalWrite(fan_1, HIGH);
          digitalWrite(fan_2, HIGH);
      }else{
          digitalWrite(fan_1, LOW);
          digitalWrite(fan_2, LOW);
        }
    }
  }
 
 
  if(switchSerial == true){
  
    //rojo es el 1
    if(delayCount == 500){
      delayCount = 0;
      Serial.print("{\"temp\": \"");
      Serial.print(t);
      Serial.print("\",\"humi\": \"");
      Serial.print(h);
      Serial.print("\", \"bright\": \"");
      Serial.print( brightness);
      Serial.print("\", \"sHumi_1\": \"");
      Serial.print(map(humidityRaw1, 500, 1000, 100, 0));
      Serial.print("\", \"sHumi_2\": \"");
      Serial.print(humidityRaw2);
      Serial.println("\"}");
    }
    delay(1);
    delayCount++;
  }
}
