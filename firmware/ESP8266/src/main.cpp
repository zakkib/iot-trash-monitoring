/****************************************************
 * HYBRID FIREBASE FIRMWARE (SEND + READ)
 * ESP8266 NodeMCU V3 Compatible Version
 ****************************************************/

#define ENABLE_USER_AUTH
#define ENABLE_DATABASE

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <FirebaseClient.h>
#include "secrets.h"

// Forward declaration
void processData(AsyncResult &aResult);

// Firebase authentication
UserAuth user_auth(Web_API_KEY, USER_EMAIL, USER_PASS);

// Firebase components
FirebaseApp app;
WiFiClientSecure ssl_client;
using AsyncClient = AsyncClientClass;
AsyncClient aClient(ssl_client);
RealtimeDatabase Database;

// ===== TIMERS =====
unsigned long lastSendTime = 0;
unsigned long lastReadTime = 0;
const unsigned long sendInterval = 10000;  // 10 seconds
const unsigned long readInterval = 10000;  // 10 seconds

// ===== Local variables =====
int intValueSend = 0;
float floatValueSend = 0.01;
String stringValueSend = "";

int intValueRead;
float floatValueRead;
String stringValueRead;

void setup() {
  Serial.begin(115200);
  delay(200);

  // Connect WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nWiFi Connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // ===== SSL CONFIGURATION =====
  ssl_client.setInsecure();           // no certificate validation
  ssl_client.setTimeout(15000);       // IMPORTANT: 15s timeout for ESP8266

  delay(300);  // ESP8266 needs a moment before TLS handshake

  // ===== FIREBASE INIT =====
  initializeApp(aClient, app, getAuth(user_auth), processData, "authTask");
  app.getApp<RealtimeDatabase>(Database);
  Database.url(DATABASE_URL);

  Serial.println("Firebase initialized!");
}

void loop() {
  app.loop();
  if (!app.ready()) return;

  unsigned long now = millis();

  /*************** SEND EVERY 10s ***************/
  if (now - lastSendTime >= sendInterval) {
    lastSendTime = now;

    Serial.println("\n=== SENDING DATA ===");

    stringValueSend = "value_" + String(now);
    Database.set<String>(aClient, "/hybrid/string", stringValueSend, processData, "Send_String");

    Database.set<int>(aClient, "/hybrid/int", intValueSend, processData, "Send_Int");
    intValueSend++;

    floatValueSend = 0.01f + (float)random(0, 100);
    Database.set<float>(aClient, "/hybrid/float", floatValueSend, processData, "Send_Float");
  }

  /*************** READ EVERY 10s ***************/
  if (now - lastReadTime >= readInterval) {
    lastReadTime = now;

    Serial.println("\n=== READING DATA ===");

    Database.get(aClient, "/hybrid/int", processData, false, "Get_Int");
    Database.get(aClient, "/hybrid/float", processData, false, "Get_Float");
    Database.get(aClient, "/hybrid/string", processData, false, "Get_String");
  }
}

void processData(AsyncResult &aResult) {
  if (!aResult.isResult()) return;

  // Firebase events
  if (aResult.isEvent())
    Firebase.printf("[EVENT] %s | %s\n",
        aResult.uid().c_str(),
        aResult.eventLog().message().c_str());

  if (aResult.isDebug())
    Firebase.printf("[DEBUG] %s | %s\n",
        aResult.uid().c_str(),
        aResult.debug().c_str());

  if (aResult.isError())
    Firebase.printf("[ERROR] %s | %s\n",
        aResult.uid().c_str(),
        aResult.error().message().c_str());

  // Payload handling
  if (aResult.available()) {
    String payload = aResult.c_str();
    Firebase.printf("[PAYLOAD] %s => %s\n",
        aResult.uid().c_str(),
        payload.c_str());

    if (aResult.uid() == "Get_Int") {
      intValueRead = payload.toInt();
      Serial.printf("  📌 intValueRead = %d\n", intValueRead);
    }

    else if (aResult.uid() == "Get_Float") {
      floatValueRead = payload.toFloat();
      Serial.printf("  📌 floatValueRead = %.2f\n", floatValueRead);
    }

    else if (aResult.uid() == "Get_String") {
      stringValueRead = payload;
      Serial.printf("  📌 stringValueRead = %s\n", stringValueRead.c_str());
    }
  }
}
