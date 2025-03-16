#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

void ota_handle( void * parameter ) {
  for (;;) {
    ArduinoOTA.handle();
    //delay(3500);
  }
}

void setupOTA(const char* nameprefix, const char* ssid, const char* password) {
  // Configure the hostname
  uint16_t maxlen = strlen(nameprefix) + 7;
  char *fullhostname = new char[maxlen];
  uint8_t mac[6];
  WiFi.macAddress(mac);
  snprintf(fullhostname, maxlen, "%s-%02x%02x%02x", nameprefix, mac[3], mac[4], mac[5]);
  ArduinoOTA.setHostname(fullhostname);
  delete[] fullhostname;

  // Configure and start the WiFi station
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  
  // Wait for connection

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial2.print("b31.txt=\"fail reboot..."); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
    delay(5000);
    ESP.restart();
  }

  //delay(5000); //this delay needed to wait for the connection
  // Port defaults to 3232
  // ArduinoOTA.setPort(3232); // Use 8266 port if you are working in Sloeber IDE, it is fixed there and not adjustable

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
	//NOTE: make .detach() here for all functions called by Ticker.h library - not to interrupt transfer process in any way.
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  
  ArduinoOTA.onEnd([]() {
    Serial2.print("b31.txt=\"End"); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
  });
  
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial2.print("b21.txt=\"Progress: "); Serial2.print(progress / (total / 100)); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
  });
  
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {Serial2.print("b31.txt=\"Auth Failed"); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");}
    else if (error == OTA_BEGIN_ERROR) {Serial2.print("b31.txt=\"Begin Failed"); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");}
    else if (error == OTA_CONNECT_ERROR) {Serial2.print("b31.txt=\"Connect Failed"); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");}
    else if (error == OTA_RECEIVE_ERROR) {Serial2.print("b31.txt=\"Receive Failed"); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");}
    else if (error == OTA_END_ERROR) {Serial2.print("b31.txt=\"End Failed"); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");}
  });

  ArduinoOTA.begin();

  Serial.println("OTA Initialized");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial2.print("b31.txt=\" OTA:"); Serial2.print(WiFi.localIP()); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");


  xTaskCreate(
    ota_handle,          /* Task function. */
    "OTA_HANDLE",        /* String with name of task. */
    10000,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL);            /* Task handle. */
}