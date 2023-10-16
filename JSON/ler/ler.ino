#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <laboratorioFW-DIY.h>
#include "addons/TokenHelper.h"
#include "credenciais.h"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

#define ledAzul L_0
#define ledVermelho L_1
#define ledVerde L_2
#define ledAmarelo L_3

void setup() {
  Serial.begin(115200);
  pinMode(ledAzul, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_SENHA);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }

  config.api_key = CHAVE_API;
  config.database_url = URL_BANCO;

  if (!Firebase.signUp(&config, &auth, USUARIO_FIREBASE, SENHA_USUARIO_FIREBASE)) {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

FirebaseJson json;
bool azul, vermelho, verde, amarelo;
unsigned long sendDataPrevMillis = 0;

void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > INTERVALO || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    Firebase.RTDB.getJSON(&fbdo, F("/json/leds"), &json);
    ligaLED(ledAzul, "azul", json);
    ligaLED(ledVermelho, "vermelho", json);
    ligaLED(ledVerde, "verde", json);
    ligaLED(ledAmarelo, "amarelo", json);
  }
}

void ligaLED(int led, char* cor, FirebaseJson json) {
  FirebaseJsonData resultado;
  json.get(resultado, cor);
  resultado.to<bool>();
  digitalWrite(led, resultado.to<bool>());
}
