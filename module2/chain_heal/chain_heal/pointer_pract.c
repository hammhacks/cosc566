#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct node {
  char *name;
  int age;
  struct node **adj_list;
  struct node *next;
}node;


int main(int argc, char **argv) {
 
  struct node *list;
  struct node *temp;
  list = malloc(sizeof(list));
  temp = malloc(sizeof(temp));

  list->name = malloc(1000);
  list->name = "Jeff";
  list->age = 37;
  temp->name = malloc(1000);
  temp->name = "Sally";
  temp->age = 24;
  list->next = malloc(sizeof(list));
  list->next = temp;

  printf("list name is: %s\n",list->name);
  printf("list age is: %d\n",list->age);
  
  printf("Address of new temp is: %p\n",temp);
  printf("Address of list->next is: %p\n",list->next);


  

  return 0;
}


