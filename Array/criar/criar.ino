#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <laboratorioFW-DIY.h>
#include "addons/TokenHelper.h"
#include "credenciais.h"


FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseJsonArray arr;

#define ldr S_2

void setup() {
  Serial.begin(115200);
  pinMode(ldr, INPUT);

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
  if (Firebase.ready()) {
    Firebase.RTDB.getArray(&fbdo, F("/array/criar/lele"), &arr);
  }
}

unsigned long sendDataPrevMillis = 0;

void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > INTERVALO || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    arr.add(analogRead(ldr));
    Firebase.RTDB.setArray(&fbdo, F("/array/criar/lele"), &arr);
  }
}
