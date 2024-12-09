#include <signal.h>
#include <stdio.h>
#include <pigpio.h>

// setting the gpio pins with the particualr led
const int RedLED=21;
const int GreenLED=24;
const int YellowLED=25;
volatile sig_atomic_t signal_received=0;

void sigint_handler(int signal){
	signal_received=signal;
}

/*
int main(){
	if (gpioInitialise() == PI_INIT_FAILED){
		printf("Error: Failed to initialize the GPIO interfrace. \n");
		return 1;
	}
	gpioSetMode(RedLED, PI_OUTPUT);
	signal(SIGINT, sigint_handler);
	printf("Press CTRL-C to exit.\n");
	while(!signal_received){
		gpioWrite(RedLED, PI_HIGH);
		time_sleep(1);
		gpioWrite(RedLED, PI_LOW);
		time_sleep(1);
                gpioWrite(RedLED, PI_HIGH);
		time_sleep(1);
		gpioWrite(RedLED, PI_LOW);
		time_sleep(1);
                gpioWrite(RedLED, PI_HIGH);
		time_sleep(1);
		gpioWrite(RedLED, PI_LOW);
                time_sleep(1);
	
                gpioWrite(YellowLED, PI_HIGH);
		time_sleep(1);
		gpioWrite(YellowLED, PI_LOW);
                time_sleep(1);
                gpioWrite(YellowLED, PI_HIGH);
		time_sleep(1);
		gpioWrite(YellowLED, PI_LOW);
                time_sleep(1);
                gpioWrite(YellowLED, PI_HIGH);
		time_sleep(1);
		gpioWrite(YellowLED, PI_LOW);
                time_sleep(1);
                
        	gpioWrite(GreenLED, PI_HIGH);
		time_sleep(1);
		gpioWrite(GreenLED, PI_LOW);
		time_sleep(1);
                gpioWrite(GreenLED, PI_HIGH);
		time_sleep(1);
		gpioWrite(GreenLED, PI_LOW);
		time_sleep(1);
                gpioWrite(GreenLED, PI_HIGH);
		time_sleep(1);
		gpioWrite(GreenLED, PI_LOW);
                time_sleep(1);
		        
	}
	gpioSetMode(RedLED, PI_INPUT);
	gpioTerminate();
	printf("\n");
	return 0;
 	}
*/


int main() {
    // Initialize the GPIO library
    if (gpioInitialise() == PI_INIT_FAILED) {
        printf("Error: Failed to initialize the GPIO interface.\n");
        return 1; // Exit with an error code if initialization fails
    }

    // Set the GPIO pins for LEDs as output
    gpioSetMode(RedLED, PI_OUTPUT);
    gpioSetMode(GreenLED, PI_OUTPUT);
    gpioSetMode(YellowLED, PI_OUTPUT);

    // Set up the signal handler for SIGINT (CTRL-C)
    signal(SIGINT, sigint_handler);

    printf("Press CTRL-C to exit.\n");

    // Main loop: runs until signal_received is set
    while (!signal_received) {
        // Turn on the Red LED
        gpioWrite(RedLED, PI_HIGH);
        time_sleep(20); // Keep it on for 20 seconds
        gpioWrite(RedLED, PI_LOW); // Turn off the Red LED

        // Turn on the Green LED
        gpioWrite(GreenLED, PI_HIGH);
        time_sleep(18); // Keep it on for 18 seconds
        gpioWrite(GreenLED, PI_LOW); // Turn off the Green LED

        // Turn on the Yellow LED
        gpioWrite(YellowLED, PI_HIGH);
        time_sleep(2); // Keep it on for 2 seconds
        gpioWrite(YellowLED, PI_LOW); // Turn off the Yellow LED
    }

    // Clean up: reset GPIO pin modes before exiting
    gpioSetMode(RedLED, PI_INPUT);
    gpioSetMode(GreenLED, PI_INPUT);
    gpioSetMode(YellowLED, PI_INPUT);

    gpioTerminate(); // Terminate the GPIO library

    printf("\n");
    return 0; // Exit the program successfully
}

