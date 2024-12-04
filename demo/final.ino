#include <Arduino.h>
#include <math.h>
#include <DHT.h> // Include DHT sensor library

// DHT11 Sensor Configuration
#define DHTPIN 2      // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11 sensor type
class VoltageFrequencyConverter
{
private:
    float slope;
    float intercept;

public:
    VoltageFrequencyConverter(float slopeValue, float interceptValue)
        : slope(slopeValue), intercept(interceptValue) {}

    float predictFrequency(float voltage)
    {
        return slope * voltage + intercept;
    }

    float frequencyDifference(float userVoltage, float referenceVoltage = 9.0)
    {
        float userFrequency = predictFrequency(userVoltage);
        float referenceFrequency = predictFrequency(referenceVoltage);
        return userFrequency - referenceFrequency;
    }
};

// IMPORTANT: Replace these values with the actual slope and intercept from your Python script
const float SLOPE = 0.028213493225044295;   // Your calculated slope
const float INTERCEPT = 104.75800692136673; // Your calculated intercept

// Create an instance of the voltage frequency converter
VoltageFrequencyConverter converter(SLOPE, INTERCEPT);

class OscillatorCalibrator
{
private:
    // REPLACE THESE WITH VALUES FROM YOUR MODEL EXTRACTION
    static constexpr float SCALER_MEAN = 34.875;             // From scaler mean
    static constexpr float SCALER_SCALE = 14.43357630665387; // From scaler scale

    // Polynomial and Model Coefficients (REPLACE THESE!)
    static constexpr float MODEL_INTERCEPT = 114.91149340881194; // From model intercept
    static constexpr float COEF_0 = 0.0;
    static constexpr float COEF_1 = 23.055496;
    static constexpr float COEF_2 = 16.136962;
    static constexpr float COEF_3 = 8.026836;
    // Add more coefficients as needed

    // Temperature and Oscillator Constants
    static constexpr float TC1 = -0.02;
    static constexpr float INITIAL_L = 1e-3;   // 1 mH
    static constexpr float INITIAL_C = 4.7e-9; // 2.35 nF
    static constexpr float REFERENCE_TEMP = 28.0;

    // Scaling input temperature
    float scaleTemperature(float temp)
    {
        return (temp - SCALER_MEAN) / SCALER_SCALE;
    }

    // Polynomial feature transformation
    float predictFrequency(float scaledTemp)
    {
        // Polynomial features calculation
        float poly_0 = 1.0; // Bias term
        float poly_1 = scaledTemp;
        float poly_2 = scaledTemp * scaledTemp;
        float poly_3 = scaledTemp * scaledTemp * scaledTemp;

        // Frequency prediction using coefficients
        return MODEL_INTERCEPT + COEF_0 * poly_0 + COEF_1 * poly_1 + COEF_2 * poly_2 + COEF_3 * poly_3;
    }

public:
    struct OscillatorCalibration
    {
        float inductance;    // Henries
        float capacitance;   // Farads
        float predictedFreq; // kHz
        float frequencyFactor;
        float inductanceDiff;  // Difference in inductance
        float capacitanceDiff; // Difference in capacitance
    };

    OscillatorCalibration calibrate(float inputTemp, float targetFreq,float voltage)
    {
        // Scale temperature
        float scaledTemp = scaleTemperature(inputTemp);

        // Predict frequency
        float predictedFreq = predictFrequency(scaledTemp);
        float frequencyDiff = converter.frequencyDifference(voltage);
        // Calculate frequency change factor
        Serial.println(frequencyDiff);
        targetFreq=targetFreq+frequencyDiff;
        float frequencyFactor = targetFreq / predictedFreq;

        // Temperature delta
        float deltaTempC = inputTemp - REFERENCE_TEMP;

        // Adjust capacitance for temperature effect
        float adjustedC = INITIAL_C * (1 - TC1 * deltaTempC);

        // Calculate new components
        float newL = INITIAL_L * pow(frequencyFactor, 2);
        float newC = adjustedC / pow(frequencyFactor, 2);

        // Compensate for temperature effect on capacitance
        newC = newC / (1 - TC1 * deltaTempC);

        float inductanceDiff = newL - INITIAL_L;
        float capacitanceDiff = newC - INITIAL_C;

        return {
            newL,            // Inductance
            newC,            // Capacitance
            predictedFreq,   // Predicted Frequency
            frequencyFactor, // Frequency Factor
            inductanceDiff,  // Difference in inductance
            capacitanceDiff  // Difference in capacitance
        };
    }

    // Conversion helpers
    String convertToMH(float value)
    {
        return String(value * 1000.0, 3) + " mH";
    }

    String convertToNF(float value)
    {
        return String(value * 1e9, 3) + " nF";
    }
};

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

OscillatorCalibrator calibrator;
const int sensorPin = A0;
// Voltage reference of Arduino (5V or 3.3V)
const float referenceVolt = 3.3;
// Voltage divider ratio of the sensor (common ones: 5 for 25V sensor, 11 for 55V sensor)
const float voltageDividerRatio = 5.0;
void setup()
{
    // Initialize serial communication
    Serial.begin(9600);

    // Initialize DHT sensor
    dht.begin();

    Serial.println("Colpitts Oscillator Frequency Calibration with DHT11");
}

void loop()
{
    // Read temperature from DHT11 sensor
    float temperature = dht.readTemperature();
    //int rawValue = analogRead(sensorPin);

    // Convert the raw value to voltage (0 to referenceVoltage)
    //float measuredVoltage = rawValue * (referenceVoltage / 1023.0);

    // Adjust for the voltage divider ratio
    //float actualVoltage = measuredVoltage * voltageDividerRatio;

    // Check if reading was successful
    if (isnan(temperature))
    {
        Serial.println("Failed to read from DHT sensor!");
        delay(2000);
        return;
    }
    int rawValue = analogRead(sensorPin);

    // Convert the raw value to voltage (0 to referenceVoltage)
    float measuredVoltage = rawValue * (referenceVolt / 1023.0);

    // Adjust for the voltage divider ratio
    float actualVoltage = measuredVoltage * voltageDividerRatio;
    Serial.print("Measured Voltage: ");
    Serial.print(actualVoltage-0.3);
    Serial.println(" V");
    // Example target frequency (you can modify or make this dynamic)
    float targetFreq = 104; // Target frequency in kHz

    // Calibrate oscillator based on current temperature
    auto result = calibrator.calibrate(temperature, targetFreq,actualVoltage-0.4);

    // Print calibration results
    Serial.println("Calibration Results:");
    Serial.print("Input Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    Serial.print("Target Frequency: ");
    Serial.print(targetFreq);
    Serial.println(" kHz");
    Serial.print("Predicted Frequency: ");
    Serial.print(result.predictedFreq);
    Serial.println(" kHz");
    Serial.print("Frequency Factor: ");
    Serial.println(result.frequencyFactor);
    Serial.print("Adjusted Inductance: ");
    Serial.println(calibrator.convertToMH(result.inductance));
    Serial.print("Inductance Difference: ");
    Serial.print(calibrator.convertToMH(result.inductanceDiff));
    //Serial.print(" (");
    // Serial.print(calibrator.calculatePercentageChange(1e-3, result.inductance), 2);
    //Serial.println("%)");

    // Capacitance details
    Serial.print("Adjusted Capacitance: ");
    Serial.println(calibrator.convertToNF(result.capacitance));
    Serial.print("Capacitance Difference: ");
    Serial.println(calibrator.convertToNF(result.capacitanceDiff));
    //Serial.print(" (");
    // Serial.print(calibrator.calculatePercentageChange(4.7e-9, result.capacitance), 2);
    //Serial.println("%)");

    // Wait for 5 seconds before next reading
    delay(2000);
}
