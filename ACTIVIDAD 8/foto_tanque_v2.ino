#include "BluetoothSerial.h"  // Biblioteca Bluetooth integrada del ESP32
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

BluetoothSerial SerialBT;  // Crear objeto BluetoothSerial

// Conexiones de las fotoresistencias
#define inFront 39  // Frente centro
#define inFroL 34   // Frente izquierda
#define inFroR 36   // Frente derecha
#define inSidR 32   // lado DERECHO
#define inSidL 35   // lado IZQUIERDO
#define inReaR 25   // trasera derecha
#define inReaL 33   // trasera izquierda

// Umbral de la fotorresistencia
#define umPho 250

// Pines PWM de motores
#define lMo 19
#define rMo 4
// Pines ENABLE de motores
#define rMoA 18
#define rMoB 5
#define lMoA 17
#define lMoB 16

// Para control de la dirección de rotación
#define DER 1
#define IZQ 0

#define car_izq_suave 0
#define car_der_suave 1
#define car_izq_fuerte 2
#define car_der_fuerte 3
#define car_frente 4
#define car_espera 5



/* Inicialización de la pantalla OLED */
#define i2c_Address 0x3c  // Dirección I2C
#define SCREEN_WIDTH 128  // Ancho del OLED en píxeles
#define SCREEN_HEIGHT 64  // Alto del OLED en píxeles
#define OLED_RESET -1     // Sin pin de reset
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool mov = 0;
int modoActual = 0;  // Variable para manejar modos (0: espera, 1: seguidor, 2: manual)

// Frenamos los motores y luego les cortamos la corriente
void parar() {
  digitalWrite(lMoA, 1);
  digitalWrite(rMoA, 1);
  digitalWrite(lMoB, 1);
  digitalWrite(rMoB, 1);
  analogWrite(lMo, 255);
  analogWrite(rMo, 255);
  delay(250);
  analogWrite(lMo, 0);
  analogWrite(rMo, 0);
  digitalWrite(lMoA, 0);
  digitalWrite(rMoA, 0);
  digitalWrite(lMoB, 0);
  digitalWrite(rMoB, 0);
};

void rotacion(bool direccion) {
  digitalWrite(lMoA, direccion);
  digitalWrite(lMoB, !direccion);
  digitalWrite(rMoA, !direccion);
  digitalWrite(rMoB, direccion);
  analogWrite(lMo, 200);
  analogWrite(rMo, 200);
};

void traslacion(bool direccion) {
  digitalWrite(lMoA, !direccion);
  digitalWrite(lMoB, direccion);
  digitalWrite(rMoA, direccion);
  digitalWrite(rMoB, !direccion);
  analogWrite((direccion == DER) ? rMo : lMo, 200);
  analogWrite((direccion == DER) ? lMo : rMo, 100);
};

void avanzar() {
  digitalWrite(lMoA, 0);
  digitalWrite(lMoB, 1);
  digitalWrite(rMoA, 0);
  digitalWrite(rMoB, 1);
  analogWrite(lMo, 200);
  analogWrite(rMo, 200);
};

void retroceder() {
  digitalWrite(lMoA, 1);
  digitalWrite(lMoB, 0);
  digitalWrite(rMoA, 1);
  digitalWrite(rMoB, 0);
  analogWrite(lMo, 250);
  analogWrite(rMo, 250);
};

void cara(int i) {
  const char* carita[] = {
    "( 'o_o)", "(o_o' )", "( \"O_O)", "(O_O\" )", "(*o*)", "(-_-')"
  };
  display.clearDisplay();  // Limpiar la pantalla
  display.setTextSize(3);  // Tamaño de texto aumentado a 3
  display.setTextColor(SH110X_WHITE);

  // Centrando las caritas horizontalmente
  int16_t x = (SCREEN_WIDTH - (strlen(carita[i]) * 18)) / 2;  // 18px es aprox. el ancho por carácter con tamaño 3
  int16_t y = (SCREEN_HEIGHT - 24) / 2;                       // 24px es aprox. la altura de texto con tamaño 3

  display.setCursor(x, y);
  display.println(carita[i]);  // Mostrar la carita actual
  display.display();
};

int data[8];

void setup() {
  Serial.begin(115200);
  display.begin(i2c_Address, true); 
  display.clearDisplay();
  // Mostrar mensaje de inicio
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("Cargando...");
  display.display();
  //SerialBT.begin("Fototanque");  // Inicializar Bluetooth
  pinMode(lMoA, OUTPUT);
  pinMode(lMoB, OUTPUT);
  pinMode(rMoA, OUTPUT);
  pinMode(rMoB, OUTPUT);
  parar();                           // Prevenir rotaciones al inicio
  delay(500);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("\nFOTOTANQUE INICIADO");
  display.display();
  display.setTextSize(1);
  for(int x=0;x<10;x++){
    delay(500);
    display.print(" .");
    display.display();
    delay(500);
  }
}

void loop() {
  delay(250);
  data[7] = 0;
  data[0] = analogRead(inFront);
  data[1] = analogRead(inFroL);
  data[2] = analogRead(inFroR);
  data[3] = analogRead(inSidL);
  data[4] = analogRead(inSidR);
  data[5] = analogRead(inReaL);
  data[6] = analogRead(inReaR);
  for (int i = 0; i < 7; i++)
    data[7] += data[i];
  data[7] = data[7] / 7;
  if (data[0] > data[7]) {
    avanzar();
    cara(car_frente);
  }
  if (data[1] > data[7]) {
    traslacion(IZQ);
    cara(car_izq_suave);
  }
  if (data[2] > data[7]) {
    traslacion(DER);
    cara(car_der_suave);
  }
  if (data[3] > data[7] || data[5] > data[7]) {
    rotacion(DER);
    cara(car_izq_fuerte);
  }
  if (data[4] > data[7] || data[6] > data[7]) {
    rotacion(IZQ);
    cara(car_der_fuerte);
  }
}