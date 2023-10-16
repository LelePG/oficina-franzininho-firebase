#include <WiFi.h>
#include <laboratorioFW-DIY.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "credenciais.h"


FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;


#define ledR L_4
#define ledG L_6
#define ledB L_5

void setup() {
  Serial.begin(115200);

  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);

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

unsigned long sendDataPrevMillis = 0;
int r, g, b;

void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > INTERVALO || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    Firebase.RTDB.getInt(&fbdo, F("/crud/ler/0"), &r);
    Firebase.RTDB.getInt(&fbdo, F("/crud/ler/1"), &g);
    Firebase.RTDB.getInt(&fbdo, F("/crud/ler/2"), &b);
    escreveRGB(r, g, b);
  }
}

void escreveRGB(int r, int g, int b) {
  analogWrite(ledR, r);
  analogWrite(ledG, g);
  analogWrite(ledB, b);
}
