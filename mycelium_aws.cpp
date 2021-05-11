extern "C" {
#include "libb64/cdecode.h"
}


MyCeliumAWS::MyCeliumAWS(){
    this->pubSubClient = PubSubClient(awsEndpoint, 8883, this.msgReceived, this->wiFiClient); 
    this->timeClient = NTPClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);
}


void MyCeliumAWS::setup(){
    Serial.println();

    Serial.println("ESP8266 AWS IoT Example");

    Serial.print("Connecting to "); Serial.print(ssid);
    WiFi.begin(ssid, password);
    WiFi.waitForConnectResult();
    Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());

    // get current time, otherwise certificates are flagged as expired
    setCurrentTime();

    uint8_t binaryCert[certificatePemCrt.length() * 3 / 4];
    int len = this.b64decode(certificatePemCrt, binaryCert);
    this->wiFiClient.setCertificate(binaryCert, len);
    
    uint8_t binaryPrivate[privatePemKey.length() * 3 / 4];
    len = this.b64decode(privatePemKey, binaryPrivate);
    this->wiFiClient.setPrivateKey(binaryPrivate, len);

    uint8_t binaryCA[caPemCrt.length() * 3 / 4];
    len = this.b64decode(caPemCrt, binaryCA);
    this->wiFiClient.setCACert(binaryCA, len);
}



void MyCeliumAWS::pubSubCheckConnect() {
  if ( ! this->pubSubClient.connected()) {
    Serial.print("PubSubClient connecting to: "); Serial.print(awsEndpoint);
    while ( ! this->pubSubClient.connected()) {
      Serial.print(".");
      this->pubSubClient.connect("ESPthing");
    }
    Serial.println("connected");
    this->pubSubClient.subscribe("ESP8266_TEST");
  }
  this->pubSubClient.loop();
}

void MyCeliumAWS::msgReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on "); Serial.print(topic); Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


int MyCeliumAWS::b64decode(String b64Text, uint8_t* output) {
  base64_decodestate s;
  base64_init_decodestate(&s);
  int cnt = base64_decode_block(b64Text.c_str(), b64Text.length(), (char*)output, &s);
  return cnt;
}
String MyCeliumAWS::currenttime(){
  char *asctime(const struct tm *time);
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: "); Serial.print(asctime(&timeinfo));
  return asctime(&timeinfo);
}


void MyCelium::setCurrentTime() {
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: "); Serial.print(asctime(&timeinfo));
}

void MyCelium::publish_to_AWS(String id, float value, String readingType ) {
    pubSubCheckConnect();
    sprintf(payload,JSONPAYLOADhum,id.c_str(),this.currenttime().c_str(),readingType.c_str(),hum);
    Serial.println(payload);  
    pubSubClient.publish("ESP8266_TEST", payload);  
      
}