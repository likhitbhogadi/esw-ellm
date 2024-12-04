# Define input and output file paths
input_file = 'data.txt'
output_file = 'output.txt'

# Read data from the input file
data = []
with open(input_file, 'r') as file:
    # Read the header line
    header = file.readline().strip()
    
    # Process the remaining lines with temperature and frequency data
    for line in file:
        parts = line.strip().split(',')
        if len(parts) == 2:
            # Convert temperature to float and keep frequency as a string
            temperature = float(parts[0].strip())
            frequency = parts[1].strip()
            data.append((temperature, frequency))

# Sort the data by temperature
sorted_data = sorted(data, key=lambda x: x[0])

# Write the sorted data to the output file, including the header
with open(output_file, 'w') as file:
    # Write the header
    file.write(f"{header}\n")
    
    # Write the sorted temperature and frequency data
    for temp, freq in sorted_data:
        file.write(f"{temp:.6f} ,{freq}\n")

print("Data has been sorted and written to output.txt")
