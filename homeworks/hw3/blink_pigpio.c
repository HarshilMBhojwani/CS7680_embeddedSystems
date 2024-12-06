#include <signal.h>
#include <stdio.h>
#include <pigpio.h>

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
		time_sleep(20);
		gpioWrite(RedLED, PI_LOW);
		//time_sleep(20);
        	gpioWrite(GreenLED, PI_HIGH);
		time_sleep(18);
		gpioWrite(GreenLED, PI_LOW);
		//time_sleep(22);
		gpioWrite(YellowLED, PI_HIGH);
		time_sleep(2);
		gpioWrite(YellowLED, PI_LOW);
	}
	gpioSetMode(RedLED, PI_INPUT);
	gpioTerminate();
	printf("\n");
	return 0;
 	}

