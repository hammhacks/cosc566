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

struct params {
  int initial_range;
  int jump_range;
  int num_jumps;
  int initial_power;
  double power_reduction;
};


void append(node *node_input,node *previous){
  node_input->adj_list = malloc(50000);
  *(node_input->adj_list) = previous;
  node_input=previous;


}



int main(int argc, char **argv) {

  struct params p;
  ssize_t read; // For the return value of getline()
  char *line = NULL;
  size_t len = 0;
 
  struct node *current = malloc(sizeof(node));
  current->name = malloc(1000);
  struct node *previous = malloc(50000);
  int node_count = 0;

  while ((read = getline(&line, &len, stdin)) != -1) {

    sscanf(line, "%d %d %d %d %s", &current->x, &current->y, &current->cur_PP, &current->max_PP, current->name);
    append(current,previous);
    node_count++;
  }

  int i;
  for(i=0;i<node_count;i++){
    printf("node name is: %s\n",current->name);
    current = *(current->adj_list);

  }

  return 0;
}

/*
  int nodecount=5;
  nodelist = malloc(nodecount*sizeof(node));

  int i;
  for(i=0;i<nodecount;i++){
    struct node *temp=malloc(sizeof(node));
    temp->id=i;
    *(nodelist+i) = temp;
  }
  
  for(i=0;i<nodecount;i++){
    printf("node id is: %d\n",(*(nodelist+i))->id);
  }
    */
