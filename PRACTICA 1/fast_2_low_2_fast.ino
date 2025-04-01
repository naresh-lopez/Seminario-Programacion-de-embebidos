/*
Author: Naresh Satoshi Lopez Ojeda
Date: 23-01-2024
Objective:


*/

#define buiLED 2

int spd = 100, maxmun = 2000, minmun = 100, change = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(buiLED, OUTPUT);
  
  digitalWrite(buiLED,1);
  delay(500);
  digitalWrite(buiLED,0);
}

void loop() {
  // put your main code here, to run repeatedly:
  

  if(spd == maxmun){
    change = -100;
  }
  if(spd == minmun){
    change = 100;
  }
  spd = spd + change;
  digitalWrite(buiLED,0);
  delay(spd);
  digitalWrite(buiLED,1);
  delay(spd);
  
  
}
