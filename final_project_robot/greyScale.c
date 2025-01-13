#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>

// I2C address of the SunFounder Robot HAT ADC (check your module's documentation)
#define ADC_ADDRESS 0x14  // Adjust this if your ADC address is different

// I2C commands for each ADC channel (based on the provided register values)
#define ADC_CHANNEL_0_CMD 0x170000  // Command for ADC channel 0 (S0)
#define ADC_CHANNEL_1_CMD 0x160000  // Command for ADC channel 1 (S1)
#define ADC_CHANNEL_2_CMD 0x150000  // Command for ADC channel 2 (S2)

// Function to read a value from the ADC module's I2C register
int read_adc(int handle, unsigned int command) {
    unsigned char buf[3];  // Buffer to store the 3-byte result from the ADC

    // Convert the command (4-byte) into a 3-byte buffer for I2C communication
    buf[0] = (command >> 16) & 0xFF;  // Most significant byte
    buf[1] = (command >> 8) & 0xFF;   // Middle byte
    buf[2] = command & 0xFF;          // Least significant byte
    
    // Send the I2C command to the ADC module
    int result = i2cWriteDevice(handle, buf, 3);
    if (result < 0) {
        printf("Error writing to ADC\n");
        return -1;
    }

    // Read the result from the ADC module (assuming 3-byte result)
    result = i2cReadDevice(handle, buf, 3);
    if (result < 0) {
        printf("Error reading from ADC\n");
        return -1;
    }

    // Combine the 3 bytes into the full ADC value
    int adc_value = (buf[0] << 16) | (buf[1] << 8) | buf[2];
    return adc_value;
}

int main() {
    // Initialize the pigpio library
    if (gpioInitialise() < 0) {
        printf("pigpio initialization failed\n");
        return -1;
    }

    // Open the I2C bus (bus 1 for Raspberry Pi 3, 4, etc.)
    int handle = i2cOpen(1, ADC_ADDRESS, 0);  // bus 1, device address, no flags
    if (handle < 0) {
        printf("Failed to open I2C bus\n");
        gpioTerminate();
        return -1;
    }

    // Read values from the three sensor channels
    while (1) {
        int sensor0_value = read_adc(handle, ADC_CHANNEL_0_CMD);  // Channel 0 (S0)
        int sensor1_value = read_adc(handle, ADC_CHANNEL_1_CMD);  // Channel 1 (S1)
        int sensor2_value = read_adc(handle, ADC_CHANNEL_2_CMD);  // Channel 2 (S2)

        printf("S0 Value: %d, S1 Value: %d, S2 Value: %d\n", sensor0_value, sensor1_value, sensor2_value);

        usleep(500000);  // Sleep for 500 ms
    }

    // Close the I2C bus and terminate pigpio
    i2cClose(handle);
    gpioTerminate();

    return 0;
}
