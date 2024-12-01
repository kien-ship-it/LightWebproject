#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>

// WiFi Credentials
const char* ssid = "Kien 2007 #2";
const char* password = "hung1971";

// NeoPixel Configuration
#define PIN        4  // GPIO4 (D4) for data pin
#define NUM_LEDS   60 // Number of LEDs in the strip

Adafruit_NeoPixel strip(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// Webserver
WebServer server(80);

// Global variables for effects
bool isOn = false;
String currentMode = "off";
int firstPixelHue = 0;

void setup() {
  // Initialize Serial with a lower baud rate to ensure communication
  Serial.begin(115200);
  
  // Wait a moment to ensure Serial is ready
  delay(1000);
  
  // Debug: Print startup message
  Serial.println("\n\n--- LED Control Firmware ---");
  Serial.println("Initializing...");

  // Initialize LittleFS
  Serial.print("Mounting LittleFS... ");
  if(!LittleFS.begin(true)){
    Serial.println("FAILED");
    return;
  }
  Serial.println("SUCCESS");

  // List files in LittleFS
  Serial.println("Files in LittleFS:");
  File root = LittleFS.open("/");
  File file = root.openNextFile();
  while(file){
    Serial.print("  FILE: ");
    Serial.println(file.name());
    file = root.openNextFile();
  }

  // Initialize NeoPixel strip
  Serial.print("Initializing NeoPixel strip... ");
  strip.begin();
  strip.show();
  strip.setBrightness(5); // Default brightness
  Serial.println("DONE");

  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    attempts++;
    if(attempts > 20) {
      Serial.println("\nFailed to connect to WiFi!");
      break;
    }
  }
  
  if(WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }

  // Serve static files
  server.on("/", HTTP_GET, []() {
    Serial.println("Serving root page");
    File file = LittleFS.open("/index.html", "r");
    if(!file){
      Serial.println("ERROR: index.html not found");
      server.send(404, "text/plain", "File not found");
      return;
    }
    server.streamFile(file, "text/html");
    file.close();
  });

  // Power control endpoint
  server.on("/power", HTTP_POST, []() {
    isOn = !isOn;
    currentMode = "off";
    updateLEDs();
    
    Serial.println(isOn ? "Lights turned ON" : "Lights turned OFF");
    
    StaticJsonDocument<200> doc;
    doc["message"] = isOn ? "Lights turned ON" : "Lights turned OFF";
    String response;
    serializeJson(doc, response);
    server.send(200, "application/json", response);
  });

  // Color setting endpoint
  server.on("/color", HTTP_POST, []() {
    Serial.println("Color setting endpoint hit");
    if (server.hasArg("plain")) {
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, server.arg("plain"));
      
      if (!error) {
        int r = doc["r"] | 0;
        int g = doc["g"] | 0;
        int b = doc["b"] | 0;
        
        Serial.printf("Setting color: R:%d, G:%d, B:%d\n", r, g, b);
        
        isOn = true;
        currentMode = "color";
        
        for(int i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(r, g, b));
        }
        strip.show();
        
        StaticJsonDocument<200> responseDoc;
        responseDoc["message"] = "Color set successfully";
        String response;
        serializeJson(responseDoc, response);
        server.send(200, "application/json", response);
      } else {
        Serial.println("JSON parsing error");
        server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
      }
    }
  });

  // Effect setting endpoint
  server.on("/effect", HTTP_POST, []() {
    if (server.hasArg("plain")) {
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, server.arg("plain"));
      
      if (!error) {
        String mode = doc["mode"] | "off";
        currentMode = mode;
        
        Serial.println("Effect mode set to: " + mode);
        
        StaticJsonDocument<200> responseDoc;
        responseDoc["message"] = "Effect set to: " + mode;
        String response;
        serializeJson(responseDoc, response);
        server.send(200, "application/json", response);
      } else {
        server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
      }
    }
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  
  if (isOn) {
    if (currentMode == "rainbow") {
      rainbowRun(20);
    }
  } else {
    strip.clear();
    strip.show();
  }
}

// Function: Running rainbow effect
void rainbowRun(int wait) {
  if (currentMode != "rainbow") return;
  
  for (int i = 0; i < strip.numPixels(); i++) {
    int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
    strip.setPixelColor(i, strip.ColorHSV(pixelHue));
  }
  strip.show();
  firstPixelHue += 256;
  if (firstPixelHue >= 65536) {
    firstPixelHue = 0;
  }
  delay(wait);
}

void updateLEDs() {
  if (!isOn) {
    strip.clear();
    strip.show();
  }
}