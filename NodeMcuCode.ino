
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "HTTPSRedirect.h"


// Enter network credentials:
const char* ssid     = "WIFINAME";
const char* password = "12345";

// Enter Google Script Deployment ID for writing data:
const char *GScriptId = "AKfycbx7_FfxnLIalDpfziaJl210aQ6l--NYibV-M--n61qhfotpe8iaSMcs1EkFfLlg3zBT";

// Enter command (insert_row or append_row) and your Google Sheets sheet name (default is Sheet1):
String payload_base =  "{\"command\": \"append_row\", \"sheet_name\": \"Sheet1\", \"values\": ";
String payload = "";

// Google Sheets setup to write the data (do not edit)
const char* host = "script.google.com";
const int httpsPort = 443;
const char* fingerprint = "";
String url = String("/macros/s/") + GScriptId + "/exec";
HTTPSRedirect* client = nullptr;

// Enter Google Script Deployment ID for reading data:
const char *GId = "AKfycbyUeIlyTm0LBjZ8lzvJoEz4W0GJOsy_f31HKQF75o4NeV2ZVpStVeJTi7fRoXr3KLAV";

// Google Sheets setup for reading data, define more variables to read more data:
const char *cellAddress1 = "A2";
String url2 = String("/macros/s/") + GId + "/exec?read=" + cellAddress1;
String payload2 = "";


// Declare variables that will be published to Google Sheets
int value0 = 0;
int value1 = 0;
int value2 = 0;
int value3 = 0;

void setup() {

  Serial.begin(115200);        
  delay(10);
  Serial.println('\n');
  
  // Connect to WiFi
  WiFi.begin(ssid, password);             
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  // Use HTTPSRedirect class to create a new TLS connection
  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();
  client->setPrintResponseBody(true);
  client->setContentTypeHeader("application/json");
  
  Serial.print("Connecting to ");
  Serial.println(host);

  // Try to connect for a maximum of 5 times
  bool flag = false;
  for (int i=0; i<5; i++){ 
    int retval = client->connect(host, httpsPort);
    if (retval == 1){
       flag = true;
       Serial.println("Connected");
       break;
    }
    else
      Serial.println("Connection failed. Retrying...");
  }
  if (!flag){
    Serial.print("Could not connect to server: ");
    Serial.println(host);
    return;
  }
  delete client;    // delete HTTPSRedirect object
  client = nullptr; // delete HTTPSRedirect object
}



void loop() {
  //set the values to what you want to publish in the sheets (using random data here)
  value0 = random(0,100000);
  value1 = random(0,100000);
  value2 = random(0,100000);
  value3 = random(0,100000);
  
  
  static bool flag = false;
  if (!flag){
    client = new HTTPSRedirect(httpsPort);
    client->setInsecure();
    flag = true;
    client->setPrintResponseBody(true);
    client->setContentTypeHeader("application/json");
  }
  if (client != nullptr){
    if (!client->connected()){
      client->connect(host, httpsPort);
    }
  }
  else{
    Serial.println("Error creating client object!");
  }
  

  // Create json object string to send to Google Sheets (add ";" + value# if you want to send more variables)
  payload = payload_base + "\"" + value0 + "," + value1 + "," + value2 + "," + value3 + "\"}";
  
  // Publish data to Google Sheets
  Serial.println("Publishing data...");
  Serial.println(payload);
  if(client->POST(url, host, payload)){ 
    // do stuff here if publish was successful
  }
  else{
    // do stuff here if publish was not successful
    Serial.println("Error while connecting");
  }

  delay(10000);

  //getting data from the Sheet 2
  Serial.print("GET Data from cell 2: ");
  Serial.println(cellAddress2);
  if (client->GET(url2, host)){
  //get the value of the cell
   payload2 = client->getResponseBody();
   int num = payload2.toInt();
    switch(num){
    case 0:
      Serial.print("No action \n");// statements
      break;
    case 1:
      Serial.print("Pump On \n");// statements
      break;
    case 2:
      Serial.print("Light On \n");// statements
      break;
}
    }

}
