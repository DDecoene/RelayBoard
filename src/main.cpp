#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>

#include <secrets.h>

#define RELAY 0 // relay connected to  GPIO0
#define SLP_TIMEOUT 6e8 // 10 mins in microseconds

const char* thingHostname = "RelayBoard-01";

WiFiServer server(80);

void setupOTA() {
  ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname(thingHostname);

  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }
  // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });

  ArduinoOTA.begin();
}


void myHandler() {
  // Check if a client has connected
  WiFiClient client = server.accept();
  if (!client) { return; }
  Serial.println(F("new client"));

  client.setTimeout(5000);  // default is 1000

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(F("request: "));
  Serial.println(req);

  // Match the request
  int val;
  if (req.indexOf(F("/RELAY/ON")) != -1) {
    val = LOW;
  } else if (req.indexOf(F("/RELAY/OFF")) != -1) {
    val = HIGH;
  } else {
    Serial.println(F("invalid request"));
    val = digitalRead(RELAY);
  }

  // Set LED according to the request
  digitalWrite(RELAY, val);

  // read/ignore the rest of the request
  // do not client.flush(): it is for output only, see below
  while (client.available()) {
    // byte by byte is not very efficient
    client.read();
  }

  // Send the response to the client
  // it is OK for multiple small client.print/write,
  // because nagle algorithm will group them into one single packet
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nRELAY is now "));
  client.print((val) ? F("OFF") : F("ON"));
  client.print(F("<br><br>Click <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/RELAY/ON'>here</a> to switch RELAY on, or <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/RELAY/OFF'>here</a> to switch RELAY off.</html>"));

  // The client will actually be *flushed* then disconnected
  // when the function returns and 'client' object is destroyed (out-of-scope)
  // flush = ensure written data are received by the other side
  Serial.println(F("Disconnecting from client"));
}

void setup() {
  Serial.begin(115200);

  // Setup Relay Pin
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, HIGH); // HIGH makes open circuit at relay

  // Attempt WIFI Connect
  Serial.printf("[WifiSetup] Connecting to %s", ssid);
  
  WiFi.begin(ssid, passPhrase);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("[WifiSetup] CONNECTED");

  // Setting up OTA automatically initiates mDNS
  setupOTA();

  // Start HTTP server  
  server.begin();
  Serial.printf("[HTTPServer] Local server started at %s\n", WiFi.localIP().toString().c_str());

  MDNS.addService("http", "tcp", 80);
}

void loop() {
  ArduinoOTA.handle();
  MDNS.update();

  myHandler();

  delay(1);
}
