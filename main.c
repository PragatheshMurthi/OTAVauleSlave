#include <WiFi.h>
#include <WebServer.h>

// Set up the ESP32's own Wi-Fi Network (Open/No Security)
const char* ap_ssid = "ESP32-LED-Control";
const char* ap_password = NULL; 

// Create HTTP server on port 80
WebServer server(80);

// LED GPIO
const int ledPin = 2;

// Root page HTML
void handleRoot()
{
    String html = R"(
    <!DOCTYPE html>
    <html>
      <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>ESP32 LED Control</title>
        <style>
          body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; background-color: #f4f4f9; }
          h2 { color: #333; }
          .btn { display: inline-block; width: 150px; height: 50px; margin: 10px; font-size: 16px; font-weight: bold; text-decoration: none; border-radius: 5px; line-height: 50px; color: white; }
          .btn-on { background-color: #2ec4b6; }
          .btn-off { background-color: #e71d36; }
          .btn:active { transform: scale(0.98); }
        </style>
      </head>
      <body>
        <h2>ESP32 LED Control</h2>
        <a href="/on" class="btn btn-on">LED ON</a>
        <a href="/off" class="btn btn-off">LED OFF</a>
      </body>
  </html>
    )";

    server.send(200, "text/html", html);
}

// Turn LED ON
void ledOn()
{
    digitalWrite(ledPin, HIGH);
    server.sendHeader("Location", "/");
    server.send(303);
}

// Turn LED OFF
void ledOff()
{
    digitalWrite(ledPin, LOW);
    server.sendHeader("Location", "/");
    server.send(303);
}

void setup()
{
    Serial.begin(115200);

    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    delay(1000);
    Serial.println("\n--- Starting ESP32 Access Point ---");

    // 1. Force Access Point Mode
    WiFi.mode(WIFI_AP);
    
    // 2. Start the open broadcast network
    WiFi.softAP(ap_ssid, ap_password);

    Serial.println("[SUCCESS] Access Point Active!");
    
    // 3. Print the IP address you need to type into your phone browser
    Serial.print("IP Address to visit on phone: ");
    Serial.println(WiFi.softAPIP()); // This defaults to 192.168.4.1

    // Server routes
    server.on("/", handleRoot);
    server.on("/on", ledOn);
    server.on("/off", ledOff);
    server.onNotFound([]() { server.send(404, "text/plain", "Page Not Found"); });
    
    server.begin();
    Serial.println("HTTP Server Started");
}

void loop()
{
    server.handleClient();
}
