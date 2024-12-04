#ifndef MODEL_H
#define MODEL_H

// Scaling parameters
const float MEAN = 39.523013;
const float STD_DEV = 17.215408;

// Model coefficients
const float INTERCEPT = 118.915338;
const float COEFF[] = {0.000000, 24.743239, 17.734596, 8.803537};
const int DEGREE = 4;

// Function to predict frequency based on temperature input
float predict_frequency(float temperature);

#endif // MODEL_H
