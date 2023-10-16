#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <laboratorioFW-DIY.h>
#include "addons/TokenHelper.h"
#include "credenciais.h"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

#define btn B_0

void setup() {

  Serial.begin(115200);
  pinMode(B_0, INPUT);

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
    if (digitalRead(btn)) {
      const char* nome = String("/crud/criar/lele/" + String(millis())).c_str();
      bool sucesso = Firebase.RTDB.setBool(&fbdo, F(nome), true);
      if (sucesso) {
        Serial.println("criado");
      } else {
        Serial.println(fbdo.errorReason().c_str());
      }
    }
    delay(300);
  }
}
