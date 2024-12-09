#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <signal.h>
#include <unistd.h>

/*

gcc -pthread -o mcp3008 mcp3008.c -lpigpio -lrt

mcp3009 [num_of_loops] [speed_of_transmission]
	defaults to 1000000 in both cases

*/

int main(int argc, char *argv[]){

	int i;
	int h;
	int v;
	int v2;
	int brightness;
	float time_blink;

	int loops; //defaultmax number of loops
	int speed; //defailt speed
	const int redLed=21; //led_gpio pin
	double start, diff, sps;
	unsigned char buf[6];//to contin the buffer
	

	volatile sig_atomic_t signal_received=0;

	void sigint_handler(int signal){
		signal_received=signal;
	}

//if no loops are provided in args set this as default
	if (argc>1) loops = atoi(argv[1]);
	else loops = 1000000;

// if no speed is provided in args set this as default
	if (argc>2) speed = atoi(argv[2]);
	else speed = 1000000;

//check iif gpio is working properly
	if (gpioInitialise()<0) return 1;

	h = spiOpen(0, speed, 0);

//set the led mode
	gpioSetMode(redLed, PI_OUTPUT);

// set the initaial state as zero
	gpioPWM(redLed, 0);

	if (h<0) return 2;

	start = time_time();

	while (!signal_received){
		for (i=0;i<loops;i++){

			buf[0]=1;// the value of first byte bit
			buf[1]=128;//the buffer channel 
			buf[2]=0;// data reading stored
			spiXfer(h, buf, buf, 3);// sends the buffer to read the reading
			v=((buf[1]&3)<<8)|buf[2];// shifts the buff 2 to 8 bits to read the ptentiometer value
			
			printf("Reading potentioMeter1: %d\n",v);
		
		//setting the led brightness accoringly as per the potentiometer reading
			brightness=(v*255)/1023;
			//gpioWrite(redLed, PI_HIGH);
			printf("PWM brightness value = %d\n",brightness);
			gpioPWM(redLed, brightness);
			//time_sleep(0.01);

// same process but instead of brighness it is blinking for the same led
			buf[0]=1;
			buf[1]=128 |(1<<4);
			buf[2]=0;
			spiXfer(h, buf, buf, 3);
			v2=((buf[1]&3)<<8)|buf[2];
			printf("Reading potentioMeter2: %d\n",v2);

// the time range is in between 0-1 seconds
			time_blink=(float)(1023-v2)/1023.0;
			printf("Time_blink=%.2f\n",time_blink);
			time_sleep(time_blink);
			gpioPWM(redLed, 0);
			time_sleep(time_blink);
			gpioPWM(redLed, brightness);
					


		}
	}

// Once the loop ends to safely disable tit
	diff = time_time()-start;
	fprintf(stderr, "sps=%.1f @ %d bps (%d/%.1f)\n", (double)loops/diff, speed, loops, diff);
	gpioPWM(redLed, 0);
	gpioWrite(redLed,PI_LOW);
	gpioSetMode(redLed, PI_INPUT);
	spiClose(h);
	gpioTerminate();
	return 0;
	
}
