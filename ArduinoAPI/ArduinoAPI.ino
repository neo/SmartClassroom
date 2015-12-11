#ifndef SERIAL_RATE
#define SERIAL_RATE         115200
#endif

#ifndef SERIAL_TIMEOUT
#define SERIAL_TIMEOUT      5
#endif

// include the library code:
#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// TMP102
#include <Wire.h>
int tmp102Address = 0x48;

void setup() {
    Serial.begin(SERIAL_RATE);
    Serial.setTimeout(SERIAL_TIMEOUT);

    int cmd = readData();
    for (int i = 0; i < cmd; i++) {
        pinMode(readData(), OUTPUT);
    }

    lcd.begin(16, 2);
    lcd.print("Today:0");
    lcd.setCursor(0, 1);
    lcd.print("Total:0");
    lcd.setCursor(10, 0);
    lcd.print("TMP:00");

    Wire.begin();
}

void loop() {
    switch (readData()) {
        case 0 :
            //set digital low
            digitalWrite(readData(), LOW); break;
        case 1 :
            //set digital high
            digitalWrite(readData(), HIGH); break;
        case 2 :
            //get digital value
            Serial.println(digitalRead(readData())); break;
        case 3 :
            // set analog value
            analogWrite(readData(), readData()); break;
        case 4 :
            //read analog value
            Serial.println(analogRead(readData())); break;
        case 5 :
            //get the internal temperature
            //Serial.println(GetTemp(),1); break; // Internal
            //Serial.println(getTemperature(),1); break; // External
            Serial.println(getTmp()); break;
        case 6 :
            //LCD
            int today;
            int total;
            int tmp;
            today = readData();
            total = readData();
            tmp = readData();
            lcd.setCursor(6, 0);
            lcd.print(today);
            lcd.setCursor(6, 1);
            lcd.print(total);
            lcd.setCursor(14, 0);
            lcd.print(tmp); break;
        case 99:
            //just dummy to cancel the current read, needed to prevent lock 
            //when the PC side dropped the "w" that we sent
            break;
    }
}

//char readData() {
int readData() {
    Serial.println("w");
    while(1) {
        if(Serial.available() > 0) {
            return Serial.parseInt();
        }
    }
}

float getTmp() {
  int samples = 100;
  float total = 0;
  for(int i = 0; i < samples; i++) {
    total += GetTemp();
    delay(1000 / samples);
  }
  return total / samples - 4;
}

float getTemperature(){
//double getTemperature(){
  Wire.requestFrom(tmp102Address,2); 

  byte MSB = Wire.read();
  byte LSB = Wire.read();

  //it's a 12bit int, using two's compliment for negative
  int TemperatureSum = ((MSB << 4) | LSB) >> 4; 

  float celsius = TemperatureSum*0.0625;
  return celsius;
}

double GetTemp(void)
{
  unsigned int wADC;
  double t;

  // The internal temperature has to be used
  // with the internal reference of 1.1V.
  // Channel 8 can not be selected with
  // the analogRead function yet.

  // Set the internal reference and mux.
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC

  delay(20);            // wait for voltages to become stable.

  ADCSRA |= _BV(ADSC);  // Start the ADC

  // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));

  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  wADC = ADCW;

  // The offset of 324.31 could be wrong. It is just an indication.
  t = (wADC - 324.31 ) / 1.22;

  // The returned temperature is in degrees Celcius.
  return (t);
}
