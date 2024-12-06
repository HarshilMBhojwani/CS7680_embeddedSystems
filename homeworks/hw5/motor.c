#include <signal.h>
#include <stdio.h>
#include <pigpio.h>



const int servo=21;
const int  redLed=20;
const int whiteLed=16;
const int greenLed=12;
const int blueLed=24;
const int yellowLed=25;

char input;

int speed;


volatile sig_atomic_t signal_received=0;

void sigint_handler(int signal){

	signal_received=signal;
}


int  main(){

	if (gpioInitialise()==PI_INIT_FAILED){

		printf("Failed to Initialize the GPIO Interface.\n");
		return 1;

	}

	gpioSetMode(servo, PI_OUTPUT);
	gpioSetMode(redLed, PI_OUTPUT);
	gpioSetMode(whiteLed, PI_OUTPUT);
	gpioSetMode(greenLed, PI_OUTPUT);
	gpioSetMode(blueLed, PI_OUTPUT);
	gpioSetMode(yellowLed, PI_OUTPUT);
	//gpioSetMode(servo, PI_OUTPUT);

	signal(SIGINT, sigint_handler);
        gpioWrite(blueLed, 1);
	gpioWrite(greenLed, 0);
	gpioWrite(whiteLed, 0);
	gpioWrite(yellowLed, 0);
	gpioWrite(redLed,0);	 
	


	printf("Press CTRL-C and 'Enter' to exit the program.\n");

	printf("Select Motor Speed?\nPress 's' for Slow\nPress 'm' for Medium.\nPress 'f' for Fast\nPress 'x' for Stop.\nPress 'Enter' to confirm the speed\n");

	while(!signal_received){ 
	        
		if(scanf("%c", &input)==1){
			//printf("You entered:%c\n",input);
			if(input=='s'){
				printf("You entered:%c\n",input);
				gpioWrite(greenLed, 1);
				gpioWrite(blueLed, 1);
				gpioWrite(whiteLed, 0);
				gpioWrite(yellowLed, 0);
				gpioWrite(redLed,0);	
				gpioPWM(servo,85);
				printf("Setting speed to slow\n");
				//time_sleep(5000);
			}else if(input=='m'){
				printf("You entered:%c\n",input);
				gpioPWM(servo, 170);
				gpioWrite(blueLed, 1);
				gpioWrite(whiteLed, 1);
				gpioWrite(greenLed, 0);
				gpioWrite(yellowLed, 0);
				gpioWrite(redLed,0);	
				printf("Setting speed to Medium\n");
				//time_sleep(5000);
			}else if(input=='f'){
				printf("You entered:%c\n",input);
				gpioPWM(servo, 255);
				gpioWrite(blueLed, 1);
				gpioWrite(redLed,1);
				gpioWrite(greenLed, 0);
				gpioWrite(whiteLed, 0);
				gpioWrite(yellowLed, 0);
				printf("Setting speed to Fast\n");
				//time_sleep(5000);
			}else if(input=='x'){
				printf("You entered:%c\n",input);
				gpioWrite(blueLed, 1);
				gpioWrite(yellowLed,1); 
				gpioWrite(greenLed, 0);
				gpioWrite(whiteLed, 0);
				gpioWrite(redLed,0);	
				gpioPWM(servo, 0);
				printf("Stopping motor\n");
				//time_sleep(5000);
		
		
			}
		}

	}
	gpioWrite(blueLed, 0);
	gpioWrite(greenLed, 0);
	gpioWrite(whiteLed, 0);
	gpioWrite(yellowLed, 0);
	gpioWrite(redLed,0);	 
	gpioPWM(servo, 0);	

	

	gpioSetMode(servo, PI_INPUT);
	gpioSetMode(redLed, PI_INPUT);
	gpioSetMode(blueLed, PI_INPUT);
	gpioSetMode(whiteLed, PI_INPUT);
	gpioSetMode(greenLed, PI_INPUT);
	gpioSetMode(yellowLed, PI_INPUT);
	printf("Exiting the program\n");
	gpioTerminate();
	printf("\n");

	return 0;



}
