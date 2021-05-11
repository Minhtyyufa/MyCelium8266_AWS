#ifndef MotorController_h
#define MotorController_h

#include "Arduino.h"
#include <PubSubClient.h>
#include "aws_config.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <time.h>
#include <ESP8266WiFi.h>


class MyCeliumAWS {
    private:
        WiFiUDP ntpUDP;
        NTPClient timeClient;
        WiFiClientSecure wiFiClient;
        PubSubClient pubSubClient;
        char payload[512];
        int b64decode(String b64Text, uint8_t* output)
        String currenttime()
        void setCurrentTime()
    public:
        void setup();
        void msgReceived(char* topic, byte* payload, unsigned int len);
        void pubSubCheckConnect();
        void publish_to_AWS(String id, float value, String readingType ) 
}

#endif