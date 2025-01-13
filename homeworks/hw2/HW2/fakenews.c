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


int temp = 72;// Initial temperature

char input[7];// Input buffer for user commands

// Function to display the welcome message and instructions
void view(){

    printf("Welcoome to the Den\n");
    printf("The set temperature = %d\n",temp);
    printf("Type 't' to be able to set the temperature\n");
    printf("Type x to exit the program");

}

// Function to simulate random temperature fluctuations
int controller() {

	 int maxt = 85; // Maximum allowable temperature
    int mint = 55; // Minimum allowable temperature
    int rmove = 0; // Random value for temperature adjustment

    rmove = (rand() % 10); // Generate a random number between 0 and 9
    if (rmove == 0) temp = temp - 1; // Decrease temperature by 1
    if (rmove == 9) temp = temp + 1; // Increase temperature by 1

    // Ensure the temperature stays within bounds
    if (temp < mint) temp = mint;
    if (temp > maxt) temp = maxt;

    return temp; // Return the updated temperature
}

// Function to control the program's main functionality
int model(){

    view();

	while (1) { // Infinite loop
        controller(); // Update the temperature with random fluctuations
        printf("The current temperature = %d\n", temp);

        // Check if a key has been pressed (_kbhit is platform-dependent)
        if (_kbhit()) {
            // Read a single character from the user
            scanf(" %c", &input[0]);

            // If the user inputs 't', allow them to set a new temperature
            if (input[0] == 't') {
                printf("Set the new temperature: ");
                scanf("%d", &temp); // Update the temperature
                continue; // Skip the rest of the loop iteration
            }

            // If the user inputs 'x', exit the program
            if (input[0] == 'x') {
                exit(0); // Terminate the program
            }
        }

        sleep(1); // Wait for 1 second before the next iteration
    }

    return 0; // This line is never reached
}

int main(int argc, char* argv[]){


	//view();
    model();
	//controller();
	

	return 0;
}
