#include <Adafruit_SGP30.h>
#include <DHT.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <time.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
extern "C" {
#include "libb64/cdecode.h"
}
#include "mycelium_aws.h"

#define WATERSENSORPIN A0
#define DHTTYPE DHT22

uint8_t DHTPin = 2;
DHT dht(DHTPin, DHTTYPE);

Adafruit_SGP30 sgp;

String sgp30_ID;
String esp8266_ID;

//int status = WL_IDLE_STATUS;
int tick=0;
int SPGcounter = 0;

MyCeliumAWS mycel_aws = new MyCeliumAWS();


void setup() {
  Serial.begin(115200); 
  mycel_aws.setup();


  if (! sgp.begin()){
    Serial.println("Sensor not found :(");
    while (1);
  }
  sgp30_ID = String(sgp.serialnumber[0], HEX) + String(sgp.serialnumber[1], HEX) + String(sgp.serialnumber[2], HEX);
  sgp30_ID.toUpperCase();
  Serial.println("Found SGP30 serial 0x"+ sgp30_ID);
  esp8266_ID = String(ESP.getChipId());
  t = String("Air-temp Level");
  h = String("Air-humidity Level");
  c = String("CO2 Level");
  l = String("Liquid Level");
// If you have a baseline measurement from before you can assign it to start, to 'self-calibrate'
  sgp.setIAQBaseline(0x8E68, 0x8F41);  // Will vary for each sensor!
  pinMode(DHTPin, INPUT);
  dht.begin();
  if (! sgp.begin()){
    Serial.println("Gas sensor not found :(");
    while (1);
  }
}

unsigned long lastPublish;
int msgCount;

void setAbsoluteHumidity() 
{
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
    sgp.setHumidity(absoluteHumidityScaled);
    return;
}

void recalibrateSPG()
{
  uint16_t TVOC_base, eCO2_base;
    if (! sgp.getIAQBaseline(&eCO2_base, &TVOC_base)) {
      Serial.println("Failed to get baseline readings");
      return;
    }
    Serial.print("****Baseline values: eCO2: 0x"); Serial.print(eCO2_base, HEX);
    Serial.print(" & TVOC: 0x"); Serial.println(TVOC_base, HEX);
}

float getCO2()
{
  setAbsoluteHumidity();
  
    if (! sgp.IAQmeasure()) {
    Serial.println("C02 Measurement failed");
    return 0.0;
  }

  float CO2 = (float)sgp.eCO2;
  
  SPGcounter++;
    if (SPGcounter == 30) {
      SPGcounter = 0;
      recalibrateSPG();
    } 

  return CO2;
}
void loop() {

  if (tick >= 3)  {
    tick = 0;
    if (! sgp.IAQmeasure()) {
      Serial.println("Measurement failed");
      return;
    }

    if (! sgp.IAQmeasureRaw()) {
      Serial.println("Raw Measurement failed");
      return;
    }
    
  mycel_aws.pubSubCheckConnect();


  if (millis() - lastPublish > 10000) {

    //publish_toAWS(esp8266_ID,dht.readHumidity(),dht.readTemperature(true),getCO2(),h,t,c,l); 
    mycel_aws.publish_to_AWS(esp8266_ID, dht.readHumidity(), "Air-humidity Level");
    mycel_aws.publish_to_AWS(esp8266_ID, dht.readTemperature(true), "Air-temp Level");
    mycel_aws.publish_to_AWS(esp8266_ID, getCO2, "CO2 Level");
    mycel_aws.publish_to_AWS(esp8266_ID, analogRead(WATERSENSORPIN)/10.24, "Liquid Level");

    delay(15000);
    lastPublish = millis();
  }


}
tick++;
  
}



