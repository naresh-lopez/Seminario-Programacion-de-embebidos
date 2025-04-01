const int ledPin = 26;
const int  portPin = 34;
int potValor = 0;
float angulo = 0;
int perc = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);

}

void loop() {
  // put your main code here, to run repeatedly:
  potValor = analogRead(portPin);
  angulo = (analogRead(portPin)*270)/4095;
  perc = (analogRead(portPin)*100)/4095;
  Serial.print("angulo: "+String(angulo));
  Serial.print("\tValor leido: " + String(potValor));
  Serial.println("\tPorcentaje: " + String(perc));  
  analogWrite(ledPin,(potValor*255)/4095);
  delay(100);
}
