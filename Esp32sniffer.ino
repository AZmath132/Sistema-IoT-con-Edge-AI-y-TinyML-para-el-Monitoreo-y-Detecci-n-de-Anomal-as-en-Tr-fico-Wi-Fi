#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include "esp_wifi.h"
#include <esp_now.h> 
#include <Adafruit_NeoPixel.h>
#define RGB_PIN 48 
#define NUM_LEDS 1
Adafruit_NeoPixel pixel(NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800);
unsigned long ultimoCambioLED = 0;
uint16_t posicionColor = 0;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Pines I2C
#define I2C_SDA 8
#define I2C_SCL 9

// Pin del Botón
#define BUTTON_PIN 4
#define ESTADO_PRESIONADO HIGH  

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- IMAGEN DEL DOGE ---
static const uint8_t image_data_Saraarray[1024] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x5f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x4b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x4b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x63, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x47, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe2, 0x7f, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xcd, 0xff, 0xff, 0x00, 0x80, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0x00, 0x02, 0xf0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0x80, 0x00, 0x19, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0x80, 0x00, 0x19, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x0d, 0x00, 0x3c, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xc2, 0xc0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0xf1, 0xa0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xcb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x87, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

// --- MAC DEL ESP32 #2 (RECEPTOR) ---
uint8_t macReceptor[] = {0x90, 0x70, 0x69, 0x33, 0x76, 0xF4};

// --- ESTRUCTURA DE DATOS A ENVIAR ---
typedef struct PaqueteDatos {
  unsigned long totalPaquetes;
  int paquetesPorIntervalo;
  int dispositivosDetectados;
} PaqueteDatos;

PaqueteDatos datosAEnviar;
unsigned long ultimoEnvioESPNOW = 0;
volatile int paquetesParaEnvio = 0;

esp_now_peer_info_t peerInfo;

// --- VARIABLES DE MENÚS ACTUALIZADOS ---
// ¡AQUÍ ESTÁ EL CAMBIO CLAVE! La opción 3 ahora es "Configuracion"
String menuPrincipal[] = {"1. Iniciar Sniffer", "2. Ver Estado", "3. Configuracion", "4. Pruebas ML"};
int totalPrincipal = 4;
int indicePrincipal = 0;

String subMenuSniffer[] = {"1. Grafico Trafico", "2. Escaner MAC/RSSI", "3. Volver al Menu"};
int totalSubMenu = 3;
int indiceSubMenu = 0;

String subMenuPruebas[] = {"1. Prueba Trafico Alto", "2. Volver al Menu"};
int totalSubMenuPruebas = 2;
int indiceSubMenuPruebas = 0;

// --- VARIABLES GLOBALES DE CONFIGURACIÓN (NUEVO) ---
bool modoMulticanal = false; // Falso = Monocanal, Verdadero = Multicanal
int canalActual = 1;
unsigned long ultimoCambioCanal = 0;

// --- MÁQUINA DE ESTADOS MODIFICADA ---
enum EstadosApp { 
  ESTADO_MENU_PRINCIPAL, 
  ESTADO_SUBMENU_SNIFFER, 
  ESTADO_SUBMENU_PRUEBAS,
  ESTADO_AFK, 
  ESTADO_ACCION, 
  ESTADO_SNIFFER_GRAFICO, 
  ESTADO_SNIFFER_MAC,
  ESTADO_PRUEBA_TRAFICO 
};
EstadosApp estadoActual = ESTADO_MENU_PRINCIPAL;

unsigned long ultimoTiempoActividad = 0;
const unsigned long TIEMPO_AFK = 30000;

// --- VARIABLES DEL BOTÓN ---
bool fuePresionado = false;
unsigned long tiempoInicioPresion = 0;

// --- VARIABLES DEL SNIFFER ---
volatile unsigned long totalPaquetes = 0;
volatile int paquetesTemporales = 0;
const int MAX_BARRAS = 30;     
int grafico[MAX_BARRAS];       
unsigned long ultimoUpdateGrafico = 0;
int maxPaquetesEscala = 10;

struct RegistroMAC {
  uint8_t direccion[6];
  int rssi;
};
#define MAX_MACS_PANTALLA 4
volatile RegistroMAC listaMACs[MAX_MACS_PANTALLA];
volatile int macsDetectadas = 0;

// ================= DECLARACIONES DE FUNCIONES =================
void mostrarPantallaCarga();
void dibujarMenuPrincipal();
void dibujarSubMenu();
void dibujarSubMenuPruebas();
void ejecutarOpcionPrincipal();
void ejecutarOpcionSubMenu();
void ejecutarOpcionSubMenuPruebas();
void iniciarModoSniffer();
void detenerModoSniffer();
void dibujarSnifferGrafico();
void dibujarSnifferMAC();
void dibujarPruebaTrafico();
void mostrarCambioDeModo(String nuevoModo);

// ================= FUNCIÓN CALLBACK DEL SNIFFER =================
void IRAM_ATTR wifi_promiscuous_cb(void* buf, wifi_promiscuous_pkt_type_t type) {
  totalPaquetes++;
  paquetesTemporales++;
  paquetesParaEnvio++; 
  
  if (estadoActual == ESTADO_SNIFFER_MAC) {
    wifi_promiscuous_pkt_t *paquete = (wifi_promiscuous_pkt_t *)buf;
    int senal_rssi = paquete->rx_ctrl.rssi;
    uint8_t *mac_origen = paquete->payload + 10;
    
    bool existe = false;
    for(int i=0; i < macsDetectadas; i++) {
      if(memcmp((void*)listaMACs[i].direccion, mac_origen, 6) == 0) {
        listaMACs[i].rssi = senal_rssi;
        existe = true;
        break;
      }
    }
    
    if(!existe) {
      int indice = (macsDetectadas < MAX_MACS_PANTALLA) ? macsDetectadas : 0;
      memcpy((void*)listaMACs[indice].direccion, mac_origen, 6);
      listaMACs[indice].rssi = senal_rssi;
      if (macsDetectadas < MAX_MACS_PANTALLA) {
        macsDetectadas++;
      }
    }
  }
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLDOWN); 

  pixel.begin();
  pixel.setBrightness(40); 
  pixel.show();
 
  Wire.begin(I2C_SDA, I2C_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for(;;); 
  }

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error inicializando ESP-NOW");
  } else {
    memcpy(peerInfo.peer_addr, macReceptor, 6);
    peerInfo.channel = 1; 
    peerInfo.encrypt = false;
    esp_now_add_peer(&peerInfo);
  }
  
  display.clearDisplay();
  display.drawBitmap(0, 0, image_data_Saraarray, 128, 64, 1);
  display.display();
  delay(2500);
  mostrarPantallaCarga();
  for(int i=0; i<MAX_BARRAS; i++){ grafico[i] = 0; }
  ultimoTiempoActividad = millis();
}
 
// ================= LOOP PRINCIPAL =================
void loop() {
  int lecturaBoton = digitalRead(BUTTON_PIN);
  bool botonActivo = (lecturaBoton == ESTADO_PRESIONADO);
  unsigned long tiempoActual = millis();

  if (millis() - ultimoCambioLED > 80) { // 80 es tu valor de 'wait'
    posicionColor++;
    if (posicionColor > 255) posicionColor = 0;
    
    pixel.setPixelColor(0, Wheel(posicionColor));
    pixel.show();
    
    ultimoCambioLED = millis();
  }

  // --- MÁQUINA DEL BOTÓN ---
  if (botonActivo && !fuePresionado) {
    fuePresionado = true;
    tiempoInicioPresion = tiempoActual;
    ultimoTiempoActividad = tiempoActual; 
    
    if (estadoActual == ESTADO_AFK) {
      estadoActual = ESTADO_MENU_PRINCIPAL;
      dibujarMenuPrincipal();
      tiempoInicioPresion = 0; 
    }
    delay(50); 
    
  } else if (!botonActivo && fuePresionado) {
    fuePresionado = false;
    ultimoTiempoActividad = tiempoActual;
    
    if (tiempoInicioPresion > 0) { 
      unsigned long duracion = tiempoActual - tiempoInicioPresion;
      // Pulsación Larga
      if (duracion >= 1500) {
        if (estadoActual == ESTADO_MENU_PRINCIPAL) {
          ejecutarOpcionPrincipal();
        } else if (estadoActual == ESTADO_SUBMENU_SNIFFER) {
          ejecutarOpcionSubMenu();
        } else if (estadoActual == ESTADO_SUBMENU_PRUEBAS) {
          ejecutarOpcionSubMenuPruebas();
        } else if (estadoActual == ESTADO_SNIFFER_GRAFICO || estadoActual == ESTADO_SNIFFER_MAC || estadoActual == ESTADO_PRUEBA_TRAFICO) {
          detenerModoSniffer();
          estadoActual = ESTADO_MENU_PRINCIPAL;
        }
      // Pulsación Corta
      } else if (duracion > 30) { 
        if (estadoActual == ESTADO_MENU_PRINCIPAL) {
          indicePrincipal = (indicePrincipal + 1) % totalPrincipal;
        } else if (estadoActual == ESTADO_SUBMENU_SNIFFER) {
          indiceSubMenu = (indiceSubMenu + 1) % totalSubMenu;
        } else if (estadoActual == ESTADO_SUBMENU_PRUEBAS) {
          indiceSubMenuPruebas = (indiceSubMenuPruebas + 1) % totalSubMenuPruebas;
        }
      }
    }
    delay(50);
  }

  // --- LÓGICA AFK ---
  if (estadoActual == ESTADO_MENU_PRINCIPAL || estadoActual == ESTADO_SUBMENU_SNIFFER || estadoActual == ESTADO_SUBMENU_PRUEBAS) {
    if (tiempoActual - ultimoTiempoActividad > TIEMPO_AFK) {
      estadoActual = ESTADO_AFK;
    }
  }

  // --- SALTO DE CANAL (SOLO MULTICANAL) ---
  if (modoMulticanal && (estadoActual == ESTADO_SNIFFER_GRAFICO || estadoActual == ESTADO_SNIFFER_MAC)) {
    if (tiempoActual - ultimoCambioCanal > 300) { 
      canalActual++;
      if (canalActual > 13) canalActual = 1;
      esp_wifi_set_channel(canalActual, WIFI_SECOND_CHAN_NONE);
      ultimoCambioCanal = tiempoActual;
    }
  }

  // --- ENVÍO DE DATOS ESP-NOW NORMAL ---
  if (estadoActual == ESTADO_SNIFFER_GRAFICO || estadoActual == ESTADO_SNIFFER_MAC) {
    if (tiempoActual - ultimoEnvioESPNOW > 1000) {
      ultimoEnvioESPNOW = tiempoActual;
      datosAEnviar.totalPaquetes = totalPaquetes;
      datosAEnviar.paquetesPorIntervalo = paquetesParaEnvio;
      datosAEnviar.dispositivosDetectados = macsDetectadas;
      
      esp_wifi_set_promiscuous(false);
      
      if(modoMulticanal) esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
      
      esp_now_send(macReceptor, (uint8_t *) &datosAEnviar, sizeof(datosAEnviar));
      
      if(modoMulticanal) esp_wifi_set_channel(canalActual, WIFI_SECOND_CHAN_NONE);
      
      esp_wifi_set_promiscuous(true);
      
      paquetesParaEnvio = 0;
    }
  }

  // --- ENVÍO DE DATOS ESP-NOW FALSOS ---
  if (estadoActual == ESTADO_PRUEBA_TRAFICO) {
    if (tiempoActual - ultimoEnvioESPNOW > 1000) {
      ultimoEnvioESPNOW = tiempoActual;
      datosAEnviar.totalPaquetes += random(150, 300);
      datosAEnviar.paquetesPorIntervalo = random(120, 180); 
      datosAEnviar.dispositivosDetectados = random(15, 25);
      esp_now_send(macReceptor, (uint8_t *) &datosAEnviar, sizeof(datosAEnviar));
    }
  }

  // --- REFRESCO DE PANTALLA ---
  if (estadoActual == ESTADO_MENU_PRINCIPAL) {
    dibujarMenuPrincipal();
  } else if (estadoActual == ESTADO_SUBMENU_SNIFFER) {
    dibujarSubMenu();
  } else if (estadoActual == ESTADO_SUBMENU_PRUEBAS) {
    dibujarSubMenuPruebas();
  } else if (estadoActual == ESTADO_AFK) {
    display.clearDisplay();
    display.drawBitmap(0, 0, image_data_Saraarray, 128, 64, 1);
    display.display();
  } else if (estadoActual == ESTADO_PRUEBA_TRAFICO) {
    dibujarPruebaTrafico();
  } else if (estadoActual == ESTADO_SNIFFER_GRAFICO) {
    if (tiempoActual - ultimoUpdateGrafico > 250) {
      ultimoUpdateGrafico = tiempoActual;
      for(int i = 0; i < MAX_BARRAS - 1; i++){ grafico[i] = grafico[i+1]; }
      grafico[MAX_BARRAS - 1] = paquetesTemporales;
      paquetesTemporales = 0;
      
      maxPaquetesEscala = 10;
      for(int i = 0; i < MAX_BARRAS; i++){
        if(grafico[i] > maxPaquetesEscala) maxPaquetesEscala = grafico[i];
      }
      dibujarSnifferGrafico();
    }
  } else if (estadoActual == ESTADO_SNIFFER_MAC) {
    if (tiempoActual - ultimoUpdateGrafico > 500) {
      ultimoUpdateGrafico = tiempoActual;
      dibujarSnifferMAC();
    }
  }
}

// ================= CONTROL LÓGICO =================

void iniciarModoSniffer() {
  totalPaquetes = 0;
  paquetesTemporales = 0;
  macsDetectadas = 0;
  paquetesParaEnvio = 0;
  for(int i=0; i<MAX_BARRAS; i++){ grafico[i] = 0; }
  
  WiFi.mode(WIFI_STA);
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_rx_cb(&wifi_promiscuous_cb);
  
  if (!modoMulticanal) {
    esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
  } else {
    canalActual = 1;
    esp_wifi_set_channel(canalActual, WIFI_SECOND_CHAN_NONE);
  }
}
void detenerModoSniffer() {
  esp_wifi_set_promiscuous(false);
  totalPaquetes = 0;
  paquetesTemporales = 0;
  macsDetectadas = 0;
  paquetesParaEnvio = 0;
}

// ================= NAVEGACIÓN Y MENÚS =================

void pantallaIniciando(String texto) {
  estadoActual = ESTADO_ACCION; 
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.print("Accion:");
  display.setCursor(0, 35);
  display.print(texto);
  display.display();
  delay(1000);
}

void ejecutarOpcionPrincipal() {
  if (indicePrincipal == 0) { 
    pantallaIniciando("Iniciando...");
    estadoActual = ESTADO_SUBMENU_SNIFFER;
    indiceSubMenu = 0; 
  } else if (indicePrincipal == 1) {
    pantallaIniciando("Estado: En Espera");
    estadoActual = ESTADO_MENU_PRINCIPAL;
  } else if (indicePrincipal == 2) {
    // ¡AQUÍ ESTÁ LA LÓGICA DE LA CONFIGURACIÓN!
    modoMulticanal = !modoMulticanal; 
    if (modoMulticanal) {
      mostrarCambioDeModo("MULTICANAL");
    } else {
      mostrarCambioDeModo("MONOCANAL");
      esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
    }
    estadoActual = ESTADO_MENU_PRINCIPAL;
  } else if (indicePrincipal == 3) {
    pantallaIniciando("Menu Pruebas");
    estadoActual = ESTADO_SUBMENU_PRUEBAS;
    indiceSubMenuPruebas = 0;
  }
}

void ejecutarOpcionSubMenu() {
  if (indiceSubMenu == 0) { 
    pantallaIniciando("Grafico Trafico");
    iniciarModoSniffer();
    estadoActual = ESTADO_SNIFFER_GRAFICO;
  } else if (indiceSubMenu == 1) { 
    pantallaIniciando("Escaner MAC");
    iniciarModoSniffer();
    estadoActual = ESTADO_SNIFFER_MAC;
  } else if (indiceSubMenu == 2) { 
    estadoActual = ESTADO_MENU_PRINCIPAL;
  }
}

void ejecutarOpcionSubMenuPruebas() {
  if (indiceSubMenuPruebas == 0) {
    pantallaIniciando("Generador de Ataque");
    datosAEnviar.totalPaquetes = 0; 
    estadoActual = ESTADO_PRUEBA_TRAFICO;
  } else {
    estadoActual = ESTADO_MENU_PRINCIPAL;
  }
}

// ================= DIBUJO DE INTERFACES =================

// ¡AQUÍ ESTÁ LA FUNCIÓN DEL DIBUJO DE LA TUERCA!
void mostrarCambioDeModo(String nuevoModo) {
  display.clearDisplay();
  
  // Dibujo de la tuerca
  display.drawCircle(64, 25, 12, SSD1306_WHITE);
  display.drawCircle(64, 25, 5, SSD1306_WHITE);
  display.drawLine(64, 8, 64, 42, SSD1306_WHITE);  
  display.drawLine(47, 25, 81, 25, SSD1306_WHITE); 
  display.drawLine(52, 13, 76, 37, SSD1306_WHITE); 
  display.drawLine(52, 37, 76, 13, SSD1306_WHITE); 
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  display.setCursor(30, 45);
  display.print("Modo Set:");
  
  display.setCursor(30, 55);
  display.print(nuevoModo);
  
  display.display();
  delay(1500); 
}

void mostrarPantallaCarga() {
  for(int i = 0; i <= 100; i += 2) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(35, 20);
    display.print("Loading...");
    display.drawRect(14, 40, 100, 10, SSD1306_WHITE);
    display.fillRect(14, 40, i, 10, SSD1306_WHITE);
    display.display();
    delay(10); 
  }
}

void dibujarListaMenu(String titulo, String opciones[], int total, int indice) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("BTN ON");
  display.fillCircle(42, 3, 3, SSD1306_WHITE); 
  display.setCursor(128 - (titulo.length() * 6), 0);
  display.print(titulo);
  display.drawLine(0, 14, 128, 14, SSD1306_WHITE);
  for (int i = 0; i < total; i++) {
    int yPos = 20 + (i * 11);
    if (i == indice) {
      display.fillRect(0, yPos - 1, 128, 10, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    } else {
      display.setTextColor(SSD1306_WHITE, SSD1306_BLACK); 
    }
    display.setCursor(4, yPos);
    display.print(opciones[i]);
  }
  display.display();
}

void dibujarMenuPrincipal() {
  dibujarListaMenu("MENU", menuPrincipal, totalPrincipal, indicePrincipal);
}

void dibujarSubMenu() {
  dibujarListaMenu("MODOS", subMenuSniffer, totalSubMenu, indiceSubMenu);
}

void dibujarSubMenuPruebas() {
  dibujarListaMenu("ML DATA", subMenuPruebas, totalSubMenuPruebas, indiceSubMenuPruebas);
}

void dibujarPruebaTrafico() {
  display.clearDisplay();
  bool blink = (millis() / 200) % 2 == 0;
  
  if (blink) {
    display.fillRect(0, 0, 128, 64, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
  } else {
    display.setTextColor(SSD1306_WHITE);
  }
  
  display.setTextSize(2);
  display.setCursor(18, 10);
  display.print("ATAQUE!!");
  
  display.setTextSize(1);
  display.setCursor(10, 35);
  display.print("Inyectando Dataset");
  
  display.setCursor(10, 50);
  display.print("Rrafaga: "); 
  display.print(datosAEnviar.paquetesPorIntervalo);
  display.print(" PKT/s");
  
  display.display();
}

void dibujarSnifferGrafico() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  display.setCursor(0, 0); display.print("PKTs: "); display.print(totalPaquetes);
  display.setCursor(85, 0); display.print("GRAF");
  if ((millis() / 500) % 2 == 0) display.fillCircle(120, 3, 3, SSD1306_WHITE); 
  display.drawLine(0, 14, 128, 14, SSD1306_WHITE);
  
  int alturaMaxima = 48; int baseY = 63; 
  for (int i = 0; i < MAX_BARRAS; i++) {
    int alturaBarra = map(grafico[i], 0, maxPaquetesEscala, 0, alturaMaxima);
    int posX = 4 + (i * 4);
    display.fillRect(posX, baseY - alturaBarra, 3, alturaBarra, SSD1306_WHITE);
  }
  display.display();
}

void dibujarSnifferMAC() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  display.setCursor(0, 0); display.print("M: "); display.print(totalPaquetes); 
  display.setCursor(75, 0); display.print("RASTRO");
  if ((millis() / 500) % 2 == 0) display.fillCircle(120, 3, 3, SSD1306_WHITE); 
  display.drawLine(0, 14, 128, 14, SSD1306_WHITE);
  
  for (int i = 0; i < macsDetectadas; i++) {
    int yPos = 18 + (i * 11);
    display.setCursor(0, yPos);
    
    for(int j=0; j<6; j++) {
      if(listaMACs[i].direccion[j] < 16) display.print("0"); 
      display.print(listaMACs[i].direccion[j], HEX);
      if(j < 5) display.print(":");
    }
    
    display.setCursor(95, yPos);
    display.print(listaMACs[i].rssi);
    display.print("dB");
  }
  display.display();
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixel.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixel.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixel.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}