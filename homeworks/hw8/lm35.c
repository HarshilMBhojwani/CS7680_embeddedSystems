#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <math.h>
#include <signal.h>
#include <time.h>


int main(int argc, char * argv[]){

	
	int h;
	double v;
	int loops;
	int speed;
	double percent;
	double start, diff;
	unsigned char buf[3];
	const int redLed=21;
	const int motor=20;
	double temperature;
	
	volatile int running=1;

	void sigint_handler(int signum){
		running=0;
	}


	if(argc >1) loops = atoi(argv[1]);
	else loops = 1000;

	if (argc>2) speed = atoi(argv[2]);
	else speed = 500000;
;


	if (gpioInitialise()<0)return 1;

	h = spiOpen(0, speed, 0);

	if (h<0) return 2;

	start = time_time();
	gpioSetMode(redLed,PI_OUTPUT);
	gpioSetMode(motor, PI_OUTPUT);
	gpioPWM(redLed, 0);
	gpioPWM(motor, 0);

	signal(SIGINT, sigint_handler);	

	printf("Press CTRL-C to exit the program\n");

	for (int i=0;i<loops && running ;i++){

		buf[0]=1;
		buf[1]=128;
		buf[2]=0;


		spiXfer(h,buf, buf, 3);

		v = ((buf[1]&3)<<8)| buf[2];
		temperature = v/10.0;
	
		printf("The current room temperature is = %f\n",temperature);


		if(temperature>=100){

		
			temperature =100;
	

		}
		percent = (temperature/100.0);
		//printf("The first percent = %f\n",percent);

		percent = percent * 255.0;
		//printf("The second percent = %f\n", percent);
		percent = (int)percent;
		//printf("The final percent = %f\n",percent);
			
		gpioPWM(motor, (int)percent);
		printf("The motor speed is set at  = %d percent\n",(int)temperature);
		

		time_sleep(0.5);

	}

	diff = time_time()-start;
	fprintf(stderr, "sps = %.1f @ %d bps (%d/%.1f)\n",(double)loops/diff,speed,loops,diff);

	gpioPWM(redLed, 0);
	gpioPWM(motor, 0);
	gpioSetMode(redLed, PI_INPUT);
	gpioSetMode(motor, PI_INPUT);

	spiClose(h);
	gpioTerminate();
	return 0;


}
