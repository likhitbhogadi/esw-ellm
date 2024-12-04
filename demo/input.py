import pandas as pd

target_frequency = 104  # Target frequency

# Step 1: Parse the input data file
input_file = 'raw_data.txt'  # Replace with the name of your input file
data = pd.read_csv(input_file, sep=",", skiprows=1, names=['temp', 'frequency'])

# Step 1.1: Clean up the 'frequency' column to remove the 'K' character and convert to float
data['frequency'] = data['frequency'].str.replace('K', '').astype(float)

# Step 2: Calculate the frequency deviation from the target frequency
data['frequency_deviation'] = target_frequency - data['frequency']

# Step 3: Save the preprocessed data to a new file
processed_file = 'processed_data.txt'
data[['temp', 'frequency_deviation']].to_csv(processed_file, sep=" ", index=False, header=False)
print(f"Processed data saved to {processed_file}")
