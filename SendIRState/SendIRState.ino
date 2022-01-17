/*
  Golf Stuff Prototyping 
  This program is meant to read a digital input from an IR sensor and upload the sensor value to a Firebase RTDB
  
  Some code in this document is based on tutorial found here: https://RandomNerdTutorials.com/esp32-esp8266-firebase-bme280-rtdb/

*/

#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include <Wire.h>


// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "daddyshouse"
#define WIFI_PASSWORD "Babyyyyy"

// Insert Firebase project API Key
#define API_KEY "AIzaSyDnWsodRXPRbEv_BEgTsW8yux7czINvzC4"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "al.m.olszewski@gmail.com"
#define USER_PASSWORD "Babyyyyy"

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL "https://golfballerz-default-rtdb.firebaseio.com/"

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable to save USER UID
String uid;

// Variables to save database paths
String databasePath;
String IRSendPath;
String IRPushPath;
int IRState;
int IRSensor;


// Timer variables (send new readings every minute)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 60000;



// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}


// Write int values to the database
void sendInt(String path, int value){
  if (Firebase.RTDB.setInt(&fbdo, path.c_str(), value)){
    Serial.print("Writing value: ");
    Serial.print (value);
    Serial.print(" on the following path: ");
    Serial.println(path);
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}

// Write int values to the database
void pushInt(String path, int value){
  if (Firebase.RTDB.pushInt(&fbdo, path.c_str(), value)){
    Serial.print("Pushing value: ");
    Serial.print (value);
    Serial.print(" on the following path: ");
    Serial.println(path);
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}

void setup(){
  Serial.begin(115200);

  //initialize wifi and sensors
  initWiFi();
  // GPIO5 as sensor
  IRSensor = 5;
  pinMode(IRSensor, INPUT_PULLUP);
  Serial.println("IR sensor initialized.");


  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update paths
  databasePath = "/UsersData/" + uid;
  IRSendPath = databasePath + "/IRState-current";
  IRPushPath = databasePath + "/IRState-log";
}

void loop(){
  // Send new readings to database
  // Get latest sensor readings
  IRState = digitalRead(IRSensor);
  Serial.println(IRState);

  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    // Send readings to database:
    sendInt(IRSendPath, IRState);

    //Append current state to RTDB
    pushInt(IRPushPath, IRState);
    
  }
  delay(100);
}
