#include <ESP32Servo.h>

//conexiones de las fotoresistencias
#define inXM 39 //verde
#define inXP 36 //amarillo
#define inYM 35 //blanco
#define inYP 34 //azul

//umbral de la fotorresistencia
#define umPho 250 

//Pines de Servomotores
#define seMoX 25 
#define seMoY 26
#define seMoZ 27
#define seMoP 14

#define maX 180
#define maY 65
#define maxP 15
#define minP 0

Servo motX;
Servo motY;
Servo motZ;
Servo pinza;

int x = 90 ,y = 45, z = 90, claw = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  motX.attach(seMoX);
  motY.attach(seMoY);
  motZ.attach(seMoZ);
  pinza.attach(seMoP);

  motX.write(x);
  motY.write(y);
  motZ.write(z);
  pinza.write(claw);
  delay(500);
  Serial.println("PINZA INICIADA");
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(analogRead(inXP)>(analogRead(inXM)+umPho)){
    if(x>(0)) x-=5;
  }
  if(analogRead(inXM)>(analogRead(inXP)+150+umPho)){
    if(x<(170)) x+=5;
  }
  if(analogRead(inYP)>(analogRead(inYM)+umPho-50)){
    if(y>(10)) y-=5;
  }
  if(analogRead(inYM)>(analogRead(inYP)+umPho-50)){
    if(y<(maY)) y+=5;
  }
  
  motX.write(x);
  motZ.write(z);
  delay(10);
  motY.write(y);
  
  Serial.print("X");
  Serial.println(x);
  Serial.print("Y");
  Serial.println(y);
  delay(100);
}
