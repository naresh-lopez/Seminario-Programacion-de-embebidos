//DECLARACIONES GENERALES
#include <Arduino.h>

// Pines del display de 7 segmentos
#define lowPho 50   // Valor mínimo del fotómetro
#define hiPho 3700  // Valor máximo del fotómetro
#define phoRes 34   // Pin de entrada de la fotorresistencia
#define volIn 39    // Pin de entrada para voltaje
#define disPin0 12  // Pin inicial del display de 7 segmentos
/*
A - 12 
B - 13 
C - 14 
D - 15 
E - 16 
F - 17 
G - 18 
DP - 4
*/

// Pines para activar los dígitos
#define UNI 27
#define DEC 26
#define CEN 25

#define DP 4       // Pin del punto decimal
#define modeSel 36 // Pin del selector de modo (switch)

// Motor paso a paso (aguja analógica)
int bobina[] = {19, 21, 22, 23};
int retraso = 2; // Tiempo entre pasos (en ms)
int currentPosition = 0; // Posición actual del motor
int targetPosition = 0; // Posición objetivo del motor
#define maxSteps 800   // Número máximo de pasos permitido

// Variables para movimiento no bloqueante
unsigned long previousStepTime = 0; // Último tiempo en que se dio un paso
int stepIndex = 0; // Fase actual del motor paso a paso

// Configuración de las fases del motor paso a paso
int fase[4][4] = {
    {1, 0, 0, 0}, // Valor 0
    {0, 1, 0, 0}, // Valor 1
    {0, 0, 1, 0}, // Valor 2
    {0, 0, 0, 1}  // Valor 3
};

// Configuración de los segmentos (cátodo común)
const bool numeros[10][7] = {
    {1, 1, 1, 1, 1, 1, 0},  // 0
    {0, 1, 1, 0, 0, 0, 0},  // 1
    {1, 1, 0, 1, 1, 0, 1},  // 2
    {1, 1, 1, 1, 0, 0, 1},  // 3
    {0, 1, 1, 0, 0, 1, 1},  // 4
    {1, 0, 1, 1, 0, 1, 1},  // 5
    {1, 0, 1, 1, 1, 1, 1},  // 6
    {1, 1, 1, 0, 0, 0, 0},  // 7
    {1, 1, 1, 1, 1, 1, 1},  // 8
    {1, 1, 1, 1, 0, 1, 1}   // 9
};

// Función para activar los segmentos correspondientes a un dígito
void mostrarDigito(int digito) {
    for (int i = 0; i < 7; i++) {
        digitalWrite(disPin0 + i, numeros[digito][i]);
    }
}

// Función para mostrar valores en el display de 7 segmentos
void show(int value, bool modoVoltimetro) {
    int unidades = value % 10;
    int decenas = (value / 10) % 10;
    int centenas = (value / 100) % 10;

    // Mostrar centena
    digitalWrite(CEN, HIGH);
    mostrarDigito(centenas);
    digitalWrite(DP, modoVoltimetro); // Activar punto decimal solo en voltímetro
    delay(5);
    digitalWrite(DP, LOW);
    digitalWrite(CEN, LOW);

    // Mostrar decena
    digitalWrite(DEC, HIGH);
    mostrarDigito(decenas);
    delay(5);
    digitalWrite(DEC, LOW);

    // Mostrar unidad
    digitalWrite(UNI, HIGH);
    mostrarDigito(unidades);
    delay(5);
    digitalWrite(UNI, LOW);
}

// Función para el medidor de luz
void fotometro() {
    int phoRead = analogRead(phoRes); // Leer valor de la fotorresistencia
    int mappedValue = map(phoRead, lowPho, hiPho, 0, 100); // Convertir a rango 0-100
    show(mappedValue, false); // Mostrar valor sin punto decimal
    targetPosition = map(mappedValue, 0, 100, 0, maxSteps); // Mapear rango de aguja a pasos
    Serial.print("Fotómetro: ");
    Serial.println(mappedValue); // Enviar datos al monitor serial
}

// Función para el medidor de voltaje
void voltimetro() {
    int volRead = analogRead(volIn); // Leer voltaje
    int mappedValue = map(volRead, 0, 4095, 0, 330); // Convertir a rango 0-330
    show(mappedValue, true); // Mostrar valor con punto decimal activado
    targetPosition = map(mappedValue, 0, 330, 0, maxSteps); // Mapear rango de aguja a pasos
    Serial.print("Voltímetro: ");
    Serial.println(mappedValue); // Enviar datos al monitor serial
}

// Control del motor paso a paso
void paso(int valor) {
    for (int j = 0; j < 4; j++) {
        digitalWrite(bobina[j], fase[valor][j]);
    }
}

// Movimiento no bloqueante del motor paso a paso
void moverAguja() {
    if (currentPosition < targetPosition) {
        unsigned long currentTime = millis(); // Obtener tiempo actual
        if (currentTime - previousStepTime >= retraso) { // Verificar si se cumplió el tiempo
            previousStepTime = currentTime;
            paso(stepIndex); // Activar la fase actual del motor
            stepIndex = (stepIndex + 1) % 4; // Avanzar a la siguiente fase
            currentPosition++;
        }
    } else if (currentPosition > targetPosition) {
        unsigned long currentTime = millis(); // Obtener tiempo actual
        if (currentTime - previousStepTime >= retraso) { // Verificar si se cumplió el tiempo
            previousStepTime = currentTime;
            paso(stepIndex); // Activar la fase actual del motor
            stepIndex = (stepIndex - 1 + 4) % 4; // Retroceder a la fase anterior
            currentPosition--;
        }
    }
}

void setup() {
    Serial.begin(115200);

    // Configurar los pines del display de 7 segmentos
    for (int i = 0; i < 7; i++) {
        pinMode(disPin0 + i, OUTPUT);
        digitalWrite(disPin0 + i, LOW); // Apagar segmentos inicialmente
    }

    pinMode(DP, OUTPUT); // Configurar el punto decimal
    digitalWrite(DP, LOW); // Apagar punto decimal al inicio

    // Configurar los pines para los dígitos
    pinMode(UNI, OUTPUT);
    pinMode(DEC, OUTPUT);
    pinMode(CEN, OUTPUT);

    // Configurar el selector de modo
    pinMode(modeSel, INPUT);

    // Configurar pines del motor paso a paso
    for (int i = 0; i < 4; i++) {
        pinMode(bobina[i], OUTPUT);
        digitalWrite(bobina[i], LOW);
    }

    // Asumir que el motor está en la posición inicial 0
    currentPosition = 0; // Establece posición inicial como 0
}

void loop() {
    // Cambiar entre voltímetro y fotómetro según el estado del switch
    if (digitalRead(modeSel) == LOW) {
        fotometro();
    } else if (digitalRead(modeSel) == HIGH) {
        voltimetro();
    }

    // Ajustar la aguja sin bloquear la ejecución del código
    moverAguja();
}