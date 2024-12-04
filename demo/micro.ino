#include <Arduino.h>
#include "model.h"

// Define the analog pin connected to the LM35 sensor
const int tempPin = A0;

// Initial values of L and C
const float initial_L = 1e-3;  // 1 mH (in Henries)
const float initial_C = 4.7e-9;  // 4.7 nF (in Farads)

float target_freq = 150.5;  // Example target frequency in kHz, change as needed

// Function to convert inductance to millihenries (mH)
String convert_to_mh(float value) {
  return String(value * 1e3, 3) + " mH";  // Convert to millihenries (mH)
}

// Function to convert capacitance to nanofarads (nF)
String convert_to_nf(float value) {
  return String(value * 1e9, 3) + " nF";  // Convert to nanofarads (nF)
}

float predict_frequency(float temperature) {
    float scaled_temp = (temperature - MEAN) / STD_DEV;
    
    float prediction = INTERCEPT;
    float term = scaled_temp; 

    for (int i = 0; i < DEGREE; i++) {
        prediction += COEFF[i] * term;
        term *= scaled_temp;    
    }
    
    return prediction;  // Predicted frequency in kHz
}

void setup() {
  Serial.begin(9600);
  Serial.println("Temperature Frequency Prediction System Initialized");
}

void loop() {
  // Read the analog value from the LM35
  int analogValue = analogRead(tempPin);

  // Convert analog reading to voltage (assuming a 5V reference)
  float voltage = analogValue * (5.0 / 1023.0);

  // Convert voltage to Celsius for LM35 (10mV per °C)
  float temperature = voltage * 100.0;

  // Predict the frequency based on temperature using the model
  float predicted_freq = predict_frequency(temperature);  // Replace with actual model function

  // Calculate the frequency change factor (f_factor = target_freq / predicted_freq)
  float f_factor = target_freq / predicted_freq;

  // Adjust L and C based on the frequency change factor
  float new_L = initial_L * (f_factor * f_factor);
  float new_C = initial_C / (f_factor * f_factor);

  // Calculate the change in inductance and capacitance
  float delta_L = new_L - initial_L;
  float delta_C = new_C - initial_C;

  // Display the results
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Predicted Frequency: ");
  Serial.print(predicted_freq);
  Serial.println(" kHz");

  Serial.print("Target frequency: ");
  Serial.print(target_freq);
  Serial.println(" kHz");

  Serial.print("Change factor: ");
  Serial.println(f_factor, 4);

  // Output the required inductance and capacitance values
  Serial.print("Required inductance (L) for the target frequency: ");
  Serial.println(convert_to_mh(new_L));

  Serial.print("Required capacitance (C) for the target frequency: ");
  Serial.println(convert_to_nf(new_C));

  // Output the change in inductance and capacitance
  Serial.print("Change in inductance: ");
  Serial.println(convert_to_mh(delta_L));

  Serial.print("Change in capacitance: ");
  Serial.println(convert_to_nf(delta_C));

  delay(4000);  // Delay for 1 second
}
