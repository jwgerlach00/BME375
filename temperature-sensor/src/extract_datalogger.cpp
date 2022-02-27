#include <Arduino.h>
#include <EEPROM.h>


int iterator = 0;
int voltage;

double voltage_to_temp(int v, bool f=false) {
  // Converts voltage from thermistor reading to temperature

  double temp_k = log(10000.0 * ((1024.0 / v - 1)));
  temp_k = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * temp_k * temp_k )) * temp_k );  // Kelvin
  float temp_c = temp_k - 273.15;  // Celcius
  float temp_f = (temp_c * 9.0)/ 5.0 + 32.0;  // Fahrenheit

  // Return Fahrenheit if specified, else Celcius
  if(f) {
    return temp_f;
  }
  else {
    return temp_c;
  }
}

void setup() {
    Serial.begin(9600);
}

void loop() {
    // Extract voltage, convert to temperature, and print
    voltage = EEPROM.read(iterator);
    if (voltage != 255){
        Serial.println(voltage_to_temp(map(voltage, 0, 255, 0, 1023)));
        iterator += 1;
    }
}
