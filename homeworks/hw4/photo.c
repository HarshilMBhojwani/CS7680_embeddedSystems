
#include <signal.h>
#include <stdio.h>
#include <pigpio.h>


const int BlueLED=21;
const int RedLED=20;
const int GreenLED=26;
const int pc=16;



//int DEBUG=1;

volatile sig_atomic_t signal_received=0;

void sigint_handler(int signal){
	signal_received=signal;
}


int RCtime(int RCpin){
	int reading =0;
	gpioSetMode(RCpin, PI_OUTPUT);
	gpioWrite(RCpin, PI_LOW);
	time_sleep(0.1);

	gpioSetMode(RCpin, PI_INPUT);
	//This takes about 1 milisecond per loop cylce
	while (gpioRead(RCpin)==PI_LOW){
		reading +=1;
	}
	printf("%d\n", reading);
	return reading;

}


int ledOut(int state){

	if(state==0){
		gpioWrite(RedLED, PI_HIGH);
		gpioWrite(GreenLED, PI_HIGH);
		return 0;
	}

	if(state==1){
		gpioWrite(GreenLED, PI_LOW);
		gpioWrite(RedLED, PI_HIGH);
		return 0;
	}
	if(state==2){
		gpioWrite(GreenLED, PI_LOW);
		gpioWrite(RedLED, PI_HIGH);
		return 0;
	}

	if(state==3){
		gpioWrite(GreenLED, PI_LOW);
		gpioWrite(RedLED, PI_LOW);
		return 0;
	}
	return 1;
}

const char* photocellParse(int reading){
	char* out="";
	if (reading <= 65000){
		out = "0";
		ledOut(0);
		return out;
	}
	if (reading<=150000){
		out = "o";
		ledOut(1);
		return out;
	}

	if (reading <= 350000){
		out = ".";
		ledOut(2);
		return out;	
	}
	out = " ";
	ledOut(3);
	return out;

}

int main(){
	if(gpioInitialise()==PI_INIT_FAILED){
		printf("Error: Failed to Initialize the GPIO interface. \n");
		return 1;
	}
	gpioSetMode(BlueLED, PI_OUTPUT);
	gpioSetMode(RedLED, PI_OUTPUT);
	gpioSetMode(GreenLED, PI_OUTPUT);
	signal(SIGINT, sigint_handler);

	printf("Press CTRL-C to exit the program.\n");	
	while (!signal_received){
		//printf("In the loop");
		gpioWrite(BlueLED, PI_HIGH);
		printf("%s",photocellParse(RCtime(pc)));

	}
        	
        gpioSetMode(RedLED, PI_INPUT);
        gpioSetMode(BlueLED, PI_INPUT);
        gpioSetMode(GreenLED, PI_INPUT);	
	gpioTerminate();
        printf("\n");
        

	return 0;
}



