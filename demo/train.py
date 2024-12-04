import numpy as np
import pandas as pd
from sklearn.linear_model import Ridge
from sklearn.preprocessing import PolynomialFeatures, StandardScaler
from sklearn.model_selection import train_test_split, cross_val_score, GridSearchCV
from sklearn.metrics import mean_squared_error
import joblib

# Step 1: Load the Data from a Text File
file_path = 'data.txt'  # Adjust the path to your file
data = pd.read_csv(file_path)

# Step 2: Clean the Frequency Data (remove 'K' and convert to float)
data['Frequency'] = data['Frequency'].str.replace('K', '').astype(float)

# Step 3: Preprocessing
# Feature (Temperature)
X = data['Temperature'].values.reshape(-1, 1)

# Target (Frequency)
y = data['Frequency'].values

# Step 4: Remove Outliers (Optional, based on domain knowledge)
mean_freq = np.mean(y)
std_freq = np.std(y)
outliers_removed = (y > mean_freq - 3 * std_freq) & (y < mean_freq + 3 * std_freq)
X = X[outliers_removed]
y = y[outliers_removed]

# Step 5: Feature Scaling
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

# Step 6: Polynomial Transformation
degree = 3  # You can experiment with the degree
poly = PolynomialFeatures(degree)
X_poly = poly.fit_transform(X_scaled)

# Step 7: Split the data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(X_poly, y, test_size=0.2, random_state=42)

# Step 8: Train the model
ridge_model = Ridge(alpha=1.0)  # Regularized model
ridge_model.fit(X_train, y_train)

# Step 9: Evaluate the model on the test set
y_pred = ridge_model.predict(X_test)
mse = mean_squared_error(y_test, y_pred)
print(f"Mean Squared Error on Test Data: {mse}")

# Step 10: Save the trained model and scaler
model_filename = 'temperature_freq_model.pkl'  # The filename for the saved model
scaler_filename = 'scaler.pkl'  # The filename for the saved scaler
poly_filename = 'poly_transformer.pkl'  # The filename for the saved polynomial transformer

joblib.dump(ridge_model, model_filename)
joblib.dump(scaler, scaler_filename)
joblib.dump(poly, poly_filename)

print(f"Model, scaler, and polynomial transformer trained and saved.")
