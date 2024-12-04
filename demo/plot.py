import matplotlib.pyplot as plt

# Initialize lists to store data
temperature = []
frequency = []

# Read data from the text file
with open('data.txt', 'r') as file:
    # Skip the header
    next(file)
    # Read each line and parse the data
    for line in file:
        temp, freq = line.strip().split(',')
        temperature.append(float(temp))
        # Remove 'K' from the frequency value and convert to float
        frequency.append(float(freq.replace('K', '')))

# Filter out data where the temperature difference is not exactly 5
filtered_temperature = [temperature[0]]  # Start with the first temperature
filtered_frequency = [frequency[0]]  # Corresponding frequency for the first temperature

for i in range(1, len(temperature)):
    if temperature[i] - filtered_temperature[-1] == 5:  # Check if the temp difference is 5
        filtered_temperature.append(temperature[i])
        filtered_frequency.append(frequency[i])

# Plotting the graph with the filtered data
plt.figure(figsize=(10, 5))
plt.plot(filtered_temperature, filtered_frequency, marker='o', linestyle='-', color='b', label="Frequency vs Temperature ")

# Labeling each point with its temperature value
for i, txt in enumerate(filtered_temperature):
    plt.text(filtered_temperature[i], filtered_frequency[i], f'{filtered_temperature[i]}°C', fontsize=9, ha='right', color='red')

# Labels and title
plt.xlabel("Temperature (°C)")
plt.ylabel("Frequency (KHz)")
plt.title("Frequency vs Temperature")
plt.legend()
plt.grid(True)
plt.show()
