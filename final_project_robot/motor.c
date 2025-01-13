#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>

// Define motor pins
#define LEFT_REAR_PWM_PIN 13
#define RIGHT_REAR_PWM_PIN 12
#define LEFT_REAR_DIR_PIN 23
#define RIGHT_REAR_DIR_PIN 24

// Define PWM parameters
#define PWM_FREQUENCY 1000  // 1 kHz
#define PWM_RANGE 100       // PWM range (0-100)

void initialize_motor_pins() {
    // Initialize PWM and direction pins
    gpioSetMode(LEFT_REAR_PWM_PIN, PI_OUTPUT);
    gpioSetMode(RIGHT_REAR_PWM_PIN, PI_OUTPUT);
    gpioSetMode(LEFT_REAR_DIR_PIN, PI_OUTPUT);
    gpioSetMode(RIGHT_REAR_DIR_PIN, PI_OUTPUT);

    // Set PWM frequency and range
    if (gpioSetPWMfrequency(LEFT_REAR_PWM_PIN, PWM_FREQUENCY) < 0 ||
        gpioSetPWMfrequency(RIGHT_REAR_PWM_PIN, PWM_FREQUENCY) < 0) {
        fprintf(stderr, "Failed to set PWM frequency.\n");
        exit(1);
    }

    if (gpioSetPWMrange(LEFT_REAR_PWM_PIN, PWM_RANGE) < 0 ||
        gpioSetPWMrange(RIGHT_REAR_PWM_PIN, PWM_RANGE) < 0) {
        fprintf(stderr, "Failed to set PWM range.\n");
        exit(1);
    }

    // Set initial PWM values to 0
    gpioPWM(LEFT_REAR_PWM_PIN, 0);
    gpioPWM(RIGHT_REAR_PWM_PIN, 0);
}

void set_motor(int motor, int speed) {
    int dir = (speed >= 0) ? 1 : 0;
    speed = abs(speed);

    // Scale speed to range 50-100 if speed is non-zero
    if (speed != 0) {
        speed = (speed / 2) + 50;
    }

    if (motor == 0) { // Left motor
        gpioWrite(LEFT_REAR_DIR_PIN, dir);
        gpioPWM(LEFT_REAR_PWM_PIN, speed);
    } else if (motor == 1) { // Right motor
        gpioWrite(RIGHT_REAR_DIR_PIN, dir);
        gpioPWM(RIGHT_REAR_PWM_PIN, speed);
    } else if (motor == -1) { // Both motors
        gpioWrite(LEFT_REAR_DIR_PIN, dir);
        gpioPWM(LEFT_REAR_PWM_PIN, speed);
        gpioWrite(RIGHT_REAR_DIR_PIN, dir);
        gpioPWM(RIGHT_REAR_PWM_PIN, speed);
    } else {
        fprintf(stderr, "Invalid motor selection: %d. Use 0, 1, or -1.\n", motor);
    }
}

int main() {
    // Initialize pigpio library
    if (gpioInitialise() < 0) {
        fprintf(stderr, "pigpio initialization failed.\n");
        return 1;
    }

    // Initialize motor pins
    printf("Initializing motor pins...\n");
    initialize_motor_pins();
    printf("Motor pins initialized.\n");

    // Example usage
    printf("Setting left motor to forward at 75%% speed.\n");
    set_motor(0, 75);
    gpioDelay(2000000); // Run for 2 seconds

    printf("Setting right motor to reverse at 50%% speed.\n");
    set_motor(1, -50);
    gpioDelay(2000000); // Run for 2 seconds

    printf("Setting both motors to forward at 100%% speed.\n");
    set_motor(-1, 100);
    gpioDelay(2000000); // Run for 2 seconds

    printf("Stopping both motors.\n");
    set_motor(-1, 0);

    // Terminate pigpio library
    gpioTerminate();

    return 0;
}
