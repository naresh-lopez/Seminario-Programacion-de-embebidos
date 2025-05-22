int pinDisplay[10][8] = {{14,26,27,32,33,25},{27,32},{13,26,27,33,25},{13,26,27,33,32},{13,14,27,32},{13,26,14,33,32},{13,26,14,33,32,25},{13,26,27,32},{14,26,27,32,33,25,13},{14,26,27,32,33,13}}, numLineas[]={6,2,5,5,4,5,6,4,7,6}, cont1=0,cont0=0, *pcont1 = &cont1;
void setup() {}
void loop() {
  if(analogRead(4) < 100 && cont1 == 0) {
    cont0++;
    for(int i = 0; i<numLineas[8];i++, *pcont1 = 1) pinMode(pinDisplay[8][i], INPUT_PULLUP);
    }
  if(analogRead(4) > 500) for(int i = 0; i<numLineas[cont0%10] ;i++, *pcont1=0) pinMode(pinDisplay[cont0%10][i], OUTPUT_OPEN_DRAIN);
}