#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <laboratorioFW-DIY.h>
#include "addons/TokenHelper.h"
#include "credenciais.h"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseData stream;

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

  Firebase.RTDB.beginMultiPathStream(&fbdo, F("/callback/rgb"));

  Firebase.RTDB.setMultiPathStreamCallback(&fbdo, streamCallback, streamTimeoutCallback);
}

void streamCallback(MultiPathStream data) {
  coloreLed(ledR, "vermelho", data);
  coloreLed(ledG, "verde", data);
  coloreLed(ledB, "azul", data);
}


void streamTimeoutCallback(bool timeout) {
  if (timeout) {
    Serial.println("stream timeout, resuming...\n");
  }
}


void loop() {
}

void coloreLed(int led, String path, MultiPathStream data) {
  if (data.get(path)) {
    int intensidade = data.value.toInt();
    Serial.println(intensidade);
    analogWrite(led, intensidade);
  }
}
