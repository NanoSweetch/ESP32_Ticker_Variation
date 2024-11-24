#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <LEDMatrixDriver.hpp>
#include <ESPAsyncWebServer.h>

const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";
String serverName = "https://query1.finance.yahoo.com/v8/finance/chart/";  // URL de base de l'API pour BTC-USD
String ticker = "BTC-USD";  // Ticker par défaut
const uint8_t LEDMATRIX_CS_PIN = 15;  // Pin CS pour la matrice LED
const int LEDMATRIX_SEGMENTS = 4;  // Nombre de segments de la matrice LED
const int LEDMATRIX_WIDTH = LEDMATRIX_SEGMENTS * 8;  // Largeur totale de la matrice
LEDMatrixDriver lmd(LEDMATRIX_SEGMENTS, LEDMATRIX_CS_PIN);  // Initialisation de la matrice LED

const int ANIM_DELAY = 90;  // Vitesse du défilement du texte

byte font[95][8] = { {0,0,0,0,0,0,0,0}, // SPACE
                     {0x10,0x18,0x18,0x18,0x18,0x00,0x18,0x18}, // EXCL
                     {0x28,0x28,0x08,0x00,0x00,0x00,0x00,0x00}, // QUOT
                     {0x00,0x0a,0x7f,0x14,0x28,0xfe,0x50,0x00}, // #
                     {0x10,0x38,0x54,0x70,0x1c,0x54,0x38,0x10}, // $
                     {0x00,0x60,0x66,0x08,0x10,0x66,0x06,0x00}, // %
                     {0,0,0,0,0,0,0,0}, // &
                     {0x00,0x10,0x18,0x18,0x08,0x00,0x00,0x00}, // '
                     {0x02,0x04,0x08,0x08,0x08,0x08,0x08,0x04}, // (
                     {0x40,0x20,0x10,0x10,0x10,0x10,0x10,0x20}, // )
                     {0x00,0x10,0x54,0x38,0x10,0x38,0x54,0x10}, // *
                     {0x00,0x08,0x08,0x08,0x7f,0x08,0x08,0x08}, // +
                     {0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x08}, // COMMA
                     {0x00,0x00,0x00,0x00,0x7e,0x00,0x00,0x00}, // -
                     {0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x06}, // DOT
                     {0x00,0x04,0x04,0x08,0x10,0x20,0x40,0x40}, // /
                     {0x00,0x38,0x44,0x4c,0x54,0x64,0x44,0x38}, // 0
                     {0x00,0x04,0x0c,0x14,0x04,0x04,0x04,0x04}, // 1
                     {0x00,0x38,0x44,0x04,0x04,0x38,0x40,0x7c}, // 2
                     {0x00,0x38,0x44,0x04,0x18,0x04,0x44,0x38}, // 3
                     {0x00,0x04,0x0c,0x14,0x24,0x7e,0x04,0x04}, // 4
                     {0x00,0x7c,0x40,0x40,0x78,0x04,0x04,0x78}, // 5
                     {0x00,0x38,0x40,0x40,0x78,0x44,0x44,0x38}, // 6
                     {0x00,0x7c,0x04,0x08,0x10,0x10,0x10,0x10}, // 7
                     {0x00,0x38,0x44,0x44,0x38,0x44,0x44,0x38}, // 8
                     {0x00,0x38,0x44,0x44,0x3c,0x04,0x04,0x38}, // 9
                     {0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x00}, // :
                     {0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x08}, // ;
                     {0x00,0x10,0x20,0x40,0x80,0x40,0x20,0x10}, // <
                     {0x00,0x00,0x7e,0x00,0x00,0xfc,0x00,0x00}, // =
                     {0x00,0x08,0x04,0x02,0x01,0x02,0x04,0x08}, // >
                     {0x00,0x38,0x44,0x04,0x08,0x10,0x00,0x10}, // ?
                     {0x00,0x30,0x48,0xba,0xba,0x84,0x78,0x00}, // @
                     {0x00,0x3c,0x42,0x42,0x42,0x7e,0x42,0x42}, // A
                     {0x00,0x78,0x44,0x44,0x78,0x44,0x44,0x78}, // B
                     {0x00,0x38,0x44,0x40,0x40,0x40,0x44,0x38}, // C
                     {0x00,0x7c,0x42,0x42,0x42,0x42,0x42,0x7c}, // D
                     {0x00,0x78,0x40,0x40,0x70,0x40,0x40,0x78}, // E {0x00,0x78,0x40,0x40,0x70,0x40,0x40,0x78}
                     {0x00,0x7c,0x40,0x40,0x7c,0x40,0x40,0x40}, // F
                     {0x00,0x38,0x44,0x40,0x5c,0x44,0x44,0x38}, // G
                     {0x00,0x42,0x42,0x42,0x7e,0x42,0x42,0x42}, // H
                     {0x00,0x7c,0x10,0x10,0x10,0x10,0x10,0x7c}, // I
                     {0x00,0x04,0x04,0x04,0x04,0x04,0x24,0x18}, // J
                     {0x00,0x44,0x48,0x50,0x60,0x50,0x48,0x44}, // K
                     {0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x7c}, // L
                     {0x00,0x82,0xc6,0xaa,0x92,0x82,0x82,0x82}, // M
                     {0x00,0x42,0x42,0x62,0x52,0x4a,0x46,0x42}, // N
                     {0x00,0x3c,0x42,0x42,0x42,0x42,0x42,0x3c}, // O
                     {0x00,0x78,0x44,0x44,0x44,0x78,0x40,0x40}, // P
                     {0x00,0x3c,0x42,0x42,0x52,0x4a,0x44,0x3a}, // Q
                     {0x00,0x78,0x44,0x44,0x78,0x50,0x48,0x44}, // R
                     {0x00,0x3c,0x40,0x40,0x38,0x04,0x04,0x78}, // S
                     {0x00,0xfe,0x10,0x10,0x10,0x10,0x10,0x10}, // T
                     {0x00,0x42,0x42,0x42,0x42,0x42,0x42,0x3c}, // U
                     {0x00,0x42,0x42,0x42,0x42,0x42,0x24,0x18}, // V
                     {0x00,0x82,0x82,0x92,0x92,0x92,0x54,0x38}, // W
                     {0x00,0x82,0x44,0x28,0x10,0x28,0x44,0x82}, // X
                     {0x00,0x82,0x44,0x28,0x10,0x10,0x10,0x10}, // Y
                     {0x00,0xfe,0x04,0x08,0x10,0x20,0x40,0xfe}, // Z
                      // (the font does not contain any lower case letters. you can add your own.)                       
};

int x = LEDMATRIX_WIDTH;  // Position de départ pour le défilement
char displayString[50] = "";  // Texte à afficher sur la matrice LED

void drawSprite(byte* sprite, int x, int y, int width, int height) {
  byte mask = B10000000;
  for (int iy = 0; iy < height; iy++) {
    for (int ix = 0; ix < width; ix++) {
      lmd.setPixel(x + ix, y + iy, (bool)(sprite[iy] & mask));
      mask = mask >> 1;
    }
    mask = B10000000;
  }
}

void drawString(char* text, int len, int x, int y) {
  for (int idx = 0; idx < len; idx++) {
    int c = text[idx] - 32;
    if (x + idx * 8 > LEDMATRIX_WIDTH)
      return;
    if (8 + x + idx * 8 > 0)
      drawSprite(font[c], x + idx * 8, y, 8, 8);
  }
}

void LEDDisplay(void* parameter) {
  while (true) {
    int len = strlen(displayString);
    drawString(displayString, len, x, 0);
    lmd.display();
    delay(ANIM_DELAY);
    if (--x < len * -8) {
      x = LEDMATRIX_WIDTH;
    }
  }
}

// Crée un serveur web asynchrone
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  // Connexion Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(".");
    delay(500);
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialisation de la matrice LED
  lmd.setEnabled(true);
  lmd.setIntensity(0);   // Intensity de l'affichage (0 = faible, 10 = fort)

  // Crée un task pour gérer l'affichage
  xTaskCreatePinnedToCore(LEDDisplay, "LedDisplay", 1000, NULL, 1, NULL, 0);

  // Affichage de l'adresse IP au démarrage
  char text[50];
  String IP = WiFi.localIP().toString();
  strcpy(text, "IP: ");
  strcat(text, IP.c_str());
  memcpy(displayString, text, sizeof(displayString));

  // Mise à jour toutes les 5 secondes
  delay(5000);

  // Serveur Web pour entrer un ticker
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<html><body><h2>Choisir un Ticker</h2>";
    html += "<form action=\"/setTicker\" method=\"GET\">";
    html += "Ticker (ex : BTC-USD, ETH-USD): <input type=\"text\" name=\"ticker\" value=\"" + ticker + "\"><br><br>";
    html += "<input type=\"submit\" value=\"Envoyer\">";
    html += "</form></body></html>";
    request->send(200, "text/html", html);
  });

  // Action de mise à jour du ticker via le formulaire
  server.on("/setTicker", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("ticker")) {
      ticker = request->getParam("ticker")->value();
      Serial.println("Nouveau Ticker: " + ticker);
    }
    request->redirect("/");
  });

  // Démarrer le serveur web
  server.begin();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = serverName + ticker + "?interval=1d";  // URL de l'API avec le ticker sélectionné
    http.begin(url);  // Requête à l'API pour le ticker

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String payload = http.getString();
      DynamicJsonDocument doc(2048);
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }

      JsonObject chart_result_0 = doc["chart"]["result"][0];
      JsonObject chart_result_0_meta = chart_result_0["meta"];
      const char* tempsymbol = chart_result_0_meta["symbol"];
      float tempmarketprice = chart_result_0_meta["regularMarketPrice"];
      float temppreviousclose = chart_result_0_meta["chartPreviousClose"];

      // Vérification si les prix sont valides
      if (temppreviousclose != 0 && !isnan(tempmarketprice) && !isnan(temppreviousclose)) {
        float tempchangepercent = ((tempmarketprice - temppreviousclose) / temppreviousclose) * 100;

        // Formatage des résultats
        char pricebuffer[10];
        char changebuffer[10];
        dtostrf(tempmarketprice, 6, 2, pricebuffer);
        dtostrf(tempchangepercent, 5, 2, changebuffer);

        // Création de la chaîne de texte à afficher
        char text[50] = "";
        strcat(text, tempsymbol);  // Symbole du ticker (BTC-USD)
        strcat(text, " ");         // Espace
        strcat(text, pricebuffer); // Prix
        strcat(text, "$ ");        // Symbole $
        strcat(text, changebuffer); // Variation en %
        strcat(text, "%");          // Symbole %

        // Mise à jour de la chaîne d'affichage
        memcpy(displayString, text, sizeof(displayString));
        Serial.println(displayString);
      }
    } else {
      Serial.print("Error Code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
    delay(5000); // Rafraîchissement toutes les 5 secondes
  }
}
