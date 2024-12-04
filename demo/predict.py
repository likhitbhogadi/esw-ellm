import numpy as np
import joblib

# Load your model, scaler, and polynomial transformer
model_filename = 'temperature_freq_model.pkl' 
scaler_filename = 'scaler.pkl' 
poly_filename = 'poly_transformer.pkl' 

model = joblib.load(model_filename)
scaler = joblib.load(scaler_filename)
poly = joblib.load(poly_filename)

# Input for temperature
input_temp = float(input("Enter the temperature (°C): "))  
reference_temp = 29  # Reference temperature, typically 25°C
delta_temp = input_temp - reference_temp

# Temperature coefficient (e.g., -0.02)
# TC1 = -0.02  
TC1 = -0.0212
# TC1 = -0.048
# TC1 = 0.00
# Initial values of L and C (for Colpitts oscillator)
initial_L = 1e-3  # 1 mH
initial_C = 2.35e-9  # 2.35 nF

# TC1+=0.0212

# Adjust capacitance for temperature effect
C_adjusted = initial_C * (1 - TC1 * delta_temp)

# Step 1: Scale and transform the temperature input
input_temp_scaled = scaler.transform(np.array([[input_temp]]))  
input_temp_poly = poly.transform(input_temp_scaled)

# Step 2: Predict the frequency for the given temperature (already in kHz)
predicted_freq = model.predict(input_temp_poly)[0]

print(f"Predicted frequency at {input_temp}°C: {predicted_freq:.2f} kHz")

# Input for target frequency
target_freq = float(input("Enter the target frequency (kHz): "))  

# Frequency change factor
f_factor = target_freq / predicted_freq

# Adjust L and C based on f_factor for Colpitts oscillator
new_L = initial_L * (f_factor**2)
new_C = C_adjusted / (f_factor**2)

# Helper function to convert to mH and nF
def convert_to_mh_nf(value, unit_type='L'):
    if unit_type == 'L':  
        return f"{value * 1e3:.3f} mH"  
    elif unit_type == 'C':  
        return f"{value * 1e9:.3f} nF"  

new_C = new_C /(1 - TC1 * delta_temp)

# Output results
print(f"Target frequency: {target_freq:.2f} kHz")
print(f"Change factor: {f_factor:.4f}")
print(f"Temperature-adjusted capacitance: {convert_to_mh_nf(C_adjusted, 'C')}")
print(f"Required inductance (L) for target frequency: {convert_to_mh_nf(new_L, 'L')}")
print(f"Required capacitance (C) for target frequency: {convert_to_mh_nf(new_C, 'C')}")

# Calculate changes in L and C
delta_L = new_L - initial_L
delta_C = new_C - initial_C

# Display changes in inductance and capacitance
print(f"Change in inductance: {convert_to_mh_nf(delta_L, 'L')}")
print(f"Change in capacitance: {convert_to_mh_nf(delta_C, 'C')}")
