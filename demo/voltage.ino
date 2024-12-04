// Constants
const int voltageSensorPin = A0;  // Analog pin connected to the voltage sensor
const float referenceVoltage = 5.0;  // Reference voltage (adjust based on your board)

// Calibration factor based on your voltage divider or sensor specifications
const float calibrationFactor = 4.87; // Adjust for your setup (e.g., 11 for a typical voltage divider with 10k and 1k resistors)

void setup() {
  Serial.begin(9600);  // Start the Serial Monitor at 9600 baud rate
  pinMode(voltageSensorPin, INPUT);  // Set the voltage sensor pin as input
}

void loop() {
  // Read the analog value from the voltage sensor
  int sensorValue = analogRead(voltageSensorPin);
  
  // Convert the analog value to voltage
  float measuredVoltage = (sensorValue * referenceVoltage) / 1023.0;

  // Apply calibration factor to get the actual voltage
  float actualVoltage = measuredVoltage * calibrationFactor-0.07;

  // Print the voltage to the Serial Monitor
  Serial.print("Measured Voltage: ");
  Serial.print(actualVoltage);
  Serial.println(" V");

  delay(1000);  // Wait for 1 second before the next reading
}