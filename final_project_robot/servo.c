#include <signal.h>
#include <stdio.h>
#include <pigpio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <stdbool.h>
//#include <wiringPi.h>



#define I2C_ADDR 0x14
#define PRESCALER_ADDR 0x40
#define SERVO_CHANNEL_ADDR 0x22
#define TIMER2_PERIOD_ADDR 0x44 
#define I2C_BUS 1



const int motor1_dir=23;
const int motor1_power=13;
const int motor2_dir=24;
const int motor2_power=12;


const int proximity_sensor1=27;
const int proximity_sensor2=22;



volatile sig_atomic_t signal_recieved=0;

void sigint_handler(int signal){
		signal_recieved=signal;				

}


int writeRegister8(int handle, int reg, int value){

	return i2cWriteByteData(handle, reg, value);
}




int writeRegister16(int handle, int reg, int value){


	int lowByte = value & 0xFF;
	int highByte = (value >>8) & 0xFF;
	if (i2cWriteByteData(handle, reg, lowByte)<0||
		i2cWriteByteData(handle, reg+1, highByte)<0){
		return -1;
	}
	return 0;

}



void calculatePWMRange(int pulseWidth, int period, int *tick){

	*tick = (pulseWidth * period)/20000;

}

int run_rear_wheels(int gpio_pin_1_dir, int gpio_pin_1_power, int gpio_pin_2_power, int gpio_pin_2_dir, int Power, int direction){


	gpioWrite(gpio_pin_1_dir, direction);
	gpioWrite(gpio_pin_2_dir, direction);
	gpioPWM(gpio_pin_1_power, Power);
	gpioPWM(gpio_pin_2_power, Power);
	return 1;


}


int stop_rear_wheels(int gpio_pin_1_power, int gpio_pin_2_power){


	gpioPWM(gpio_pin_1_power, 0);
	gpioPWM(gpio_pin_2_power, 0);

	return 1;
}

bool proximity_sensor(int sensor1, int sensor2){


	 if (gpioRead(sensor1) == PI_HIGH || gpioRead(sensor2) == PI_HIGH) {
       		 return true; 
    	}
    	return false;

}



int main(){


	if (gpioInitialise()<0){



		printf("Error: Failed to Initialize the GPIO interface.\n");
		return 1;	
	}

	printf("pigpio initialized successfully.\n");


	gpioSetMode(proximity_sensor1, PIHIGH);
	gpioSetMode(proximity_sensor2, PIHIGH);

/*
	int handle=i2cOpen(I2C_BUS, I2C_ADDR, 0);

	if(handle <0){

		printf("Failed to open I2C device\n");
		gpioTerminate();
		return 1;
	}


	int prescaler = 351;

	if(writeRegister16(handle, PRESCALER_ADDR, prescaler)<0){

		printf("Failed to set prescaler\n");

		i2cClose(handle);

		gpioTerminate();
		return 1;


	}


	printf("Prescaler set to %d\n", prescaler);

	int period=20000;

	if(writeRegister16(handle, TIMER2_PERIOD_ADDR, period)<0){

		printf("Failed to set timer 2 periods\n");
		i2cClose(handle);
		gpioTerminate();
		return 1;
	}

//	printf("Timer 2 period set to %d\n",period);
		
	int pulseWidth = 2000;
	int tick=0;
	
	calculatePWMRange(pulseWidth, period, &tick);

	printf("The value of Tick = %d\n", tick);

	if (writeRegister16(handle, SERVO_CHANNEL_ADDR, tick)<0){

		printf("Failed to set duty cycle for channel 11\n");
		i2cClose(handle);
		gpioTerminate();
		return 1;

	}

	printf("Servo on channel 2 set to 0 degress\n");


	i2cClose(handle);
	gpioTerminate();
	return 0;
*/

gpioSetMode(motor1_dir, PI_OUTPUT);
gpioSetMode(motor1_power, PI_OUTPUT);
gpioSetMode(motor2_dir, PI_OUTPUT);
gpioSetMode(motor2_power, PI_OUTPUT);




gpioSetPWMfrequency(motor1_power, 1000);

gpioSetPWMfrequency(motor2_power, 1000);

run_rear_wheels(motor1_dir, motor1_power, motor2_power, motor2_dir, 255, 1);

time_sleep(2);

stop_rear_wheels(motor1_power, motor2_power);

gpioSetMode(motor1_dir, PI_INPUT);
gpioSetMode(motor1_power, PI_INPUT);
gpioSetMode(motor2_dir, PI_INPUT);
gpioSetMode(motor2_power, PI_INPUT);

printf("Servo action completed\n");
gpioTerminate();

return 0;
}


