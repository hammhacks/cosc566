#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char *argv[]){
char *output;
output = malloc(9);

int input_number = atoi(*(argv+1));
printf("input_number is: %d\n",input_number);

int i;
for(i=1;i<=input_number;i++){
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

return 0;
}
