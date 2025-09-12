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
  struct node *previous;
}node;

struct params {
  int initial_range;
  int jump_range;
  int num_jumps;
  int initial_power;
  double power_reduction;
};

struct answer {
  int best_path_length;
  int current_path_length;
  struct node **best;
  struct node **current;
  int best_healing;
};

struct node **create_nodes_array(size_t *n) {
  
}

bool adjacent(int jump_range, struct node *a, struct node *b) {
}

void setup_graph(int jump_range, size_t num_nodes, struct node **nodes) {
}

void print_adjacency(size_t num_nodes, struct node **nodes) {
}

void check_best(struct params *p, struct answer *a) {
}

void print_best(struct params *p, struct answer *a) {
}

void dfs(struct params *p, struct answer *a, int depth, struct node *n) {
}

int main(int argc, char **argv) {
  if(argc < 6) {
    fprintf(stderr, "usage: %s initial_range jump_range num_jumps initial_power power_reduction < input_file\n", argv[0]);
    return 0;
  }

  struct params p;
  ssize_t read; // For the return value of getline()
  char *line = NULL;
  size_t len = 0;
 

  sscanf(argv[1],"%d",&p.initial_range);
  sscanf(argv[2],"%d",&p.jump_range);
  sscanf(argv[3],"%d",&p.num_jumps);
  sscanf(argv[4],"%d",&p.initial_power);
  sscanf(argv[5],"%lf",&p.power_reduction);

  //checking
  printf("initial range: %d\n",p.initial_range);
  printf("jump range: %d\n",p.jump_range);
  printf("num_jumps: %d\n",p.num_jumps);
  printf("initial_power: %d\n",p.initial_power);
  printf("power_reduction: %f\n",p.power_reduction);
  struct answer a = {0};

  struct node *current;
  current = malloc(sizeof(node));
  struct node *previous;
  

  int node_count=0;
  while ((read = getline(&line, &len, stdin)) != -1) {
      node *previous = malloc(sizeof(node));
      previous = current;

      current = malloc(sizeof(node));
      current->name = malloc(50000); //memory check
      sscanf(line, "%d %d %d %d %s", &current->x, &current->y, &current->cur_PP, &current->max_PP, current->name);

      if(node_count == 0)
        current->previous = NULL;
      else
        current->previous = previous;

      //printf("AAAcurrent address: %p\n",current);
      //printf("AAAprior address: %p\n",current->adj_list);
    
        node_count +=1;

    }

  node **nodes_array[node_count];
  
  int countdown = node_count;
  while(current != NULL){
    printf("current name is: %s\n",current->name);
    nodes_array[countdown-1] = (struct node **)current;
    current = current->previous;
    countdown -=1;
  }

  int i=0;
  for(i=0;i<node_count;i++)
    printf("array at %d: %s\n",i,**nodes_array[i]);



cleanup:

  return 0;
}


          /*printf("head.x is: %d\n",head.x);
          printf("head.y is: %d\n",head.y);
          printf("head.cur_PP is: %d\n",head.cur_PP);
          printf("head.max_PP is: %d\n",head.max_PP);
          printf("head.name is: %s\n",head.name);*/

                    //node *previous = malloc(sizeof(node));

          //current.adj_list = (struct node **) previous;



          /*current->adj_list = (struct node**) previous;



        if(node_count == 0){
          printf(line);
          node *current = malloc(sizeof(node));
          current->name = malloc(50000); //memory check
          sscanf(line, "%d %d %d %d %s", &current->x, &current->y, &current->cur_PP, &current->max_PP, current->name);
          current->adj_list = NULL;
          //printf("current name is: %s\n",current->name);
          printf("STARTcurrent address is %p\n",current);

        }
        else{
          printf(line);
          printf("BBBcurrent address is: %p\n",current);
          node *previous = malloc(sizeof(node));
          previous = current;
          printf("previous name is: %s\n",previous->name);
          //printf("Acurrent address is: %p\n",current);
          //printf("Aprevious address is: %p\n",previous);
          current = malloc(sizeof(node));
          //printf("Bcurrent address is: %p\n",current);
          //printf("Bprevious address is: %p\n",previous);
          current->name = malloc(50000); //memory check
          sscanf(line, "%d %d %d %d %s", &current->x, &current->y, &current->cur_PP, &current->max_PP, current->name);

          current->adj_list = (struct node**) previous;
          //printf("The previous name is: %s\n",previous->name);
        }*/