
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <laboratorioFW-DIY.h>
#include "addons/TokenHelper.h"
#include "credenciais.h"


FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

#define btnCriar B_0
#define btnDeletar B_3

void setup() {
  Serial.begin(115200);
  pinMode(btnCriar, INPUT);
  pinMode(btnDeletar, INPUT);

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

void loop() {
  if (Firebase.ready()) {
    if (digitalRead(btnCriar)) {
      Firebase.RTDB.setBool(&fbdo, F("/crud/deletar/lele"), true);
      Serial.println("criado");
    } else if (digitalRead(btnDeletar)) {
      Firebase.RTDB.deleteNode(&fbdo, F("/crud/deletar/lele"));
      Serial.println("deletado");
    }
    delay(300);
  }
}