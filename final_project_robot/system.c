#include <stdio.h>
#include <pigpio.h> // Include Pi GPIO library
#include <unistd.h>

#define I2C_ADDRESS 0x14
#define ADC_CHANNEL_0_CMD 0x170000  
#define ADC_CHANNEL_1_CMD 0x160000  
#define ADC_CHANNEL_2_CMD 0x150000 
#define TRIG_PIN 27
#define ECHO_PIN 22
#define SERVO_PIN 17
#define LEFT_MOTOR_PWM 13
#define RIGHT_MOTOR_PWM 12
#define LEFT_MOTOR_DIR 23
#define RIGHT_MOTOR_DIR 24



void setup() {
    if (gpioInitialise() < 0) {
        printf("GPIO Initialization failed!\n");
        return;
    }

    // Ultrasonic Sensor
    gpioSetMode(TRIG_PIN, PI_OUTPUT);
    gpioSetMode(ECHO_PIN, PI_INPUT);

    // Servo Motor
    gpioSetMode(SERVO_PIN, PI_OUTPUT);

    // TT Motors (PWM and Direction)
    gpioSetMode(LEFT_MOTOR_PWM, PI_OUTPUT);
    gpioSetMode(RIGHT_MOTOR_PWM, PI_OUTPUT);
    gpioSetMode(LEFT_MOTOR_DIR, PI_OUTPUT);
    gpioSetMode(RIGHT_MOTOR_DIR, PI_OUTPUT);

    gpioSetPWMfrequency(LEFT_MOTOR_PWM, 1000);
    gpioSetPWMfrequency(RIGHT_MOTOR_PWM, 1000);

    // Initialize outputs
    gpioWrite(TRIG_PIN, 0);
    gpioWrite(LEFT_MOTOR_DIR, 0);
    gpioWrite(RIGHT_MOTOR_DIR, 0);
    gpioPWM(LEFT_MOTOR_PWM, 0); // Start with 0% duty cycle
    gpioPWM(RIGHT_MOTOR_PWM, 0); // Start with 0% duty cycle
}


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


long measureDistance() {
    long startTime = 0, endTime = 0, duration = 0, distance = 0;
    const long timeout = 30000; // Timeout in microseconds (30 ms)

    // Trigger ultrasonic burst
    gpioWrite(TRIG_PIN, 0);
    usleep(2);
    gpioWrite(TRIG_PIN, 1);
    usleep(10);
    gpioWrite(TRIG_PIN, 0);

    long startWait = gpioTick(); // Record the start of waiting for echo

    // Wait for echo to go high
    while (gpioRead(ECHO_PIN) == 0) {
        if ((gpioTick() - startWait) > timeout) {
            printf("Ultrasonic sensor timeout while waiting for echo to go high.\n");
            return -1; // Indicate error
        }
    }
    startTime = gpioTick(); // Record the start time

    // Wait for echo to go low
    while (gpioRead(ECHO_PIN) == 1) {
        if ((gpioTick() - startTime) > timeout) {
            printf("Ultrasonic sensor timeout while waiting for echo to go low.\n");
            return -1; // Indicate error
        }
    }
    endTime = gpioTick(); // Record the end time

    // Calculate distance in cm
    duration = endTime - startTime;
    distance = duration / 58.0; // Convert duration to distance in cm

    if (distance > 400 || distance <= 0) { // Check for out-of-range values
        printf("Distance out of range: %ld cm\n", distance);
        return -1; // Indicate error
    }

    return distance;
}


void setServoAngle(int angle) {
    // Ensure angle is within the expected range (-45° to 45°)
    if (angle < -45 || angle > 45) {
        printf("Invalid angle: %d. Angle must be between -45 and 45 degrees.\n", angle);
        return;
    }

    // Map angle (-45 to 45) to pulse width (1000 to 2000 μs)
    int pulseWidth = 1500 + (angle * 500) / 45;

    // Set servo PWM pulse width
    int result = gpioServo(SERVO_PIN, pulseWidth);
    if (result < 0) {
        printf("Error setting servo PWM. GPIO library returned: %d\n", result);
    } else {
        printf("Servo angle set to %d degrees (Pulse width: %d us).\n", angle, pulseWidth);
    }
}


void controlMotors(long distance, int gSensor1, int gSensor2, int gSensor3) {
    int speed = 128; // Default speed (~50% duty cycle for rear motors)
    int turnAngle = 0; // Servo angle for steering
    printf("Moving Forward\n");
    if (distance <= 7) {
        // Obstacle detected: Turn left
        printf("Obstacle detected! Distance: %ld cm. Steering left.\n", distance);
        turnAngle = 45; // Steer left
	setServoAngle(turnAngle);
        gpioWrite(LEFT_MOTOR_DIR, 1);  // Rear motors move forward
        gpioWrite(RIGHT_MOTOR_DIR, 1);
        gpioPWM(LEFT_MOTOR_PWM, speed);
        gpioPWM(RIGHT_MOTOR_PWM, speed);
	usleep(500000);
	printf("Obstacle avoided, going Straight\n");
	turnAngle =0;
	setServoAngle(turnAngle);
    } else if (gSensor1 < 100000 || gSensor2 < 100000 || gSensor3 < 100000) {
        // Edge detected: Back up and turn right
        printf("Edge detected! Sensor values: S0 = %d, S1 = %d, S2 = %d. Backing up and steering right.\n",gSensor1, gSensor2, gSensor3);
        gpioWrite(LEFT_MOTOR_DIR, 0);  // Rear motors move backward
        gpioWrite(RIGHT_MOTOR_DIR, 0);
        gpioPWM(LEFT_MOTOR_PWM, speed);
        gpioPWM(RIGHT_MOTOR_PWM, speed);
        usleep(500000); // Back up for 500 ms

        printf("Steering right after edge detection.\n");
        turnAngle = -45; // Steer right
        gpioWrite(LEFT_MOTOR_DIR, 1);  // Rear motors move forward
        gpioWrite(RIGHT_MOTOR_DIR, 1);
        gpioPWM(LEFT_MOTOR_PWM, speed);
        gpioPWM(RIGHT_MOTOR_PWM, speed);
	usleep(500000);
	printf("Edge avoided, steering straight again.\n");
	turnAngle=0;
	setServoAngle(turnAngle);
    } else {
        // Path is clear: Move forward
        printf("Path clear. Moving forward. Distance: %ld cm, Sensor values: S0 = %d, S1 = %d, S2 = %d.\n", 
               distance, gSensor1, gSensor2, gSensor3);
       
        turnAngle = 0; // Center steering
	setServoAngle(turnAngle);
        gpioWrite(LEFT_MOTOR_DIR, 1);  // Rear motors move forward
        gpioWrite(RIGHT_MOTOR_DIR, 1);
        gpioPWM(LEFT_MOTOR_PWM, speed);
        gpioPWM(RIGHT_MOTOR_PWM, speed);
    }

   
}

int main() {
    setup();
    int handle = i2cOpen(1, I2C_ADDRESS, 0);  // bus 1, device address, no flags
    if (handle < 0) {
        printf("Failed to open I2C bus.\n");
        gpioTerminate();
        return -1;
    }
    printf("Robot initialized successfully.\n");

    while (1) {
        long distance = measureDistance();
        if (distance < 0) {
            printf("Ultrasonic sensor error: Failed to measure distance.\n");
            continue;
        }
        printf("Distance measured: %ld cm.\n", distance);

        int sensor0_value = read_adc(handle, ADC_CHANNEL_0_CMD);  // Channel 0 (S0)
        int sensor1_value = read_adc(handle, ADC_CHANNEL_1_CMD);  // Channel 1 (S1)
        int sensor2_value = read_adc(handle, ADC_CHANNEL_2_CMD);  // Channel 2 (S2)

        if (sensor0_value < 0 || sensor1_value < 0 || sensor2_value < 0) {
            printf("ADC error: Failed to read grayscale sensor values.\n");
            continue;
        }
        printf("Grayscale sensor values: S0 = %d, S1 = %d, S2 = %d.\n", sensor0_value, sensor1_value, sensor2_value);

        // Control TT motors
        controlMotors(distance, sensor0_value, sensor1_value, sensor2_value);

        usleep(100000); // 100 ms delay
    }

    gpioTerminate(); // Clean up GPIO resources
    return 0;
}

