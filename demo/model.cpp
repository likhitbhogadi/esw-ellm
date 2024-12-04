#include "model.h"

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
