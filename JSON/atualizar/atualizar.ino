#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <laboratorioFW-DIY.h>
#include "addons/TokenHelper.h"
#include "credenciais.h"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(115200);

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
FirebaseJson json;
void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > INTERVALO || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    json.set("millis", millis());
    Firebase.RTDB.updateNode(&fbdo, F("/json/criar/lele"), &json);
  }
}
