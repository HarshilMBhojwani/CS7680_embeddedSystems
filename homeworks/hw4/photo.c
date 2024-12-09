
#include <signal.h>
#include <stdio.h>
#include <pigpio.h>



// setting the gpio pins to the components
const int BlueLED=21;
const int RedLED=20;
const int GreenLED=26;
const int pc=16;



//int DEBUG=1;

volatile sig_atomic_t signal_received=0;

void sigint_handler(int signal){
	signal_received=signal;
}


// enabling the RCpin to take the light readings
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

// as per the input of the light the state which is in the given light will on
int ledOut(int state){

//for 0 state Red and Green will on
	if(state==0){
		gpioWrite(RedLED, PI_HIGH);
		gpioWrite(GreenLED, PI_HIGH);
		return 0;
	}
// for 1 state Green will off, red will be on
	if(state==1){
		gpioWrite(GreenLED, PI_HIGH);
		gpioWrite(RedLED, PI_LOW);
		return 0;
	}

// for 2 state red will be on, green will of
	if(state==2){
		gpioWrite(GreenLED, PI_LOW);
		gpioWrite(RedLED, PI_HIGH);
		return 0;
	}

// for 3 state both will be off	

	if(state==3){
		gpioWrite(GreenLED, PI_LOW);
		gpioWrite(RedLED, PI_LOW);
		return 0;
	}
	return 1;
}

// setting the state as per the photcell reading
const char* photocellParse(int reading){
	char* out="";
	// if the reading is less than 65 state ==0
	if (reading <= 65){
		out = "0";
		ledOut(0);
		return out;
	}

	// if the reading is more than 150 state is 1
	if (reading<=150){
		out = "o";
		ledOut(1);
		return out;
	}
// if the reading is more than 350 state ==2
	if (reading <= 350){
		out = ".";
		ledOut(2);
		return out;	
	}

// else state 3
	out = " ";
	ledOut(3);
	return out;

}

int main(){
	if(gpioInitialise()==PI_INIT_FAILED){
		printf("Error: Failed to Initialize the GPIO interface. \n");
		return 1;
	}

// setting the led as output
	gpioSetMode(BlueLED, PI_OUTPUT);
	gpioSetMode(RedLED, PI_OUTPUT);
	gpioSetMode(GreenLED, PI_OUTPUT);
	signal(SIGINT, sigint_handler);

// starting the loop for the photocell to take the readings
	printf("Press CTRL-C to exit the program.\n");	
	while (!signal_received){
		//printf("In the loop");
		gpioWrite(BlueLED, PI_HIGH);
		printf("%s",photocellParse(RCtime(pc)));

	}
// once the loop ends safely exiting the program
        gpioSetMode(RedLED, PI_INPUT);
        gpioSetMode(BlueLED, PI_INPUT);
        gpioSetMode(GreenLED, PI_INPUT);	
	gpioTerminate();
        printf("\n");
        

	return 0;
}



