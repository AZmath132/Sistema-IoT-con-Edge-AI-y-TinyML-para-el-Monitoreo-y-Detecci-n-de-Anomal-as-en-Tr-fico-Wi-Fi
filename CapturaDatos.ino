#include <WiFi.h>
#include <esp_now.h>
#include "esp_wifi.h"
#include <Adafruit_NeoPixel.h>
#include <ESPAsyncWebServer.h> // Librería: ESPAsyncWebServer
#include <AsyncTCP.h>          // Librería: AsyncTCP
#include <ArduinoJson.h>       // Librería: ArduinoJson

// --- CONFIGURACIÓN WI-FI ---
const char* ssid = "ESP32_Sniffer_Net";
const char* password = "password123";

// --- PINES Y CONFIGURACIÓN ---
#define RGB_PIN 48
#define NUM_LEDS 1
#define BUZZER_PIN 5
#define UMBRAL_TRAFICO 20
#define TIEMPO_TIMEOUT 3000

Adafruit_NeoPixel pixel(NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800);
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

typedef struct PaqueteDatos {
  unsigned long totalPaquetes;
  int paquetesPorIntervalo;
  int dispositivosDetectados;
} PaqueteDatos;

volatile bool nuevoDatoDisponible = false;
volatile int paquetesActuales = 0;
volatile int dispositivosActuales = 0;
unsigned long ultimaVezRecibido = 0;
bool mostrandoEspera = false;

void enviarDatosApp(int paquetes, bool anomalia) {
  if (ws.count() > 0) { 
    StaticJsonDocument<128> doc;
    doc["timestamp"] = millis();
    doc["paquetes"] = paquetes;
    doc["anomalia"] = anomalia;
    
    String output;
    serializeJson(doc, output);
    ws.textAll(output);
  }
}

void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  if (len == sizeof(PaqueteDatos)) {
    PaqueteDatos datos;
    memcpy(&datos, incomingData, sizeof(datos));
    paquetesActuales = datos.paquetesPorIntervalo;
    dispositivosActuales = datos.dispositivosDetectados;
    nuevoDatoDisponible = true;
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000); 
  Serial.println("\n--- INICIANDO CAPTURADOR ---");

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);

  pixel.begin();
  pixel.setPixelColor(0, pixel.Color(255, 255, 0)); 
  pixel.show();

  WiFi.mode(WIFI_AP_STA); 
  if (WiFi.softAP(ssid, password, 1)) {
    Serial.println("AP Iniciado correctamente");
  } else {
    Serial.println("Error al iniciar AP");
  }

  Serial.print("IP del Servidor: ");
  Serial.println(WiFi.softAPIP());
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error inicializando ESP-NOW. Reiniciando...");
    delay(1000);
    ESP.restart();
  }
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Sistema Listo. Esperando datos...");
  pixel.setPixelColor(0, pixel.Color(0, 0, 255)); 
  pixel.show();

  ws.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) Serial.println("App Flutter Conectada");
  });
  server.addHandler(&ws);
  server.begin();
}
void loop() {
  unsigned long tiempoActual = millis();

  if (nuevoDatoDisponible) {
    nuevoDatoDisponible = false;
    ultimaVezRecibido = tiempoActual;
    mostrandoEspera = false;
    
    bool anomalia = (paquetesActuales >= UMBRAL_TRAFICO);
    enviarDatosApp(paquetesActuales, anomalia);
    
    Serial.printf("%lu,%d,%d\n", tiempoActual, paquetesActuales, dispositivosActuales);
  }
  if (tiempoActual - ultimaVezRecibido >= TIEMPO_TIMEOUT && !mostrandoEspera) {
    paquetesActuales = 0;
    enviarDatosApp(0, false);
    mostrandoEspera = true;
  }
  if (paquetesActuales >= UMBRAL_TRAFICO) {
    digitalWrite(BUZZER_PIN, LOW);
    pixel.setPixelColor(0, pixel.Color(255, 0, 0));
  } else {
    digitalWrite(BUZZER_PIN, HIGH);
    pixel.setPixelColor(0, mostrandoEspera ? pixel.Color(0, 0, 255) : pixel.Color(0, 255, 0));
  }
  pixel.show();
  delay(10);
}