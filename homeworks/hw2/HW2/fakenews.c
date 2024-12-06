#include "fakenews.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>// for sleep function
#include <conio.h> // for _keypress function

/*
 *  fakenews() : Function to generate random temperature values to simulate
 *               a temperature sensor issuing random values between 55F and
 *               85F moving by one degree at a time at random times.
 *               Initial temp starts at 72F each time.
 */


int temp = 72;

char input[7];

void view(){

    printf("Welcoome to the Den\n");
    printf("The set temperature = %d\n",temp);
    printf("Type 't' to be able to set the temperature\n");
    printf("Type x to exit the program");

}

int fakenews() {

	int maxt = 85;
	int mint = 55;
	int rmove = 0;
	
	rmove = (rand() % 10);
	if (rmove==0) temp = temp-1;
	if (rmove==9) temp = temp+1;
	if (temp<mint) temp = mint;
	if (temp>maxt) temp = maxt;

	return temp;
}

int controller(){

	while(1){
		
		fakenews();
		printf("The current Temperature = %d\n", temp);

		
		if(_kbhit()){

			scanf(" %c", &input[0]);

			if(input[0]=='t'){
			printf("Set the new the new temperature ");
			scanf("%d", &temp);
			continue;
			}
			if(input[0]=='x'){
			exit(0);
			}


		}

		sleep(1);
		


	}
	

	return 0;






}

int main(int argc, char* argv[]){


	// printf("Welcoome to the Den\n");
	// printf("The set temperature = %d\n", temp);
	// printf("Type 't' to be able to set the temperature\n");
	// printf("Type x to exit the program");

	view();

	controller();

	// while(1){
		
	// 	fakenews();
	// 	printf("The current Temperature = %d\n", temp);

		
	// 	if(_kbhit()){

	// 		scanf(" %c", &input[0]);

	// 		if(input[0]=='t'){
	// 		printf("Set the new the new temperature ");
	// 		scanf("%d", &temp);
	// 		continue;
	// 		}
	// 		if(input[0]=='x'){
	// 		exit(0);
	// 		}


	// 	}

	// 	sleep(1);
		


	// }
	

	return 0;
}
