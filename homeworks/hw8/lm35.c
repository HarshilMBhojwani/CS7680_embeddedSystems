#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <math.h>
#include <signal.h>


int main(int argc, char * argv[]){

	int i;
	int h;
	double v;
	int loops;
	int speed;
	double start, diff, sps;
	unsigned char buf[3]; ///to contin the buffer
	
	const int redLed=21;//led_gpio pin
	double first_temperature = INFINITY;
	double temperature;
	
	volatile int running=0;

	void sigint_handler(int signum){
		running=0;
	}

//if no loops are provided in args set this as default
	if(argc >1) loops = atoi(argv[1]);
	else loops = 1000;//defaultmax number of loops

// if no speed is provided in args set this as default
	if (argc>2) speed = atoi(argv[2]);
	else speed = 1000;//defailt speed

//check iif gpio is working properly
	if (gpioInitialise()<0)return 1;

//set the led mode
	h = spiOpen(0, speed, 0);

	if (h<0) return 2;

	start = time_time();
	gpioSetMode(redLed,PI_OUTPUT);

	// set the initaial state as zero
	gpioPWM(redLed, 0);

	signal(SIGINT, sigint_handler);	

	printf("Press CTRL-C to exit the program");

	for (int i=0;i<loops && running ;i++){

		buf[0]=1;// the value of first byte bit
		buf[1]=128;//the buffer channel 
		buf[2]=0;// data reading stored


		spiXfer(h, buf, buf, 3);// sends the buffer to read the reading

		v = ((buf[1]&3)<<8)| buf[2];// shifts the buff 2 to 8 bits to read the ptentiometer value
		temperature = v/10.0;

		printf("The current room temperature is = %d\n",temperature);

		if (i == 1) {
    // On the first loop iteration, initialize the `first_temperature` with the current value `v`
    first_temperature = v;

    // Set the red LED to half brightness (PWM value = 127 out of 255)
    gpioPWM(redLed, 127);
}

if (v > first_temperature) {
    // If the current temperature `v` is greater than the initial temperature,
    // set the red LED to full brightness (PWM value = 255)
    gpioPWM(redLed, 255);
} else if (v < first_temperature) {
    // If the current temperature `v` is less than the initial temperature,
    // turn off the red LED (PWM value = 0)
    gpioPWM(redLed, 0);
} else {
    // If the current temperature `v` is equal to the initial temperature,
    // set the red LED to a low brightness (PWM value = 80)
    gpioPWM(redLed, 80);
}

// Pause for 0.2 seconds to allow time for the LED state to be visible
time_sleep(0.2);



	diff = time_time()-start;// Calculate the time difference since the start
	fprintf(stderr, "sps = %.1f @ %d bps (%d/%.1f)\n",(double)loops/diff,speed,loops,diff);

	spiClose(h);// Close the SPI handle to free resources
	gpioTerminate();
	return 0;


}
