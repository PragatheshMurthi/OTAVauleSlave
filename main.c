#include <WiFi.h>
#include <WebServer.h>

// WiFi Credentials
const char* ssid = "Your_WiFi_Name";
const char* password = "Your_WiFi_Password";

// Create HTTP server on port 80
WebServer server(80);

// LED GPIO
const int ledPin = 2;

// Root page
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

    Serial.println();
    Serial.println("Connecting to WiFi...");

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi Connected");

    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP());

    // URL routing
    server.on("/", handleRoot);
    server.on("/on", ledOn);
    server.on("/off", ledOff);

    // Handle invalid URLs
    server.onNotFound([]() {
        server.send(404, "text/plain", "Page Not Found");
    });

    server.begin();

    Serial.println("HTTP Server Started");
}

void loop()
{
    server.handleClient();
}
