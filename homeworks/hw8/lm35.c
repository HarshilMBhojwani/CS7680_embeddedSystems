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
	unsigned char buf[3];
	const int redLed=21;
	double first_temperature = INFINITY;
	double temperature;
	
	volatile int running=0;

	void sigint_handler(int signum){
		running=0;
	}


	if(argc >1) loops = atoi(argv[1]);
	else loops = 1000;

	if (argc>2) speed = atoi(argv[2]);
	else speed = 1000;


	if (gpioInitialise()<0)return 1;

	h = spiOpen(0, speed, 0);

	if (h<0) return 2;

	start = time_time();
	gpioSetMode(redLed,PI_OUTPUT);
	gpioPWM(redLed, 0);

	signal(SIGINT, sigint_handler);	

	printf("Press CTRL-C to exit the program");

	for (int i=0;i<loops && running ;i++){

		buf[0]=1;
		buf[1]=128;
		buf[2]=0;


		spiXfer(h, buf, buf, 3);

		v = ((buf[1]&3)<<8)| buf[2];
		temperature = v/10.0;

		printf("The current room temperature is = %d\n",temperature);

		if(i==1){

			first_temperature=v;
			gpioPWM(redLed, 127);
		}

		if(v>first_temperature){

			gpioPWM(redLed, 255);
		}else if (v<first_temperature){
			gpioPWM(redLed, 0);

		}else{
			gpioPWM(redLed, 80);
		}

		time_sleep(0.2);


	}

	diff = time_time()-start;
	fprintf(stderr, "sps = %.1f @ %d bps (%d/%.1f)\n",(double)loops/diff,speed,loops,diff);

	spiClose(h);
	gpioTerminate();
	return 0;


}
