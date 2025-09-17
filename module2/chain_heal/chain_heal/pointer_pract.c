#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct node {
  char *name;
  int x, y;
  int cur_PP, max_PP;
  int adj_size;
  bool visited;
  struct node **adj_list;
  //struct node *previous;
}node;

struct node2{
  char x;
};

struct params {
  int initial_range;
  int jump_range;
  int num_jumps;
  int initial_power;
  double power_reduction;
};

void append_node(struct node *input_node,struct node **list ){
  
  if(list == NULL){
    printf("here1\n");

    list = (struct node **) input_node;
    printf("list value name: %s\n",(*list)->name);
  }
  else{
    printf("here2\n");
      struct node *current = *list;
      while(current->adj_list != NULL){
        current = *(current)->adj_list;
      }

      current->adj_list = (struct node **)input_node; 

  }


}



int main(int argc, char **argv) {

  struct node *list;
  struct node *node1;
  struct node *node2;
  struct node *node3;




  node1 = malloc(sizeof(node));
  node1->name = malloc(50*sizeof(char));
  node1->name = "Jeff";
  node1->adj_list = malloc(50*sizeof(node *));


  node2=malloc(sizeof(node));
  node2->name = malloc(50*sizeof(char));
  node2->name = "Sally";
  node2->adj_list = malloc(sizeof(node*));
  node1->adj_list = (struct node **)node2;


  node3=malloc(sizeof(node));
  node3->name = malloc(50*sizeof(char));
  node3->name = "Tom";
  node3->adj_list = malloc(sizeof(node*));
  node2->adj_list = (struct node **)node3;
  //append_node(node3,list);
  //*(list+2)=node3;

  struct node *temp = node1;

  printf("silly test is: %s\n",*(*(node1->adj_list)->adj_list)->name);





  return 0;
}

/*

node1 = malloc(sizeof(node));
  node1->name = malloc(50*sizeof(char));
  node1->name = "Jeff";
  node1->adj_list = malloc(50*sizeof(node *));
  //append_node(node1,list);
  *(list)=node1;

  node2=malloc(sizeof(node));
  node2->name = malloc(50*sizeof(char));
  node2->name = "Sally";
  node2->adj_list = malloc(sizeof(node*));
  node1->adj_list = (struct node **)node2;
  //append_node(node2,list);
  *(list+1)=node2;

  node3=malloc(sizeof(node));
  node3->name = malloc(50*sizeof(char));
  node3->name = "Tom";
  node3->adj_list = malloc(sizeof(node*));
  node2->adj_list = (struct node **)node3;
  //append_node(node3,list);
  *(list+2)=node3;

  int i;
  for(i=0;i<3;i++){
  printf("name: %s\n",(*list)->name);
  list++;}*/