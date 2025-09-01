#include<stdio.h>
#include<stdlib.h>


int main(){

int array[100]={0};
array[42] = 123;

printf("array[42] is: %d\n",array[42]);
printf("*(array+42) is: %d\n",*(array+42));

printf("&array[42] is: %d\n",&array[42]);
return 0;

}
