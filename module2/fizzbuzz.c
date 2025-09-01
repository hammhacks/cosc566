#include<stdio.h>
#include<stdlib.h>

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
		char fizz[] = "fizz";
		for(counter=0;counter<4;counter++)
			*(output+counter)=fizz[counter];
		counter_count = counter;
	}
	if(i%5==0){
		char buzz[] = "buzz";
		for(counter=0;counter<4;counter++){
			*(output+counter+counter_count)=buzz[counter];
		}
	}	
	
	if(i%3 == 0 || i%5 == 0){
		printf("%s\n",output);
	}
	else{
		printf("%d\n",i);
	}
}

return 0;
}
