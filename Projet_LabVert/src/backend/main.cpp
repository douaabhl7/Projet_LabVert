/*********
  ESP32 Serveur Web + Capteur DHT11
*********/
/*
#include <WiFi.h>
#include "DHT.h"

// Identifiants Wifi
char* ssid ="batman";
const char* motDePasse = "selma1502";

// DHT11 setup
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Serveur web port 80 
WiFiServer server(80);

String header;

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  dht.begin();

  Serial.println("Connexion au WiFi...");
  WiFi.begin(ssid, motDePasse);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connecté !");
  Serial.print("Adresse IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("Nouvel utilisateur connecté.");

    String currentLine = "";

    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();

      if (client.available()) {
        char c = client.read();
        header += c;

        if (c == '\n') {
          if (currentLine.length() == 0) {

            // Lecture du DHT11
            float h = dht.readHumidity();
            float t = dht.readTemperature();

            // Réponse HTTP
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html; charset=UTF-8");
            client.println("Connection: close");
            client.println();

            // Début HTML
            client.println("<!DOCTYPE html><html lang=\"fr\">");
            client.println("<head>");
            client.println("<meta charset=\"UTF-8\">");
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<meta http-equiv=\"refresh\" content=\"5\">");

            // CSS moderne 
            client.println("<style>");
            client.println("body { font-family: Arial, sans-serif; background: #f2f2f2; margin: 0; padding: 0; display: flex; flex-direction: column; align-items: center; }");
            client.println("h1 { margin-top: 30px; color: #2e7d32; font-size: 2.2rem; }");
            client.println(".card { background: white; padding: 20px 30px; margin-top: 20px; border-radius: 12px; box-shadow: 0 4px 12px rgba(0,0,0,0.1); width: 260px; }");
            client.println(".value { font-size: 1.6rem; margin: 10px 0; color: #333; }");
            client.println(".label { font-size: 1rem; color: #777; }");
            client.println("</style>");

            client.println("</head>");
            client.println("<body>");

            client.println("<h1>Lab Vert</h1>");
            client.println("<div class='card'>");

            if (isnan(h) || isnan(t)) {
              client.println("<p><strong>Erreur de lecture du capteur !</strong></p>");
            } else {
              client.println("<div class='value'>" + String(t) + " °C</div>");
              client.println("<div class='label'>Température</div>");

              client.println("<div class='value'>" + String(h) + " %</div>");
              client.println("<div class='label'>Humidité</div>");
            }

            client.println("</div>");
            client.println("</body></html>");
            client.println();

            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    header = "";
    client.stop();
    Serial.println("Client déconnecté.\n");
  }
}
  */


/*********
  ESP32 Backend + DHT11
*********/

#include <WiFi.h>
#include "DHT.h"
#include <FS.h>
#include <SPIFFS.h>

char* ssid ="batman";
const char* motDePasse = "selma1502";

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  dht.begin();

  if (!SPIFFS.begin(true)) {
    Serial.println("Erreur SPIFFS");
    return;
  }

  Serial.println("Connexion au WiFi...");
  WiFi.begin(ssid, motDePasse);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connecté !");
  Serial.print("Adresse IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  Serial.println("Nouvelle requête HTTP");

  while (!client.available()) delay(1);

  String req = client.readStringUntil('\r');
  client.readStringUntil('\n');

  // Route JSON
  if (req.indexOf("GET /data") >= 0) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();

    client.printf("{\"temperature\": %.1f, \"humidity\": %.1f}", t, h);
    return;
  }

  // Route HTML
  if (req.indexOf("GET /") >= 0) {
    File file = SPIFFS.open("/index.html", "r");

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();

    while (file.available()) {
      client.write(file.read());
    }
    file.close();
    return;
  }

  // Route CSS
  if (req.indexOf("GET /style.css") >= 0) {
    File file = SPIFFS.open("/style.css", "r");

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/css");
    client.println("Connection: close");
    client.println();

    while (file.available()) {
      client.write(file.read());
    }
    file.close();
    return;
  }

  // Route JS
  if (req.indexOf("GET /script.js") >= 0) {
    File file = SPIFFS.open("/script.js", "r");

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/javascript");
    client.println("Connection: close");
    client.println();

    while (file.available()) {
      client.write(file.read());
    }
    file.close();
    return;
  }
}