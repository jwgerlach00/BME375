#include <Arduino.h>
#include <LiquidCrystal.h>
#include <DHT.h>
#include <EEPROM.h>

#define DHTPIN 2
#define DHTTYPE DHT11

// Class instantiations
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
DHT dht(DHTPIN, DHTTYPE);

// Pin assignments
int thermistor_pin = A0;

// Variables
int thermistor_voltage;
int stored_voltage;
int iteration = 0;

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
  lcd.begin(16, 2);
  dht.begin();

  // Display the last stored temp for 20 seconds
  stored_voltage = EEPROM.read(0);
  lcd.print("LAST STORED TEMP");
  lcd.setCursor(0, 1);
  lcd.print(String(voltage_to_temp(map(stored_voltage, 0, 255, 0, 1023))) + " Centigrade");
  delay(20000);
}

void loop() {
  if (iteration < 1000) {
    // Output thermistor data
    thermistor_voltage = analogRead(thermistor_pin);

    // Save thermistor temp data to EEPROM
    EEPROM.update(iteration, map(thermistor_voltage, 0, 1023, 0, 255));  // Originally write but update is more efficient
    iteration += 1;

    // Print thermistor to lcd
    lcd.setCursor(0, 0);
    lcd.print("Temp " + String(voltage_to_temp(thermistor_voltage)) + " C");

    // Output DHT sensor data
    lcd.setCursor(0, 1);
    lcd.print(String(dht.readTemperature()) + " C, " + String(dht.readHumidity()) + " %");

    // Sample temperature about 1 time per minute
    delay(60000);
  }
}
