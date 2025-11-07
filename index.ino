#include <WiFi.h>
#include <WebServer.h>

// ==== USER SETTINGS ====
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// ==== SENSOR SETTINGS ====
const int sensorPin = 34;        // Analog input pin
const float sensitivity = 0.185; // V/A for ACS712-5A, change for your sensor
const float vRef = 3.3;          // ESP32 reference voltage
const int adcMax = 4095;         // 12-bit ADC
const float offsetVoltage = vRef / 2.0; // Midpoint (no current)

// ==== WEB SERVER ====
WebServer server(80);

float readCurrent() {
  int adcValue = analogRead(sensorPin);
  float voltage = (adcValue * vRef) / adcMax;
  float current = (voltage - offsetVoltage) / sensitivity; // In Amperes
  return current;
}

String htmlPage(float current) {
  String html = "<!DOCTYPE html><html><head><meta http-equiv='refresh' content='2' />";
  html += "<title>ESP32 Current Monitor</title>";
  html += "<style>body{font-family:Arial;text-align:center;margin-top:50px;}";
  html += "h1{color:#333;} .value{font-size:2em;color:#0066cc;}</style></head><body>";
  html += "<h1>ESP32 Hall Effect Current Sensor</h1>";
  html += "<p class='value'>Current: " + String(current, 3) + " A</p>";
  html += "</body></html>";
  return html;
}

void handleRoot() {
  float current = readCurrent();
  server.send(200, "text/html", htmlPage(current));
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server started.");
}

void loop() {
  server.handleClient();
}
