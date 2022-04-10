#include<LiquidCrystal.h>
const int en = 9, rs = 8, d7 = 7, d6 = 6, d5 = 5, d4 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

unsigned char fs_out = 3, fs_in = 2;
float vol = 0.0, l_min_in, l_min_out;
volatile int flow_freq_in, flow_freq_out;
unsigned long currentTime;
unsigned long cloopTime;
void flow_in(){
  flow_freq_in++;
}
void flow_out(){
  flow_freq_out++;
}

const int pressureInput = A0;
const int sensorDelay = 250;

void setup() {
    Serial.begin(9600);
    lcd.begin(16,2);
    
    pinMode(fs_in, INPUT);
    pinMode(fs_out, INPUT);
    digitalWrite(fs_in, HIGH);
    digitalWrite(fs_out, HIGH);
    attachInterrupt(digitalPinToInterrupt(fs_in), flow_in, RISING);
    attachInterrupt(digitalPinToInterrupt(fs_out), flow_out, RISING);
  
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mega");
    lcd.setCursor(0, 1);
    lcd.print("Soaker");
    currentTime - millis();
    cloopTime - currentTime;
}

void loop() {
    currentTime = millis();
    if(currentTime >= (cloopTime + 1000)){
      cloopTime = currentTime;
      if(flow_freq_in > 0){
        lcd.clear();
        lcd.setCursor(0,0);
        l_min_in = (flow_freq_in / 7.5);
        l_min_in = l_min_in/60;
        vol = vol + l_min_in;
        lcd.print("W: ");
        lcd.print(vol);
        lcd.print(" L");
        Serial.println("IN");
        flow_freq_in = 0;
      }else{
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("W: ");
        lcd.print(vol);
        lcd.print(" L");
      }
      if(flow_freq_out > 0){
        lcd.clear();
        lcd.setCursor(0,0);
        l_min_out = (flow_freq_out / 7.5);
        l_min_out = l_min_out/60;
        vol = vol - l_min_out;
        lcd.print("W: ");
        lcd.print(vol);
        lcd.print(" L");
        Serial.println("OUT");
        flow_freq_out = 0;
      }else{
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("W: ");
        lcd.print(vol);
        lcd.print(" L");
      }
      int pressureValue = analogRead(pressureInput);
      float voltage = pressureValue * (5.0 / 1023.0);
      float mpa = ((voltage/5) - 0.1)/0.75;
      float bar = mpa * (-10);
      lcd.setCursor(0,1);
      lcd.print("P: ");
      lcd.print(bar);
      lcd.print("Bar");
      delay(sensorDelay);
    }
    //pressureTransducer();
}
