#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <laboratorioFW-DIY.h>
#include "addons/TokenHelper.h"
#include "credenciais.h"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

#define led L_0
#define btn B_0

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(btn, OUTPUT);

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


FirebaseJsonArray arr;

void loop() {
  if (Firebase.ready()) {
    if (digitalRead(btn)) {
      Firebase.RTDB.getArray(&fbdo, F("/array/ler"), &arr);
      int tamanho = arr.iteratorBegin();
      for (int i = 0; i < tamanho; i++) {
        int intensidade = arr.valueAt(i).value.toInt();
        Serial.println(intensidade);
        analogWrite(led, intensidade);
        delay(500);
      }
      arr.iteratorEnd();
    }
  }
}
