#include <Arduino.h>
#include <LiquidCrystal.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

// Class instantiations
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
DHT dht(DHTPIN, DHTTYPE);

// Pin assignments
int thermistor_pin = A0;

// Variables
int thermistor_voltage;

void setup() {
  lcd.begin(16, 2);
  dht.begin();
}

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

void loop() {
  // Output thermistor data
  thermistor_voltage = analogRead(thermistor_pin);
  lcd.setCursor(0, 0);
  lcd.print("Temp " + String(voltage_to_temp(thermistor_voltage)) + " C");

  // Output DHT sensor data
  lcd.setCursor(0, 1);
  lcd.print(String(dht.readTemperature()) + " C, " + String(dht.readHumidity()) + " %");

  // Update every 1/2 second
  delay(500);
}