import joblib
import numpy as np

# Load the saved models
model_filename = 'temperature_freq_model.pkl'
scaler_filename = 'scaler.pkl'
poly_filename = 'poly_transformer.pkl'

ridge_model = joblib.load(model_filename)
scaler = joblib.load(scaler_filename)
poly = joblib.load(poly_filename)

# Get the scaling parameters
mean = scaler.mean_[0]
std_dev = scaler.scale_[0]

# Get the polynomial coefficients
intercept = ridge_model.intercept_
coefficients = ridge_model.coef_

# Prepare the header file contents
header_content = f"""\
#ifndef MODEL_H
#define MODEL_H

// Scaling parameters
const float MEAN = {mean:.6f};
const float STD_DEV = {std_dev:.6f};

// Model coefficients
const float INTERCEPT = {intercept:.6f};
const float COEFF[] = {{{', '.join(f"{coef:.6f}" for coef in coefficients)}}};
const int DEGREE = {len(coefficients)};

// Function to predict frequency based on temperature input
float predict_frequency(float temperature);

#endif // MODEL_H
"""

# Write the header content to 'model.h'
with open("model.h", "w") as f:
    f.write(header_content)

print("model.h file generated successfully.")
