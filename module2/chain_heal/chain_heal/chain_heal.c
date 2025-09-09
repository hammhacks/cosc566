#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct node {
  char *name;
  int x, y;
  int cur_PP, max_PP;
  int adj_size;
  bool visited;
  struct node **adj_list;
};

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
  int node_count=0;

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

  // ssize_t getline(char **lineptr, size_t *n, FILE *stream);

  struct node head;
  struct node next;

  while ((read = getline(&line, &len, stdin)) != -1) {
        if(node_count == 1){
          head.x = atoi(&line[0]);
          head.y = atoi(&line[2]);
          printf("head.x is: %d\n",head.x);
          printf("head.y is: %d\n",head.y);
        }
        printf("Read %zd characters: %s", read, line);
        node_count +=1;

    }

    


cleanup:

  return 0;
}
