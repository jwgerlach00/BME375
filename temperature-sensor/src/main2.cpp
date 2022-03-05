#include <Arduino.h>
#include <EEPROM.h>

// Pin assignments
int thermistor_pin = A0;

// Variables
int thermistor_voltage;
int iteration = 0;
unsigned long prev_time;  // milliseconds
unsigned long current_time;  // milliseconds
unsigned int sample_freq = 60000;  // milliseconds | sample every per minute


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

int last_iteration() {
  // Reads when the last index was and starts of recording temperature from there

  // First element
  if (EEPROM.read(0) < 255) {
    return EEPROM.read(0);
  }
  // Second element
  else if (EEPROM.read(1) < 255) {
    return EEPROM.read(1);
  }
  // Third element
  else if (EEPROM.read(2) < 255) {
    return EEPROM.read(2);
  }
  // Fourth element
  else if (EEPROM.read(3) < 255) {
    return EEPROM.read(3);
  }
  // Memory full, reset
  else {
    return 0;
  }
}

void setup() {
  // Start serial output
  Serial.begin(9600);

  // Record program start time
  prev_time = millis();
  
  // Find last iteration
  iteration = last_iteration();
}

void loop() {
  current_time = millis();  // Get current time
  if (iteration < 1000 && current_time - prev_time >= sample_freq) {  // Ensures EEPROM storage is not exceeded
    // Successful, so update time
    prev_time = current_time;

    // Save iteration number in EEPROM
    if (iteration <= 255) {
      EEPROM.update(0, iteration);
    }
    else if (iteration <= 255 + 256) {
      EEPROM.update(1, iteration);
    }
    else if (iteration < 255 + 2*256) {
      EEPROM.update(2, iteration);
    }
    else if (iteration < 1000) {
      EEPROM.update(3, iteration);
    }

    // Output thermistor data
    thermistor_voltage = analogRead(thermistor_pin);

    // Save thermistor temp data to EEPROM
    EEPROM.update(iteration + 4, map(thermistor_voltage, 0, 1023, 0, 255));  // Originally write but update is efficient
    iteration += 1;

    // Print thermistor temp to serial monitor
    Serial.println(String(voltage_to_temp(thermistor_voltage)));
  }
}
