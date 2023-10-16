#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <laboratorioFW-DIY.h>
#include "addons/TokenHelper.h"
#include "credenciais.h"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseData stream;

#define buzzer P_0

void setup() {
  Serial.begin(115200);
  pinMode(buzzer, OUTPUT);

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

  Firebase.RTDB.beginStream(&fbdo, F("/callback/buzzer"));
  Firebase.RTDB.setStreamCallback(&fbdo, streamCallback, streamTimeoutCallback);
}


void streamCallback(FirebaseStream data) {
  int freq = data.to<int>();
  tone(buzzer, freq, 500);
}

void streamTimeoutCallback(bool timeout) {
  if (timeout) {
    Serial.println("stream timeout, resuming...\n");
  }
}

void loop() {
}
