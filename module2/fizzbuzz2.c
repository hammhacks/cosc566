/*
	Author: Jeff Hammond
	Date: September 5th, 2025
	Course: COSC566
	Instructor: Dr. Haynes
*/

// Needed libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]){

// Define char pointer
char *output;
output = malloc(9);

// Convert input to number and show what input was
int input_number = atoi(*(argv+1));
printf("input_number is: %d\n",input_number);

int i;
for(i=1;i<=input_number;i++){

	// Cycle through--allocate output space for each loop
	int counter;
	int counter_count = 0;
	output = malloc(9);
	if(i%3==0){
		strcat(output,"fizz");
	}
	if(i%5==0){
		strcat(output,"buzz");
	}	
	
	if(i%3 != 0 & i%5 != 0)
		sprintf(output, "%d", i);

		printf("%s\n",output);

}

	free(output);
	output = NULL;

return 0;
}
