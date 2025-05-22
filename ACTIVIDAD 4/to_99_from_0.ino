#define bLed 2

// Baud rate
#define baud 115200

#define butPin 5

#define w1s 50

// Pines del display de 7 segmentos
#define G7 6
#define B7 7
#define F7 12
#define A7 11
#define E7 10
#define D7 8
#define C7 15
#define DP7 35

// Pines de selección de dígitos
#define uni 13
#define dec 5
#define cen 4
#define mil 3

int uniSeg;
  int decSeg=0;
  int uniMin=0;
  int decMin=0;

void apagar();
void mostrar(int numero, int digito);
void mostrarNumero(int numero);

void setup() {
  Serial.begin(baud);

  // Configuración de pines
  pinMode(butPin, INPUT);

  pinMode(A7, OUTPUT);
  pinMode(B7, OUTPUT);
  pinMode(C7, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(E7, OUTPUT);
  pinMode(F7, OUTPUT);
  pinMode(G7, OUTPUT);
  pinMode(DP7, OUTPUT);

  pinMode(uni, OUTPUT);
  pinMode(dec, OUTPUT);
  pinMode(cen, OUTPUT);
  pinMode(mil, OUTPUT);
}

void loop() {
  for (int i = 0; i < 1000000; i++) {
    unsigned long lastMillis = millis();
    while (millis() - lastMillis < w1s) {  // Mantener el número visible durante 1 segundo
      mostrarNumero(i);
    }
  }
}

void mostrarNumero(int numero) {
   uniSeg = numero % 10;
   decSeg = (numero / 10);
  if(decSeg%6==0){
    uniMin++;
  }
  
  

  for (int j = 0; j < 20; j++) {  // Refrescar varias veces para estabilidad
    mostrar(decMin, mil);
    delay(1);
    apagar();


    mostrar(uniMin, cen);
    delay(1);
    apagar();

    mostrar(decSeg, dec);
    delay(1);
    apagar();

    mostrar(uniSeg, uni);
    delay(1);
    apagar();
  }
}

void mostrar(int numero, int digito) {
  // Apagar todos los dígitos antes de encender el correcto
  digitalWrite(uni, LOW);
  digitalWrite(dec, LOW);
  digitalWrite(cen, LOW);
  digitalWrite(mil, LOW);

  digitalWrite(digito, HIGH);  // Activar el dígito correspondiente

  // Encender segmentos según el número
  switch (numero) {
    case 0:
      digitalWrite(A7, HIGH);
      digitalWrite(B7, HIGH);
      digitalWrite(C7, HIGH);
      digitalWrite(D7, HIGH);
      digitalWrite(E7, HIGH);
      digitalWrite(F7, HIGH);
      break;
    case 1:
      digitalWrite(B7, HIGH);
      digitalWrite(C7, HIGH);
      break;
    case 2:
      digitalWrite(A7, HIGH);
      digitalWrite(B7, HIGH);
      digitalWrite(G7, HIGH);
      digitalWrite(E7, HIGH);
      digitalWrite(D7, HIGH);
      break;
    case 3:
      digitalWrite(A7, HIGH);
      digitalWrite(B7, HIGH);
      digitalWrite(G7, HIGH);
      digitalWrite(C7, HIGH);
      digitalWrite(D7, HIGH);
      break;
    case 4:
      digitalWrite(F7, HIGH);
      digitalWrite(G7, HIGH);
      digitalWrite(B7, HIGH);
      digitalWrite(C7, HIGH);
      break;
    case 5:
      digitalWrite(A7, HIGH);
      digitalWrite(F7, HIGH);
      digitalWrite(G7, HIGH);
      digitalWrite(C7, HIGH);
      digitalWrite(D7, HIGH);
      break;
    case 6:
      digitalWrite(A7, HIGH);
      digitalWrite(F7, HIGH);
      digitalWrite(G7, HIGH);
      digitalWrite(C7, HIGH);
      digitalWrite(D7, HIGH);
      digitalWrite(E7, HIGH);
      break;
    case 7:
      digitalWrite(A7, HIGH);
      digitalWrite(B7, HIGH);
      digitalWrite(C7, HIGH);
      break;
    case 8:
      digitalWrite(A7, HIGH);
      digitalWrite(B7, HIGH);
      digitalWrite(C7, HIGH);
      digitalWrite(D7, HIGH);
      digitalWrite(E7, HIGH);
      digitalWrite(F7, HIGH);
      digitalWrite(G7, HIGH);
      break;
    case 9:
      digitalWrite(A7, HIGH);
      digitalWrite(B7, HIGH);
      digitalWrite(C7, HIGH);
      digitalWrite(D7, HIGH);
      digitalWrite(F7, HIGH);
      digitalWrite(G7, HIGH);
      break;
  }
}

void apagar() {
  digitalWrite(A7, LOW);
  digitalWrite(B7, LOW);
  digitalWrite(C7, LOW);
  digitalWrite(D7, LOW);
  digitalWrite(E7, LOW);
  digitalWrite(F7, LOW);
  digitalWrite(G7, LOW);
  digitalWrite(DP7, LOW);
}
